#-*- coding: utf-8 -*-
import socket
import select
import MySQLdb
from datetime import datetime

hote = ''
port = int(raw_input("Selectionnez un numero de port: "))


global db,cur
db = MySQLdb.connect(host="localhost",    # your host, usually localhost
		     user="root",         # your username
		     passwd="bba9e4d",  # your password
		     db="lanestel")        # name of the data base
cur = db.cursor()

def init_BDD():
	global db,cur
	#TODO: FAIRE EN SORTE DE LIRE LE FICHIER CONTENANT CES INFOS
	db = MySQLdb.connect(host="localhost",    # your host, usually localhost
			     user="root",         # your username
			     passwd="bba9e4d",  # your password
			     db="lanestel")        # name of the data base
	cur = db.cursor()
def verification_BDD_libre():
	fichier=open("/var/www/html/blocage_BDD.txt")
	texte=fichier.read()
	BDD_libre=True
	if(texte=="1"):
		BDD_libre=False
	fichier.close()
	return BDD_libre

def execute_command(command):
	global db
	if(verification_BDD_libre()==True):
		db.commit()
		cursor=db.cursor()
		cursor.execute(command)
		result=cursor.fetchall();
		return result;

init_BDD()
connexion_principale = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
connexion_principale.bind((hote, port))
connexion_principale.listen(5)
print("Le serveur écoute à présent sur le port {}".format(port))

serveur_lance = True
clients_connectes = []
while serveur_lance:
	if(verification_BDD_libre()==True):
		# On va vérifier que de nouveaux clients ne demandent pas à se connecter
		# Pour cela, on écoute la connexion_principale en lecture
		# On attend maximum 50ms
		connexions_demandees, wlist, xlist = select.select([connexion_principale],
		[], [], 0.05)

		for connexion in connexions_demandees:
			connexion_avec_client, infos_connexion = connexion.accept()
			# On ajoute le socket connecté à la liste des clients
			clients_connectes.append(connexion_avec_client)

		# Maintenant, on écoute la liste des clients connectés
		# Les clients renvoyés par select sont ceux devant être lus (recv)
		# On attend là encore 50ms maximum
		# On enferme l'appel à select.select dans un bloc try
		# En effet, si la liste de clients connectés est vide, une exception
		# Peut être levée
		clients_a_lire = []
		try:
			clients_a_lire, wlist, xlist = select.select(clients_connectes,
			[], [], 0.05)
		except select.error:
			pass
		else:
			# On parcourt la liste des clients à lire
			for client in clients_a_lire:
				# Client est de type socket
				msg_recu = client.recv(1024)
				# Peut planter si le message contient des caractères spéciaux
				msg_recu = msg_recu.decode()
				if(msg_recu=="fin"):
					client.send("recu")
					client.close()
					clients_connectes.remove(client)
					break;

				else:
					informations=msg_recu.split("\n")
					nom_appareil=informations[0].split("|")
					tensions=informations[2]+"\n"
					courants=informations[3]+"\n"
					temperatures=informations[4]+"\n"

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

					date_php=str(date.year)+"."+month+"."+day
					date=str(date.year)+"/"+month+"/"+day+" "+hour+":"+minute+":"+second
					

					#Update de la BDD:
					list_tension=tensions.split(",")
					for i in range(1,len(list_tension)):
						if(verification_BDD_libre()==True):
							fichier_log_tension=open("log/log_tension/"+date_php+"_tension_"+nom_appareil[i-1]+".txt","a")
							fichier_log_tension.write(date+','+list_tension[i]+"\n")
							fichier_log_tension.close()

							print("UPDATE lanestel_informations NATURAL JOIN lanestel_ping SET tension='"+list_tension[i]+"' WHERE ip='"+nom_appareil[i-1]+"'")
							execute_command("UPDATE lanestel_informations NATURAL JOIN lanestel_ping SET tension='"+list_tension[i]+"' WHERE ip='"+nom_appareil[i-1]+"'")

					#Update de la BDD:
					list_courant=courants.split(",")
					for i in range(1,len(list_courant)):
						if(verification_BDD_libre()==True):
							fichier_log_courant=open("log/log_courant/"+date_php+"_courant_"+nom_appareil[i-1]+".txt","a")
							fichier_log_courant.write(date+','+list_courant[i]+"\n")
							fichier_log_courant.close()

							print("UPDATE lanestel_informations NATURAL JOIN lanestel_ping SET courant='"+list_courant[i]+"' WHERE ip='"+nom_appareil[i-1]+"'")
							execute_command("UPDATE lanestel_informations NATURAL JOIN lanestel_ping SET courant='"+list_courant[i]+"' WHERE ip='"+nom_appareil[i-1]+"'")

					list_temperature=temperatures.split(",")
					for i in range(1,len(list_temperature)):
						if(verification_BDD_libre()==True):
							fichier_log_temperature=open("log/log_temperature/"+date_php+"_temperature_"+nom_appareil[i-1]+".txt","a")
							fichier_log_temperature.write(date+','+list_temperature[i]+"\n")
							fichier_log_temperature.close()

							print("UPDATE lanestel_informations NATURAL JOIN lanestel_ping SET temperature='"+list_temperature[i]+"' WHERE ip='"+nom_appareil[i-1]+"'")
							execute_command("UPDATE lanestel_informations NATURAL JOIN lanestel_ping SET temperature='"+list_temperature[i]+"' WHERE ip='"+nom_appareil[i-1]+"'")

					client.send("recu")

connexion_principale.close()
