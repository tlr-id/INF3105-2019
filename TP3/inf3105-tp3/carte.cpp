/*  TP3
    carte.cpp
    LE ROUX Thomas
    LERT28049705
    19/12/19
*/

#include "carte.h"

/**
 * Ajoute un Noeud à la carte.
 * 
 * @param : id : Nom du noeud
 *          p  : Instance de PointST contenant les coordonnées géographiques du noeud
 */
void Carte::ajouter_noeud(long id, const PointST& p){
    noeuds[id].point = p;
    noeuds[id].id_noeud = id;
}


/**
 * Ajoute une route à la carte.
 * 
 * Une route est composée d'une liste de noeud qu'on relie entre eux par le biais d'arêtes/
 * 
 * @param : nomroute : Chaîne de caractère correspondant au nom de la route qu'on ajoute.
 *          noeuds   : Liste des noeuds qui composent la route.
 */
void Carte::ajouter_route(const std::string& nomroute, std::list<long>& noeuds){
    route_est_dispo[nomroute] = true;

    do{
        long n = noeuds.front();
        noeuds.pop_front();

        if(!noeuds.empty()){

            /* Ce noeud peut atteindre le noeud suivant dans la route*/
            this->noeuds[n].arete_sortantes.insert({noeuds.front(),nomroute});

            /* Le noeud suivant dans la route est atteint par ce noeud */
            this->noeuds[noeuds.front()].arete_entrantes.insert({n,nomroute});
        }
    }while(!noeuds.empty());
}

/**
 * Modifie la disponibilité d'une route
 * 
 * On utilise la structure de donnée route_est_dispo afin de savoir la disponibilité d'une route et de la modifier.
 * Revient à "activer ou désactiver" l'utilisation possible d'une route.
 * 
 * @param : nomsroutes    : liste des noms des routes dont on met la disponibilité à jour
 *          estdisponible : booléen indiquant si les routes deviennent disponibles ou innaccessibles
 */
void Carte::changerDisponibilites(std::list<std::string>& nomsroutes, bool estdisponible){
    
    /* On "active les routes */
    if(estdisponible){
        while(!nomsroutes.empty()){
            std::string nom = nomsroutes.front();
            nomsroutes.pop_front();
            route_est_dispo[nom]=true;
        }
    }

    /* on "désactive" les routes */
    else{
        while(!nomsroutes.empty()){
            std::string nom = nomsroutes.front();
            nomsroutes.pop_front();
            route_est_dispo[nom]=false;
        }
    }
}



/**
 * Fonction réalisant la recherche du noeud idéal ainsi que déterminant le parcours à réaliser.
 * 
 * On réalise ici une approche exploitant la caractéristique point-to-multipoint et multipoint-to-point de l'algorithme de Dijkstra. 
 * On réalise donc deux appels de Dijkstra sur le noeud de départ : 
 *      - Un sur les arêtes sortantes via la fonction dijkstra_sortant()
 *      - Un sur les arêtes entrantes via la fonction dijkstra_entrant()
 * 
 * @param  : noeudorigine   : La valeur associée au noeud d'où part la requête
 *           distancecible  : La valeur associée à la distance minimale requise
 *           distancetrouve : Référence vers la valeur de distance associé au chemin calculé
 *           trajet         : Référence vers une liste de noeud que l'on doit remplir avec les noeuds correspondant au chemin calculé
 * @return : noeud_cible    : On retourne le noeud où le chemin arrive avant de faire demi-tour
 */
long Carte::trouverDestIdeale(long noeudorigine, double distancecible, double& distancetrouve, std::list<long>& trajet){

    /* Appel aux 2 algorithmes de Dijkstra */
    dijkstra_entrant(noeudorigine);
    dijkstra_sortant(noeudorigine);

    /* On recherche le meilleur noeud pour la requête */
    double dist_tot = std::numeric_limits<double>::infinity();
    double dist_tmp;
    long noeud_cible = -1;
    std::map<long,double>::iterator iter_sort = distances_sort.begin();
    std::map<long,double>::iterator iter_ent = distances_ent.begin();
    for(;iter_sort!=distances_sort.end(),iter_ent!=distances_ent.end();iter_sort++,iter_ent++){
        dist_tmp = iter_sort->second + iter_ent->second;
        /* Si on a trouvé un noeud cible assurant une distance cible minimale tout en respectant les consignes, on le sauvegarde */
        if(dist_tmp >= distancecible && dist_tmp < dist_tot){
            dist_tot = dist_tmp;
            noeud_cible = iter_sort->first;
        }
    }

    /* Pour le cas particulier ou la requête ne possède pas de réponses */
    if(noeud_cible == -1){
        return -1;
    }


    /* Maintenant que l'on connait le noeud où l'on doit faire demi-tour, on stocke le trajet à réaliser pour notre application */

    /* Trajet aller */
    long noeud_chemin_aller = noeud_cible;
    trajet.push_front(noeud_cible);
    while(noeud_chemin_aller != noeudorigine){
        noeud_chemin_aller = parents_sort[noeud_chemin_aller];
        trajet.push_front(noeud_chemin_aller);
    }

    /* Trajet retour */
    long noeud_chemin_retour = noeud_cible;
    while(noeud_chemin_retour != noeudorigine){
        noeud_chemin_retour = parents_ent[noeud_chemin_retour];
        trajet.push_back(noeud_chemin_retour);
    }

    /* Sortie de la fonction */
    distancetrouve = round(dist_tot);
    return noeud_cible;
}

