#-*- coding: utf-8 -*-

import sys
import time
import os
import xlrd
import MySQLdb


global d,cursor,id_client
db = MySQLdb.connect(host="localhost",    # La machine sur laquel on travail 
		     user="root",         # Le login pour se connecter à la BDD, j'ai mis root mais il serait peut etre mieux de creer un autre user ...
		     passwd="bba9e4d", 	  # Le password
		     db="lanestel")       # Le nom de la base de donnee
cursor = db.cursor()

id_client=0

def init_BDD():
	global d,cursor
	db = MySQLdb.connect(host="localhost",    # La machine sur laquel on travail 
			     user="root",         # Le login pour se connecter à la BDD, j'ai mis root mais il serait peut etre mieux de creer un autre user ...
			     passwd="bba9e4d", 	  # Le password
			     db="lanestel")       # Le nom de la base de donnee
	cursor = db.cursor()


def execute_command(command):
	global db
	db.commit()
	cursor=db.cursor()
	cursor.execute(command)
	result=cursor.fetchall();
	return result;

def init():
	#On supprime la table existante:
	try:
		execute_command("DROP TABLE lanestel_ping")
		execute_command("DROP TABLE lanestel_informations")
	except:
		print "Aucune table trouvée ... Creation de la table"

	#On en cree une nouvelle vide:
	try:
		execute_command("CREATE TABLE lanestel_ping(id_client INT,nom VARCHAR(200) UNIQUE NOT NULL,ip VARCHAR(50),ping VARCHAR(20),type VARCHAR(20))")
		execute_command("CREATE TABLE lanestel_informations(id_client INT,tension VARCHAR(15),courant VARCHAR(15),temperature VARCHAR(15))")
	except:
		print "Une erreur a eu lieu pendant la creation de la table :" + str(sys.exc_info()[0])
		exit(0);

def bloquer_ping(blocage):
	if(blocage==True):
		fichier=open("/var/www/html/blocage_BDD.txt","w")
		fichier.write("1")
		fichier.close()
	else:
		fichier=open("/var/www/html/blocage_BDD.txt","w")
		fichier.write("0")
		fichier.close()

def mettre_feuille_dans_BDD(nom_feuille,type_c):
	"""
		Fonction ajoutant toute les valeurs contenues dans une feuille d'un tableur dans la BDD.
		Attention: La feuille doit toujours avoir le meme format:
			-> les donnees doivent commencer à partir de la ligne 8
			-> les ip doivent etre sur la premiere et les nom correspondant aux ip sur la deuxieme
			-> pas deux fois le meme nom
			-> pas de caractere francais comme é è à ô ...
			-> pas d'espace, mettre de _ à la place

	"""
	global id_client
	wb=xlrd.open_workbook('/var/www/html/document.xlsx');


	liste_feuille=wb.sheet_names()
	for i in range(len(liste_feuille)):
		base_ip=""
		if(liste_feuille[i].decode().find('IP admin')!=-1):
			base_ip="172.17.0."
		elif(liste_feuille[i].decode().find('IP Lan Office')!=-1):
			base_ip="192.168.3."

		if(liste_feuille[i].decode()==nom_feuille):
			sh=wb.sheet_by_name(liste_feuille[i])
			c2=sh.col_values(0)
			c1=sh.col_values(1)
			c3=sh.col_values(2)
			for j in range(len(c2)):
				if(j>7):
					try:
						if(len(base_ip+str(c2[j]))>0 and len(c1[j])>1):
							if(type(c2[j]) is float):
								c2[j]=int(c2[j])
								
							id_client=int(c3[j])
							print 'INSERT INTO lanestel_ping VALUES ("'+str(id_client)+'","'+c1[j]+'","'+base_ip+str(c2[j])+'","0ms","'+type_c+'")'
							execute_command('INSERT INTO lanestel_ping VALUES ("'+str(id_client)+'","'+str(c1[j])+'","'+base_ip+str(c2[j])+'","0ms","'+type_c+'")')
							execute_command('INSERT INTO lanestel_informations VALUES ("'+str(id_client)+'","NS","NS","NS")')
					except:
						print "colomne deja presente ou syntax incorrect"+str(sys.exc_info()[0])
	

#Securitee pour eviter de bloquer la BDD:
print "Blocage du ping"
bloquer_ping(True)
time.sleep(1)
print "Temps d'attente fini"
print "init BDD"
init_BDD()
print "init"
init()
print "init fini"

		
	

list_feuille_a_traiter=["IP VIC","IP admin","IP Lan Office"]

for feuille in list_feuille_a_traiter:
	type_c="normal"
	if(feuille=="IP VIC"):
		type_c="important"
	
	mettre_feuille_dans_BDD(feuille,type_c)

bloquer_ping(False)
