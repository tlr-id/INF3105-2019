/*  TP3
    carte.h
    LE ROUX Thomas
    LERT28049705
    19/12/19
*/

#if !defined(_CARTE__H_)
#define _CARTE__H_
#include <istream>
#include <list>
#include <set>
#include <string>
#include "pointst.h"
#include <queue>
#include <limits>
#include <math.h>
#include <map>


class Carte{
  struct Noeud;

  public:
    void ajouter_noeud(long id, const PointST& p);
    void ajouter_route(const std::string& nom, std::list<long>& noeuds);    
    void changerDisponibilites(std::list<std::string>& nomsroute, bool estdisponible);
    long trouverDestIdeale(long noeudorigine, double distancecible, double& distancetrouve, std::list<long>& trajet);
    void dijkstra_sortant(long);
    void dijkstra_entrant(long);

  private:
    /* Un noeud est un sommet dans le graphe. Il possède des coordonnées, un nom et des aretes entrantes et sortantes. */
    struct Noeud{
      PointST point;
      long id_noeud = -1 ;
      std::set<std::pair<long,std::string>> arete_sortantes;
      std::set<std::pair<long,std::string>> arete_entrantes;
    };

    /* Dictionnaire permettant de lier le nom du noeud à l'objet noeud */
    std::map<long, Noeud> noeuds;
    
    /* Structure permettant de stocker l'information concernant la disponibilité d'une route */
    std::map<std::string, bool> route_est_dispo;
    
    /* Structures liées à l'appel de la fonction Dijkstra sur les arêtes sortantes et entrantes */
    std::map<long,double> distances_sort;
    std::map<long,long> parents_sort;
    std::map<long,double> distances_ent;
    std::map<long,long> parents_ent;

};

/* Déclaration opérateur (fonction) global */
std::istream& operator >> (std::istream& is, Carte& carte);

#endif
