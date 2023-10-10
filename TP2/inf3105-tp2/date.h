/*
  INF3105 - Structures de données et algorithmes
  UQAM | Département d'informatique
  Automne 2019 | TP2 | date.h
*/

#if !defined(_DATE__H_)
#define _DATE__H_

#include <iostream>

/* Définition de la classe Date */
class Date{
  public:
    Date();
    bool operator <(const Date& date) const;
    bool operator <=(const Date& date) const;
    bool operator ==(const Date& date) const;
    bool operator !=(const Date& date) const;  
  private:
    int jour;
    int heure;
    int minute;
    int cumul;

  friend std::ostream& operator << (std::ostream&, const Date& date);
  friend std::istream& operator >> (std::istream&, Date& date);
};

#endif

