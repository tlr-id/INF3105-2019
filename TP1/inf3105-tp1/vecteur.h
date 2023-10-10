/*
  INF3105 -- Structures de données et algorithmes
  UQAM | Département d'informatique
  Automne 2019 | TP1 | vecteur.h
*/

#if !defined(__VECTEUR_H__)
#define __VECTEUR_H__

#include <iostream>
#include "point.h"


class Vecteur {

  public:
    Vecteur(){}
    Vecteur(double x, double y);
    Vecteur(const Vecteur&);
    Vecteur(const Point&, const Point&);

    ~Vecteur(){};

    double prod_scal(const Vecteur&)const;

  private:
    double x;
    double y;

  friend std::ostream& operator << (std::ostream&, const Vecteur&);
  friend int distancePointSegment(Point, Point, Point);

};

#endif

