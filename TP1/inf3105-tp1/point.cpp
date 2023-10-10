/*
  INF3105 -- Structures de données et algorithmes
  UQAM | Département d'informatique
  Automne 2019 | TP1 | point.cpp 
*/

#include <assert.h>
#include <math.h>
#include "point.h"

/* Constructeur par recopie */
Point::Point(const Point& point)
  : x(point.x), y(point.y) 
{
}

/* Constructeur à partir de deux doubles */
Point::Point(double _x, double _y) 
  : x(_x), y(_y)
{}

/* Fonction calculant la distance d'un point à un autre */
double Point::distance(const Point& point) const {
  double xa = x;
  double xb = point.x;
  double ya = y;
  double yb = point.y;
  double dist = sqrt( (xb-xa)*(xb-xa) + (yb-ya)*(yb-ya) );
  return dist;
}

/* Redéfinition opérateur << */
std::ostream& operator << (std::ostream& os, const Point& point) {
  os << "(" << point.x << "," << point.y << ")";
  return os;
}

/* Redéfinition opérateur >> */
std::istream& operator >> (std::istream& is, Point& point) {
  char po, vir, pf;
  is >> po;
  if(is){
    is >> point.x >> vir >> point.y >> pf;
    assert(po=='(');
    assert(vir==',');
    assert(pf==')');
  }
  return is;
}

/* Redéfinition opérateur + */
Point operator + (Point const& A, Point const& B){
  Point ret = Point(A.x+B.x , A.y + B.y);
  return ret;

}

