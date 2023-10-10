/*
  INF3105 -- Structures de données et algorithmes
  UQAM | Département d'informatique
  Automne 2019 | TP1 | polygone.h 
*/

#if !defined(__POLYGONE_H__)
#define __POLYGONE_H__
#include <iostream>
#include "point.h"
#include "tableau.h"


class Polygone {
  public:
    /* Getters & Setters pour des variables supplémentaires */
    int getOrdre(){return this->ordre;};
    void setOrdre(int ordre){this->ordre = ordre;};
    double getAire(){return this->aire;};
    void setAire(double aire){this->aire = aire;}
    char * getName(){return this->name;};
    int getTaille(){return this->tab_points.taille();};

    Polygone(){}

    ~Polygone(){};

    double calculerAire() const;

  private:
    int ordre;
    double aire;
    Tableau<Point> tab_points;
    char name [50];


  friend std::ostream& operator << (std::ostream&, const Polygone&);
  friend std::istream& operator >> (std::istream&, Polygone&);
  friend int distance2Polygones(Tableau <Polygone>, int, int, int, int);
};

#endif

