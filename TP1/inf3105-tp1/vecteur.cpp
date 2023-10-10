/*
  INF3105 -- Structures de données et algorithmes
  UQAM | Département d'informatique
  Automne 2019 | TP1 | vecteur.cpp 
*/

#include <assert.h>
#include <math.h>
#include "vecteur.h"
#include "point.h"

/* Constructeur par recopie */
Vecteur::Vecteur(const Vecteur& vect)
  : x(vect.x), y(vect.y) 
{
}

/* Constructeur à partir de deux coordonnées */
Vecteur::Vecteur(double _x, double _y) 
  : x(_x), y(_y)
{}

/* Constructeur à partir de deux points */
Vecteur::Vecteur(const Point& A,const Point& B){
    this->x = A.x - B.x;
    this->y = A.y - B.y;
}

/* Calcul du produit scalaire d'un vecteur */
double Vecteur::prod_scal(const Vecteur& autre)const{
    return (this->x * autre.x + this->y * autre.y);
}

/* Redéfinition opérateur << */
std::ostream& operator << (std::ostream& os, const Vecteur& vect) {
  os << "(" << vect.x << "," << vect.y << ")";
  return os;
}



