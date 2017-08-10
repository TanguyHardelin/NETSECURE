# coding: utf-8 

from threading import Thread
import random
import time
import serial
from gpio import *

class Code_Digicode (Thread):
	def __init__(self, serial_digicode):
		Thread.__init__(self)
		self.ser_ecran=serial.Serial("/dev/ttyACM0",9600,timeout=1)
		self.random_code=[]
		self.index_code=0
		self.user_input=""
		self.id_porte=-1
		self.sleeping_time=10
		self.gen_random_code()

	def run(self):
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
							self.index_code+=1
							self.sleeping_time=10
							ouvre()

					else:
						print("Erreur et attente")
						self.ser_ecran.write("E")
						time.sleep(self.sleeping_time)
						self.sleeping_time*=2
						self.ser_ecran.write("N")
						self.user_input=""

	def check_arret(self):
		fichier=open(".arret.txt")
		texte=fichier.read()
		if(texte=="1"):
			return False
		return True

	def gen_random_code(self):
		 for i in range (100):
			  code=""
			  for j in range(8):
					code+=str(random.randrange(10))
			  code_file=open(".code_file.txt","a")
			  code_file.write(code+"\n")
			  self.random_code.append(code)