/**
 * Fonction implémentant l'algorithme de Dijkstra sur les arêtes sortantes 
 * 
 * L'algorithme suit celui donné dans les notes de cours
 * 
 * @param : noeudorigine : Noeud d'où on réalise l'algorithme de Dijkstra
 */
void Carte::dijkstra_sortant(long noeudorigine){

    /* Mise en place d'un monceau de type minimal */
    std::priority_queue< std::pair<double,long>, std::vector<std::pair<long,long>>, std::greater<std::pair<double,long>> > queue_prio;
    std::map<long, Noeud>::iterator iter = noeuds.begin();
    for(;iter != noeuds.end();iter++){
        distances_sort[iter->first] = std::numeric_limits<double>::infinity();
        parents_sort[iter->first] = -1;
        queue_prio.push( {distances_sort[iter->first],iter->first} );
    }
    distances_sort[noeudorigine] = 0;

    long noeud_etudie;     
    double d;

    while(!queue_prio.empty()){
        noeud_etudie = queue_prio.top().second;
        queue_prio.pop();
        if(distances_sort[noeud_etudie]==std::numeric_limits<double>::infinity()){}
        else{
            std::set<std::pair<long,std::string>>::iterator iter_arete = noeuds[noeud_etudie].arete_sortantes.begin();
            for(;iter_arete != noeuds[noeud_etudie].arete_sortantes.end();iter_arete++){

                /* Si les deux noeuds sont dans une route qui n'est pas disponible, on n'étudie pas le noeud */
                if(!route_est_dispo[iter_arete->second]){}
                else{
                    d = distances_sort[noeud_etudie] + noeuds[noeud_etudie].point.distance( noeuds[iter_arete->first].point );
                    if(d < distances_sort[iter_arete->first]){
                        parents_sort[iter_arete->first] = noeud_etudie;
                        distances_sort[iter_arete->first] = d;
                        queue_prio.push({d,iter_arete->first});
                    }
                }
            }
        }
    }
}

/**
 * Fonction implémentant l'algorithme de Dijkstra sur les arêtes entrantes 
 * 
 * @param : noeudorigine : Noeud d'où on réalise l'algorithme de Dijkstra
 */
void Carte::dijkstra_entrant(long noeudorigine){
    
    /* Mise en place d'un monceau de type minimal */
    std::priority_queue< std::pair<double,long>, std::vector<std::pair<double,long>>, std::greater<std::pair<double,long>> > queue_prio;
    std::map<long, Noeud>::iterator iter = noeuds.begin();
    for(;iter != noeuds.end();iter++){
        distances_ent[iter->first] = std::numeric_limits<double>::infinity();
        parents_ent[iter->first] = -1;
        queue_prio.push( {distances_ent[iter->first],iter->first} );
    }
    distances_ent[noeudorigine] = 0;

    long noeud_etudie; 
    double d;

    while(!queue_prio.empty()){       
        noeud_etudie = queue_prio.top().second;
        queue_prio.pop();

        if(distances_ent[noeud_etudie]==std::numeric_limits<double>::infinity()){}
        else{
            std::set<std::pair<long,std::string>>::iterator iter_arete = noeuds[noeud_etudie].arete_entrantes.begin();
            for(;iter_arete != noeuds[noeud_etudie].arete_entrantes.end();iter_arete++){

                /* Si les deux noeuds sont dans une route qui n'est pas disponible, on n'étudie pas le noeud */
                if(!route_est_dispo[iter_arete->second]){}
                else{
                    d = distances_ent[noeud_etudie] + noeuds[noeud_etudie].point.distance( noeuds[iter_arete->first].point );
                    if(d < distances_ent[iter_arete->first]){
                        parents_ent[iter_arete->first] = noeud_etudie;
                        distances_ent[iter_arete->first] = d;
                        queue_prio.push({d,iter_arete->first});
                    }
                }
            }
        }
    }
}