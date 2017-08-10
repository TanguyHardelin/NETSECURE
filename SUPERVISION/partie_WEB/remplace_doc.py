import time
import os


while 1:
	fichier_exist=False
	try:
		#Test si le fichier existe:
		fichier=open("document.xlsx")
		fichier.close()
		fichier_exist=True
	except IOError:
		fichier_exist=False
	
	#Si le fichier existe on le move et on relance la BDD
	if(fichier_exist==True):
		print "deplacement du fichier"
		os.system("sudo mv document.xlsx /var/www/html")		
		os.system("cd /var/www/html")
		print "blocage et reconstruction de la BDD"
		os.system("sudo python /var/www/html/script/python/lecture_exel.py")
		os.system("cd ")
		print "transfert du fichier fini"
	time.sleep(1)
		
