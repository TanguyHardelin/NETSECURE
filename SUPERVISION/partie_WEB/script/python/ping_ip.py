#!/usr/bin/env python
#-*- coding: utf-8 -*-
"""
	Code réalisé par Tanguy HARDELIN pendant un stage chez lanestel.
	
	Ce code permet de ping toute les IP contenues dans la BDD et d'envoyer un SMS au numero de la personne d'astrainte si une IP importante tombe.
"""
import os
import time
import datetime
import subprocess
import MySQLdb
from datetime import datetime

global db,cur
db = MySQLdb.connect(host="localhost",    # your host, usually localhost
		     user="root",         # your username
		     passwd="bba9e4d",  # your password
		     db="lanestel")        # name of the data base
cur = db.cursor()
id_ip_importante=[]
id_ip_error=[]
id_ip_aquitte=[]

def init_BDD():
	global db,cur
	#TODO: FAIRE EN SORTE DE LIRE LE FICHIER CONTENANT CES INFOS
	db = MySQLdb.connect(host="localhost",    # your host, usually localhost
			     user="root",         # your username
			     passwd="bba9e4d",  # your password
			     db="lanestel")        # name of the data base
	cur = db.cursor()

def execute_command(command):
	global db
	
	if(verification_BDD_libre()==True):
		db.commit()
		cursor=db.cursor()
		cursor.execute(command)
		result=cursor.fetchall();
		return result;
	else:
		return ""

def result_ping(ip):
	retourPing=os.popen("ping -c 1 "+ip,"r").read()

	#On fait tout un travaille sur le retour de la commande pour en extraire le ping:
	tmp=retourPing.split("time=")
	ping=tmp[len(tmp)-1]
	ping=ping.split('ms')
	tmp=ping

	ping=ping[0]+'ms'
	if(tmp[0].find('100% packet loss')!=-1 or tmp[0].find('Unreachable')!=-1 or tmp[0].find("0 received")!=-1):
		return "ne repond pas"
	else:
		return ping
	

def execute_ping(ip):
	if(len(ip)>4):
		status_ok=True
		compteur=3
		ping=result_ping(ip)
		while(ping=="ne repond pas" and compteur>0):
			status_ok=False
			ping=result_ping(ip)
			compteur-=1
			time.sleep(10)
		if(compteur==0 and status_ok==False):
			return "ne repond pas"
		elif(compteur>0) and status_ok==False:
			return "ping irregulier"
		return ping
	
def update_log(ip,ping,status_ok):
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



	 ping_history=open("log/log_ping/"+str(date.year)+"."+month+"."+day+"_ping_client_"+str(client[0])+".txt","a")

	 date=str(date.year)+"/"+month+"/"+day+" "+hour+":"+minute+":"+second
	 if(status_ok and ping!='ms'):
		 ping_history.write(str(date)+","+ping.split(" ms")[0]+"\n")
	 else:
		 ping_error=open("log/log_error/error_client_"+str(client[0])+".txt","a")
		 ping_error.write(str(date)+","+"error")
		 ping_error.close()

		 ping_history.write(str(date)+",9999\n")

def get_id_ip_importante():
	global id_ip_importante
	id_ip_importante=[]
	for client in execute_command("select id_client from lanestel_ping where type='important'"):
		id_ip_importante.append(int(client[0]))
	return id_ip_importante


def get_id_ip_error():
	global id_ip_error
	id_ip_error=[]
	for client in execute_command("select id_client from lanestel_ping where type='error'"):
		id_ip_error.append(int(client[0]))
	return id_ip_error


def get_id_ip_aquitte():
	global id_ip_aquitte
	id_ip_aquitte=[]
	for client in execute_command("select id_client from lanestel_ping where type='aquitte'"):
		id_ip_aquitte.append(int(client[0]))
	return id_ip_aquitte

	
def envoie_sms(ip,nom):
	date=datetime.now()
	print "exec envoie_sms.sh "+ip+" "+nom
	os.system("exec /var/www/html/envoie_sms.sh "+ip+" "+nom)
	print str(date.year)+"_"+str(date.month)+"_"+str(date.day)+"_"+str(date.hour)+"_"+str(date.minute)+" -> Un SMS vous a été envoyé"

def verification_BDD_libre():
	fichier=open("/var/www/html/blocage_BDD.txt")
	texte=fichier.read()
	BDD_libre=True
	if(texte=="1"):
		BDD_libre=False
	fichier.close()
	return BDD_libre


#Initialisation pour PING:
print "Lancement du ballayage du ping..."
get_id_ip_importante()
BDD_bloque=False
sms_deja_envoye=False
sms_rappel_deja_envoye=False
heure_envoie_premier_sms=0
nombre_sms_envoye=0


while (1):
	#On lit test tout les clients contenus dans la BDD
	if(verification_BDD_libre()==True):
		for client in execute_command("select id_client from lanestel_ping"):
			compteur=0
			if(verification_BDD_libre()==True):
				for row in execute_command("select ip from lanestel_ping where id_client="+str(client[0])):
					if(verification_BDD_libre()==False):
						BDD_bloque=True
						print "blocage du programme on est en train de toucher à la BDD..."
						break
			
					ip_client=row[0]
					ping=execute_ping(ip_client)
					#print("On traite "+str(client[0])+" ---> "+ip_client+"  ping: "+ping)
					if(ping!="error"):
							status_ok=True
							if(ping!="ne repond pas"):
								get_id_ip_aquitte()
								if(client[0] in id_ip_aquitte):
									execute_command("UPDATE lanestel_ping set type='important' where id_client="+str(client[0]))
									sms_deja_envoye=False
									sms_rappel_deja_envoye=False
									

								command_update="UPDATE lanestel_ping set ping='"+ping+"' where ip='"+ip_client+"'"
							elif(ping=="ping irregulier"):
								pass
							else:
								status_ok=False
								command_update="UPDATE lanestel_ping set ping='999999999ms' where ip='"+ip_client+"'"	
								
								#Je fais ça car des fois python d'update pas les valeurs ...
								id_ip_importante=get_id_ip_importante()
								id_ip_error=get_id_ip_error()
								id_ip_aquitte=get_id_ip_aquitte()

							execute_command(command_update)
							update_log(ip_client,ping,status_ok)

							if(status_ok==False and sms_deja_envoye==False and client[0] in id_ip_importante):
								sms_deja_envoye=True
								heure_envoie_premier_sms=time.time()
								nom_client=execute_command("select nom from lanestel_ping where id_client="+str(client[0]))[0][0]
								envoie_sms(ip_client,nom_client)
								execute_command("UPDATE lanestel_ping set type='error' where id_client="+str(client[0]))
								print("Ip --> "+ip_client+" impossible à ping")
						
							t=time.time()
							if(sms_deja_envoye and sms_rappel_deja_envoye==False and t-heure_envoie_premier_sms>3600 and client[0] in id_ip_error and client[0] not in id_ip_aquitte):
								nom_client=execute_command("select nom from lanestel_ping where id_client="+str(client[0]))[0][0]
								sms_rappel_deja_envoye=True
								print("RAPPEL: Ip --> "+ip_client+" impossible à ping")
								envoie_sms(ip_client,nom_client)
					compteur+=1
			if(BDD_bloque==True):
				break
		#Une fois tout les clients balayé on fait une pause pour soulager le processeur:
		time.sleep(1)
						
					
	else:
		break					
	


print "Programme arrêté"
