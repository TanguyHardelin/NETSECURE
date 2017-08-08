# -*- coding: utf-8 -*-
import MySQLdb
import sys

 
def recup_command(command):
	result=""
	for i in range(len(command)):
		if(command[i]!="_"):
			result+=command[i]
		else:
			result+=" "
	return result

def execute_command(command):
	#connection à la BDD
	command=recup_command(command)
	db= MySQLdb.connect(host="192.168.3.213",
	user="root",
	passwd="bba9e4d",
	db="lanestel")

	db.commit()
	cursor = db.cursor()
	cursor.execute(command)
	db.commit()
	data = cursor.fetchall()

	db.close()
	return data

fonction=sys.argv[1]

if(fonction=="--execute_command"):
	command=sys.argv[2]
	data=execute_command(command)
	#Petit traitement pour les splits sous Qt:
	result=""
	for row in data:
		for element in row:
			result+=str(element)+"-SEPARATEUR-"
		result+="|SEPARATEUR|"
	print result



elif(fonction=="--get_all_name"):
	data=execute_command("SELECT nom FROM password")
	result=""
	for row in data:
		result+=str(row[0])+"|SEPARATEUR|"
	print result


elif(fonction=="--get_all_ip"):
	data=execute_command("SELECT ip FROM password")
	result=""
	for row in data:
		result+=str(row[0])+"|SEPARATEUR|"
	print result
