# coding: utf-8 

from threading import Thread
import random
import time
import serial
import os
from gpio import *

class Code_Digicode (Thread):
	def __init__(self, serial_digicode):
		Thread.__init__(self)
		self.ser_ecran=serial_digicode
		self.random_code=[]
		self.index_code=0
		self.user_input=""
		self.id_porte=-1
		self.sleeping_time=10
		self.arret=0
		self.gen_random_code()

	def run(self):
		"""
			Fonction executée par le thread. Contient la boucle de simulation de la partie digicode.
		"""
		while(self.check_arret()):
			data=self.ser_ecran.readline().split('\n')[0].split('\r')[0]
			if(len(data)>0):
				print data

			if("Touchscreen started" in data):
				print("Digicode started !")

			if(len(data)==1 and data=='<'):
				self.user_input=self.user_input[:-1]

			elif(len(data)==1 and data=='C'):
				self.user_input=""

			elif(len(data)==1):
				#One number has been type
				self.user_input+=str(data)
				print("Input user "+self.user_input)
				print("clef "+self.random_code[self.index_code])

				if(len(self.user_input)==len(self.random_code[self.index_code])):
					if(self.user_input==self.random_code[self.index_code]):
						if(moteur_en_mvt==0):
							print ("Ouverture porte")
							self.arret=1
							self.index_code+=1
							self.sleeping_time=10
							self.user_input=""
							ouvre()

					else:
						print("Erreur et attente")
						self.ser_ecran.write("E")
						time.sleep(self.sleeping_time)
						self.sleeping_time*=2
						self.ser_ecran.write("N")
						self.user_input=""

	def envoie_code_SMS(self,numero):
		print("NUMERO:"+numero+"FIN")
		print("./envoie_sms.sh "+str(numero)+" "+str(self.random_code[self.index_code]))
		os.system("./envoie_sms.sh "+str(numero)+" "+str(self.random_code[self.index_code]))

	def check_arret(self):
		"""
			Fonction permetant de détecter quand on veut arreter le programme.
		"""
		if(self.arret==0): return True
		return False

	def gen_random_code(self):
		 """
			Fonction retournant 100 codes aléatoires pour le digicode. Ces codes sont aussi écrit dans le fichier .code_file.txt
		 """
		 for i in range (100):
			  code=""
			  for j in range(8):
					code+=str(random.randrange(10))
			  code_file=open(".code_file.txt","a")
			  code_file.write(code+"\n")
			  self.random_code.append(code)



