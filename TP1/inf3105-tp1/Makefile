# Makefile pour TP1.
# Adaptez ce fichier au besoin (si vous ajouter des fichiers).
#
# Choisir l'une des deux configurations (-g: Debug | -O2: Release)
OPTIONS = -g -O3 -Wall
#OPTIONS = -O2 -Wall

all : tp1

tp1 : tp1.cpp point.o polygone.o vecteur.o
	g++ ${OPTIONS} -o tp1 tp1.cpp point.o polygone.o vecteur.o
	
point.o : point.cpp point.h
	g++ ${OPTIONS} -c point.cpp

polygone.o : polygone.cpp polygone.h point.h
	g++ ${OPTIONS} -c polygone.cpp

vecteur.o : vecteur.cpp vecteur.h point.h
	g++ ${OPTIONS} -c vecteur.cpp

clean:
	rm -rf tp1 *~ *.o

