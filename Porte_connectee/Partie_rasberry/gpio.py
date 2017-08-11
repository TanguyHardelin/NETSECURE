#coding: utf-8
import RPi.GPIO as GPIO
import time

moteur_en_mvt=0
#	0 pas de mvt
#	1 ouverture
#	2 fermeture
#	3 error

def init():
	"""
		Fonction permettant d'initialiser les port GPIO du raspberry Pi 
	"""
	GPIO.setmode(GPIO.BCM)

	#Bits de controle
	GPIO.setup(2, GPIO.OUT,initial=GPIO.LOW)	#Fermeture
	GPIO.setup(3, GPIO.OUT,initial=GPIO.LOW)	#Ouverture
	GPIO.setup(4, GPIO.OUT,initial=GPIO.LOW)	#Error

	GPIO.setup(17,GPIO.IN)				#capteur FDC sorti
	GPIO.setup(27,GPIO.IN)				#capteur FDC rentree

def RAZ():
	"""
		Met tout les bits à 0
	"""
	GPIO.output(3,GPIO.LOW)
	GPIO.output(2,GPIO.LOW)
	GPIO.output(4,GPIO.LOW)

def ouvre():
	"""
		Ouvre la porte
	"""
	global moteur_en_mvt
	#On passe en mode ouverture	
	moteur_en_mvt=1

	RAZ()

	#Le bit d'ouverture est passé à 1
	GPIO.output(3,GPIO.HIGH)	


	t0=time.time()
	while(time.time()-t0<121):
		#On attend 2 mins, si au bout de 2 min le capteur de FDC n'est pas enclanché on considère que la porte est en erreur.
		if(GPIO.input(17)==1):
			#capteur de FDC, on attend 1 min et on inverse le sens du moteur et on ferme
			time.sleep(60)
			moteur_en_mvt=2
			ferme()
			break

		time.sleep(1)

	if(moteur_en_mvt==1):	
		error()
def ferme():
	"""
		Ferme la porte
	"""
	global moteur_en_mvt
	#On passe en mode fermeture
	moteur_en_mvt=2

	RAZ()
	
	#Le bit de fermeture est passé à 1:
	GPIO.output(2,GPIO.HIGH)

	t0=time.time()
	while(time.time()-t0<121):
		if(GPIO.input(27)==1):
			#capteur de FDC, on retourne au repos
			moteur_en_mvt=0
			break
		time.sleep(1)

	if(moteur_en_mvt!=0):
		error()
	if(moteur_en_mvt==0):
		RAZ()
def error():
	"""
		Passe le programme en mode error
	"""
	GPIO.output(3,GPIO.LOW)
	GPIO.output(2,GPIO.LOW)
	GPIO.output(4,GPIO.HIGH)
	moteur_en_mvt=3


