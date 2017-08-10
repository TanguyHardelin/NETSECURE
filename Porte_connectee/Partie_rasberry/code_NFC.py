# coding: utf-8 

from threading import Thread
import datetime
import time
import serial
from gpio import *

import threading
 
class FuncThread(threading.Thread):
    def __init__(self, target, *args):
        self._target = target
        self._args = args
        threading.Thread.__init__(self)
 
    def run(self):
        self._target(*self._args)

class Code_NFC (Thread):
	def __init__(self, serial_NFC,db, id_porte):
		Thread.__init__(self)
		self.ser_NFC=serial.Serial("/dev/ttyACM1",115200,timeout=1)
		self.infos=[]
		self.db=db
		self.id_porte=id_porte

		self.old_clef=""
		self.t1=0	#Permet d'eviter d'avoir plein de log pour une clef une clef inutilement

	def run(self):
			while(self.check_arret()):
				#############################
				#			MIS A JOUR			 #
				#############################
				self.mise_a_jour()
				#############################
				#			LECTURE NFC  		 #
				#############################
				data=self.ser_NFC.readline().split('\n')[0].split('\r')[0]
				if(len(data)>1):

					if(self.verification_clef(data)==True):
						if(moteur_en_mvt==0):
							print("Ouverture porte")	
							t0=FuncThread(ouvre)
							t0.start()
							t0.join()
							while(self.ser_NFC.readline().split('\n')[0].split('\r')[0]!=""):pass
							data=""
						else:
							data=""


	

	def check_arret(self):
		fichier=open(".arret.txt")
		texte=fichier.read()
		if(texte=="1"):
			return False
		return True

	def mise_a_jour(self):
		cur=self.db.cursor()
		cur.execute("SELECT * FROM access WHERE id_porte="+str(self.id_porte))


		id_utilisateur=[]
		for row in cur.fetchall():
			id_utilisateur.append(row[0])

		for i in range(len(id_utilisateur)):
			cur=self.db.cursor()
			cur.execute("SELECT id_utilisateur,nom,clef,heure_debut,heure_fin,date_debut,date_fin FROM utilisateurs WHERE id_utilisateur="+str(id_utilisateur[i]))
			result=cur.fetchall()
			self.infos.append({})
			self.infos[i]["id_utilisateur"]=id_utilisateur[i]
			self.infos[i]["nom"]=result[0][1]
			self.infos[i]["clef"]=result[0][2]
			self.infos[i]["heure_debut"]=result[0][3]
			self.infos[i]["heure_fin"]=result[0][4]
			self.infos[i]["date_debut"]=result[0][5]
			self.infos[i]["date_fin"]=result[0][6]


	def verification_clef(self,clef):
		#Variables d'authetification:
		clef_OK=False
		horraire_OK=False
		date_OK=False

		copie_user={}
		print(str(self.infos))
		for user in self.infos:
			try:
				if(user["clef"]==clef):
					clef_OK=True
					copie_user=user
			except:pass
		
		if(clef_OK==True):
			now = datetime.datetime.now()
			
			if(copie_user["heure_debut"]=="NONE"):
				horraire_OK=True
			else:
				#On verifie si l'uilisateur est bien dans ses horraires prévus
				heure_debut=copie_user["heure_debut"].split(":")[0]
				min_debut=copie_user["heure_debut"].split(":")[1]

				heure_fin=copie_user["heure_fin"].split(":")[0]
				min_fin=copie_user["heure_fin"].split(":")[1]


				if(now>=now.replace(hour=int(heure_debut),minute=int(min_debut)) and now<=now.replace(hour=int(heure_fin),minute=int(min_fin))):
					horraire_OK=True
			if(copie_user["date_debut"]=="NONE"):
				date_OK=True
			else:
				annee_debut=copie_user["date_debut"].split("/")[2]
				mois_debut=copie_user["date_debut"].split("/")[1]
				jour_debut=copie_user["date_debut"].split("/")[0]

				annee_fin=copie_user["date_fin"].split("/")[2]
				mois_fin=copie_user["date_fin"].split("/")[1]
				jour_fin=copie_user["date_fin"].split("/")[0]
				
				if(now>=now.replace(year=int(annee_debut),month=int(mois_debut),day=int(jour_debut)) and now<=now.replace(year=int(annee_fin),month=int(mois_fin),day=int(jour_fin))):
					date_OK=True

			if(clef_OK==True and horraire_OK==True and date_OK==True):
				cur=self.db.cursor()
				#Recuperation de l'identitee de la personne:
				cur.execute("SELECT nom FROM utilisateurs WHERE clef='"+clef+"'")
				nom=cur.fetchall()[0][0]
			
				#On cree le log et on lle met dans la BDD:
				if(time.time()-self.t1>=300 or self.old_clef!=clef):	
					#300 sec = 5 min
					log=str(now)+" "+nom
					print(log)
					cur=self.db.cursor()
					cur.execute("INSERT INTO log(id_porte,log) VALUES ("+str(self.id_porte)+",'"+log+"')")
					self.db.commit()

					self.old_clef=clef
					self.t1=time.time()


		return(clef_OK==True and horraire_OK==True and date_OK==True)
			
	
