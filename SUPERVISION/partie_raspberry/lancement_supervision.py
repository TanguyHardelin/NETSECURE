# -*- coding: utf-8  -*-

import os
import sys
import subprocess
import smbus
import time
import socket
from datetime import datetime
import os.path


global hote,port,nom_appareil,connexion_avec_serveur
hote=""
port=""
nom_appareil=""
coef=0.0
connexion_avec_serveur = socket.socket(socket.AF_INET, socket.SOCK_STREAM)


def init():
	global hote,port,nom_appareil,connexion_avec_serveur,coeff
	print os.path.isfile("param.txt")
	if(os.path.isfile("param.txt")==True): 
		fichier_param=open("param.txt","r")
		text=fichier_param.read()
		
		hote=text.split(',')[0]
		port=text.split(',')[1]
		nom_appareil=text.split(',')[2]
		coef=float(text.split(',')[3])
		
		
		print hote
		print port
		print nom_appareil

		connexion_avec_serveur.connect((hote, int(port)))
	else:
		hote=raw_input("IP de l'hote ")
		port=raw_input("Port de connection ")
		nom_appareil=raw_input("Nom du raspberry ")
		ip0=raw_input("Ip du premier appareil a superviser ")
		ip1=raw_input("Ip du deuxieme appareil a superviser ")
		ip2=raw_input("Ip du troisieme appareil a superviser ")
		ip3=raw_input("Ip du quatrieme appareil a superviser ")

		print("Nous supposons que I=kV")
		coef=float(raw_input("Coefficient k= "))
		text=hote+","+port+","+ip0+"|"+ip1+"|"+ip2+"|"+ip3+","+str(coef)
		fichier_param=open("param.txt","w")
		fichier_param.write(text)
		fichier_param.close()
		init()

def envoyer(msg):
	msg_a_envoyer=msg
	
    	msg_a_envoyer = msg_a_envoyer.encode()
    	connexion_avec_serveur.send(msg_a_envoyer)
	while(connexion_avec_serveur.recv(1024)!="recu"):
		time.sleep(1)

def get_date():
	date=datetime.now()
	if len(str(date.month))==1:
		month="0"+str(date.month)
	else:
		month=str(date.month)

	if len(str(date.day))==1:
		day="0"+str(date.day)
	else:
		day=str(date.day)

	if len(str(date.hour))==1:
		hour="0"+str(date.hour)
	else:
		hour=str(date.hour)

	if len(str(date.minute))==1:
		minute="0"+str(date.minute)
	else:
		minute=str(date.minute)

	if len(str(date.second))==1:
		second="0"+str(date.second)
	else:
		second=str(date.second)

	date=str(date.year)+"/"+month+"/"+day+" "+hour+":"+minute+":"+second
	return date

def read_analog_input():
	global coef
	list_tension=os.popen("./lecture_tension","r").read().split("\n")

	date=get_date()
	msg=""
	msg=nom_appareil+"\n"

	#On s'occupe des tension (les 4 premieres)
	msg+=date+","
	for i in range(4):
		if(i!=3):
			msg+=list_tension[i]+","
		else:
			msg+=list_tension[i]
	msg+="\n"
	#On s'occupe des courants (les 4 dernieres)
	msg+=date+","
	for i in range(4,8):
		if(i!=7):
			msg+=str(coef*float(list_tension[i]))+","
		else:
			msg+=str(coef*float(list_tension[i]))
	msg+="\n"
	return msg

def read_tmp():
	#######################################################################
	#								      #
	#	AJOUTER LE CODE POUR 3 autres capteurs de temperature ici     #
	#								      #
	#######################################################################
	
	bus = smbus.SMBus(1)
	data = bus.read_i2c_block_data(0x48, 0)

	msb = data[0]
	lsb = data[1]

	tmp0=(((msb << 8) | lsb) >> 4) * 0.0625

	msg=get_date()+","
	#Partie du code a changer:
	for i in range(4):
		msg+=str(tmp0)
		if(i<3):
			msg+=","
	msg+="\n"
	return msg
		
init()
arret=0
while(int(arret)==0):
	text=""
	text+=read_analog_input()
	text+=read_tmp()
	print(text)
	envoyer(text)

	fichier_arret=open(".arret.txt","r")
	arret=fichier_arret.read().split("\n")[0]
	fichier_arret.close()

	time.sleep(1)

envoyer("fin")
print ("fin de la communication")
connexion_avec_serveur.close()
print ("fin du programme")
