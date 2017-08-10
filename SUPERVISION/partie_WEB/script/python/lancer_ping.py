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

while (1):
	while(verification_BDD_libre()==False):
		time.sleep(1)

	print "Relancement du programme"
	os.system("sudo python /var/www/html/script/python/ping_ip.py")
