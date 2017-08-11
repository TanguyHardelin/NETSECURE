# coding: utf-8 

import serial
import getpass
import MySQLdb
import time
import threading
import thread
import os


from code_NFC import *
from code_Digicode import *
from gpio import *


def installation():
	text=""

	print ("##########################################")
	print ("####              Server              ####")
	print ("##########################################")
	ip=raw_input("IP du serveur où est installé la BDD: ")
	user=raw_input("user: ")
	pwd=getpass.getpass("password: ")
	db=raw_input("Nom de la BDD")

	text+="<ip>"+ip+"</ip>"
	text+="<user>"+user+"</user>"
	text+="<pwd>"+pwd+"</pwd>"
	text+="<DB>"+db+"</DB>"

	print ("##########################################")
	print ("####              Porte               ####")
	print ("##########################################")
	nom_porte=""

	db = MySQLdb.connect(host=ip,user=user,passwd=pwd,db=db)

	ok=True
	while(1):
		nom_porte=raw_input("Nom de la porte")


		#On regarde si le nom existe déjà:
		cursor=db.cursor()
		cursor.execute("SELECT * FROM porte WHERE nom='"+nom_porte+"'")
		if(len(cursor.fetchall())>0):
			c=raw_input("Une porte à ce nom existe déjà, voulez vous continuer ? [O/N]")	

			#On s'assure que la reponse est correct:
			while(c[0].upper()!='O' and c[0].upper()!='N'):
				c=raw_input("Une porte à ce nom existe déjà, voulez vous continuer ? [O/N]")	

			
			if(c[0].upper()=='O'):
				tmp=db.cursor()
				tmp.execute("SELECT id_porte FROM porte WHERE nom='"+nom_porte+"'")
				id_porte=int(tmp.fetchall()[0][0])
				print id_porte
				ok=False
				break

	
	if(ok==True):
		cursor=db.cursor()
		cursor.execute("INSERT INTO porte(nom) VALUE ('"+nom_porte+"')")
		db.commit()

	text+="<nom_porte>"+nom_porte+"</nom_porte>"
	
	fichier_installation=open(".param.xml","w")
	fichier_installation.write(text)
	fichier_installation.close()

	
def parse_xml(xml,separateur):
	try:
		return xml.split("<"+separateur+">")[1].split("</"+separateur+">")[0]
	except: return ""

def connexion_BDD():
	global db,id_porte
	fichier=open(".param.xml","r")
	text=fichier.read()
	fichier.close()
	
	ip=parse_xml(text,"ip")
	user=parse_xml(text,"user")
	pwd=parse_xml(text,"pwd")
	db=parse_xml(text,"DB")
	nom_porte=parse_xml(text,"nom_porte")
	print("ip: "+ip)
	print("user: "+user)
	print("pwd: "+pwd)
	print("db: "+db)
	db = MySQLdb.connect(host=ip,user=user,passwd=pwd,db=db)


	#Recuperation de l'id de la porte:
	tmp=db.cursor()
	tmp.execute("SELECT id_porte FROM porte WHERE nom='"+nom_porte+"'")
	id_porte=int(tmp.fetchall()[0][0])

def initialisation_arduino_ecran(index):
	global ser_ecran
	#On ne sait pas de base sur quel port est l'arduino
	#On tente donc les ports les uns à la suite des autres
	#si ça marche pas on tente sur le suivant de maniere recursive 
	try:
		#Ouverture des liaisons series:
		ser_ecran=serial.Serial("/dev/ttyACM"+str(index),9600,timeout=1)
		c=raw_input("Un arduino a été trouvé en /dev/ttyACM"+str(index)+" confirmez vous ? [O/N]")

		#On s'assure que la reponse est correct:
		while(c[0].upper()!='O' and c[0].upper()!='N'):
			c=raw_input("Un arduino a été trouvé en /dev/ttyACM"+str(index)+" confirmez vous ? [O/N]")	
		
		if(c[0].upper()=='O'):
			return index;
		else:
			raise serial.serialutil.SerialException
		
	
	except serial.serialutil.SerialException:
		time.sleep(0.05)
		index+=1
		if(index>100):index=0
		initialisation_arduino_ecran(index)


def initialisation_arduino_NFC(index,index_ecran):
	global ser_NFC
	#On ne sait pas de base sur quel port est l'arduino
	#On tente donc les ports les uns à la suite des autres
	#si ça marche pas on tente sur le suivant de maniere recursive 
	try:
		#Ouverture des liaisons series:
		ser_NFC=serial.Serial("/dev/ttyACM"+str(index),115200,timeout=1)
		if(index!=index_ecran):
			c=raw_input("Un arduino a été trouvé en /dev/ttyACM"+str(index)+" confirmez vous ? [O/N]")

			#On s'assure que la reponse est correct:
			while(c[0].upper()!='O' and c[0].upper()!='N'):
				c=raw_input("Un arduino a été trouvé en /dev/ttyACM"+str(index)+" confirmez vous ? [O/N]")	
			
			if(c[0].upper()=='O'):
				return ;
			else:
				raise serial.serialutil.SerialException

		else:
			raise serial.serialutil.SerialException
		
	
	except serial.serialutil.SerialException:
		time.sleep(0.05)
		index+=1
		if(index>100):index=0
		initialisation_arduino_NFC(index,index_ecran)

	

def verification_mis_a_jour():
	"""
		Vérification de la mise à jour
	"""
	infos=mise_a_jour()
	

################################################################
#										Installation							#
################################################################

#On verifie si l'installation a ete faite:
if(os.path.isfile(".param.xml")==False):
	 installation()
else:
	#On enleve l'ancien fichier de code
	os.system("rm .code_file.txt")


################################################################
#				Installation et detection des arduinos					#
################################################################

t=raw_input("Voulez vous un ecran de branché sur la porte? [O/N]")

#On s'assure que la reponse est correct:
while(t[0].upper()!='O' and t[0].upper()!='N'):
	t=raw_input("Voulez vous un ecran de branché sur la porte? [O/N]")

index_ecran=-1
if(t[0].upper()=='O'):
	print("Branchez l'arduino de l'ecran sur une liaison USB du raspberry")
	index_ecran=initialisation_arduino_ecran(0)

print("Installation de l'arduino NFC")
print("Branchez l'arduino de NFC sur une liaison USB du raspberry")
initialisation_arduino_NFC(0,index_ecran)
connexion_BDD()

#Initialisation du GPIO:
init()

thread_NFC=Code_NFC(ser_NFC,db,id_porte)
thread_digicode=Code_Digicode(ser_ecran)

thread_NFC.start()
thread_digicode.start()

thread_digicode.join()
thread_NFC.join()



