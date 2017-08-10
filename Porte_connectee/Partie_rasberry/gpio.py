import RPi.GPIO as GPIO
import time

moteur_en_mvt=0
#	0 pas de mvt
#	1 ouverture
#	2 fermeture
#	3 error

def init():
	GPIO.setmode(GPIO.BCM)

	#Bits de controle
	GPIO.setup(2, GPIO.OUT,initial=GPIO.LOW)	#Fermeture
	GPIO.setup(3, GPIO.OUT,initial=GPIO.LOW)	#Ouverture
	GPIO.setup(4, GPIO.OUT,initial=GPIO.LOW)	#Error

	GPIO.setup(17,GPIO.IN)				#capteur FDC sorti
	GPIO.setup(27,GPIO.IN)				#capteur FDC rentree

def RAZ():
	GPIO.output(3,GPIO.LOW)
	GPIO.output(2,GPIO.LOW)
	GPIO.output(4,GPIO.LOW)

def ouvre():
	global moteur_en_mvt
	#On passe en mode ouverture	
	moteur_en_mvt=1

	RAZ()
	GPIO.output(3,GPIO.HIGH)
	t0=time.time()
	while(time.time()-t0<121):
		if(GPIO.input(17)==1):
			#capteur de fin de course enclanchee
			#On passe en mode fermeture
			moteur_en_mvt=2
			ferme()
			break

		time.sleep(1)

	if(moteur_en_mvt==1):	
		error()
def ferme():
	global moteur_en_mvt

	RAZ()
	GPIO.output(2,GPIO.HIGH)
	t0=time.time()
	while(time.time()-t0<121):
		if(GPIO.input(27)==1):
			#capteur de fin de course enclanchee
			#On retourne au repos:
			moteur_en_mvt=0
	


			break
		time.sleep(1)

	if(moteur_en_mvt!=0):
		error()
	if(moteur_en_mvt==0):
		RAZ()
def error():
	GPIO.output(3,GPIO.LOW)
	GPIO.output(2,GPIO.LOW)
	GPIO.output(4,GPIO.HIGH)
	moteur_en_mvt=3


