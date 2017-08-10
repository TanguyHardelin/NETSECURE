import os
import time


def verification_BDD_libre():
	fichier=open("/var/www/html/blocage_BDD.txt")
	texte=fichier.read()
	BDD_libre=True
	if(texte=="1"):
		BDD_libre=False
	fichier.close()
	return BDD_libre

execution_ping=True
while (execution_ping):
	while(verification_BDD_libre()==False):
		time.sleep(1)

	#On clear tout les logs:
#	os.system("sudo rm /var/www/html/log/log_ping/*.txt")

	print "Relancement du programme"
	if(verification_BDD_libre()==True):
		os.system("sudo python /var/www/html/script/python/ping_ip.py")
