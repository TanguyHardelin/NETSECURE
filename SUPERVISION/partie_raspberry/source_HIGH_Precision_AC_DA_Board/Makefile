all:lecture_tension

ADS1256.o: ADS1256.c ADS1256.h
	gcc -c ADS1256.c -o ADS1256.o


lecture_tension.o:lecture_tension.c ADS1256.h
	gcc -c lecture_tension.c -o lecture_tension.o -lbcm2835

lecture_tension:lecture_tension.o ADS1256.o
	gcc lecture_tension.o ADS1256.o -o lecture_tension -lbcm2835

