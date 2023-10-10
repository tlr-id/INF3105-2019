/*
  INF3105 -- Structures de données et algorithmes
  UQAM | Département d'informatique
  Automne 2019 | TP1 | polygone.cpp
*/

#include <cmath>
#include <limits>
#include "polygone.h"
#include <stdio.h>
#include <string.h>

/**
 * Calcule l'aire d'un polygone.
 *
 * On utilise l'algorithme décrit dans le sujet.
 *
 * @param : void
 * @return : valeur de l'aire.
 */
double Polygone::calculerAire() const{
    int nb_points = tab_points.taille();   
    int j = nb_points-1;
    double area = 0;
    for(int i=0;i<nb_points;i++){
        area = area + ( ( tab_points[j].x + tab_points[i].x ) * ( tab_points[j].y - tab_points[i].y ) );
        j = i;
    } 
    return fabs(area/2);
}

/* Redéfition opérateur << */
std::ostream& operator << (std::ostream& os, const Polygone& polygone){
    int taille = polygone.tab_points.taille();
    os << polygone.name << " : ";
    for(int i=0;i<taille-1;i++) { 
        os <<  polygone.tab_points[i]  << ", ";
    }
    os << polygone.tab_points[taille-1] << " ;";
    return os;
}

/* Redéfition operateur >> */
std::istream& operator >> (std::istream& in, Polygone& polygone){
    char nom[50];
    char deuxpoints;
    in >> nom >> deuxpoints;
    strcpy(polygone.name,nom);
    assert(deuxpoints==':'); 
    char c;
    do{
        Point p;
        in >> p >> c >> std::ws;
        polygone.tab_points.ajouter(p);
    }while(c==',');
    assert(c==';');
    return in;
}