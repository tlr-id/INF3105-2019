/*
  INF3105 - Structures de données et algorithmes
  UQAM | Département d'informatique
  Automne 2019 | TP2 | date.cpp
*/

#include "date.h"
#include <cstdio>
#include <iomanip>
#include <assert.h>

Date::Date()
{
  jour = 0;
  heure = 0;
  minute = 0;
}

/* Redéfinition opérateur < */
bool Date::operator <(const Date& d) const{
    if(this->cumul < d.cumul){
      return true;
    }
    return false;
} 

/* Redéfinition opérateur <= */
bool Date::operator <= (const Date& d) const{
    
    /*if( (10000*this->jour + 100*this->heure + this->minute) <= (10000*d.jour + 100*d.heure + d.minute)){
      return true;
    }
    return false;
    */
   if(this->cumul <= d.cumul){
      return true;
    }
    return false;
}

/* Redéfinition opérateur == */
bool Date::operator ==(const Date& d) const{
    
    if(this->cumul == d.cumul){
      return true;
    }
    return false;
}

/* Redéfinition opérateur != */
bool Date::operator !=(const Date& d) const{
    
    if(this->cumul != d.cumul){
      return true;
    }
    return false;
}

/* Redéfinition opérateur << */
std::ostream& operator << (std::ostream& os, const Date& d){
    char chaine[40];
    sprintf(chaine, "%dj_%02dh%02dm", d.jour, d.heure, d.minute);
    os << chaine;
    return os;
}

/* Redéfinition opérateur >> */
std::istream& operator >> (std::istream& is, Date& d){
    char j, m, h, underscore;
    is >> d.jour >> j >> underscore >> d.heure >> h >> d.minute >> m;
    d.cumul = 10000*d.jour + 100*d.heure + d.minute;
    assert(j=='j');
    assert(underscore=='_');
    assert(h=='h' && m=='m');
    return is;
}