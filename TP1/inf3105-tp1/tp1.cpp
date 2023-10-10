/*
  INF3105 -- Structures de données et algorithmes
  UQAM | Département d'informatique
  Automne 2019 | TP1 | tp1.cpp
*/

    /////////////////////////////////////////////////////////////
    //
    /*                       Includes                          */
    //
    /////////////////////////////////////////////////////////////
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <limits>
#include "polygone.h"
#include "tableau.h"
#include "vecteur.h"
#include "stdio.h"
#include <string.h>



    /////////////////////////////////////////////////////////////
    //
    /*                 Namespace & Prototypes                   */
    //
    /////////////////////////////////////////////////////////////


using namespace std;

int calculCombinaison(Tableau <Polygone>, int, int, double, double);

int trouverAireMax(Tableau <Polygone>);

int distance2Polygones(Tableau <Polygone>, int, int, int, int);

int distancePointSegment(Point,Point,Point);


    /////////////////////////////////////////////////////////////
    //
    /*                          MAIN                           */
    //
    /////////////////////////////////////////////////////////////
    

int main(int argc, const char** argv){

    /* Tableau de polygones sur lequel on va travailler */
    Tableau <Polygone> tab_poly;


    /////////////////////////////////////////////////////////////
    //
    /*                     MAIN - Partie I                     */
    //
    /////////////////////////////////////////////////////////////


    /* Affichage d'une erreur si le nombre d'arguments n'est pas bon */
    if(argc<3){
        cout << "./tp1 carte.txt nbRegions [distMax]" << endl;
        return 1;
    }
    
    /* Lecture des arguments */
    const char* fichiercarte = argv[1];
    int nbRegions = atoi(argv[2]);
    double DM = argc>=4 ? atof(argv[3]) : numeric_limits<double>::infinity();
    assert(DM>=0);
    
    /* Vérification de l'existence et ouverture du fichier passé en paramètre */
    ifstream in(fichiercarte);
    if(in.fail()){
        cout << "Imposible de lire le fichier " << fichiercarte << endl;
        return 2;
    }
    
    /* On parcourt le fichier ligne par ligne en créant un polynome par ligne */
    while(!in.eof()){
        Polygone polygone;
        in >> polygone >> std::ws;
        tab_poly.ajouter(polygone);  
    }

    /* Gestion de l'ordre du polynome - Utile pour l'affichage */
    /* Gestion de l'aire pour optimiser les calculs */
    for(int i = 0 ; i < tab_poly.taille() ; i++ ){
        tab_poly[i].setOrdre(i);
        tab_poly[i].setAire(tab_poly[i].calculerAire());
    }


    /////////////////////////////////////////////////////////////
    //
    /*                    MAIN - Partie II                     */
    //
    /////////////////////////////////////////////////////////////


    /* Selon le nombre de regions nbRegions passé en paramètre, on va exécuter certaines portions du code */
    switch(nbRegions){

        /* Le cas 1 est relativement simple : On cherche la region avec la plus grande aire */
        case 1:
        {
            int reg_aire_max = trouverAireMax(tab_poly);
            cout << round(tab_poly[reg_aire_max].getAire()) << endl;
            cout << tab_poly[reg_aire_max].getName() << endl;
            break;
        }

        /* Le cas 2 est beaucoup plus complexe : On va parcourir les différentes régions à la recherche de la meilleure combinaison de deux régions */
        case 2:
        {        
            /* Variable utilisées pour cette section */
            char nom_reg1[50];
            char nom_reg2[50];
            int bonne_reg1=0;
            int bonne_reg2=0;
            double aire1_tmp=0;
            double aire2_tmp=0;
            double aire_cumul= 0;
            int meilleure_combi;

            /* On va parcourir le tableau des régions en recherchant toute les combinaisons de régions possibles. */
            for(int iterator = 0 ; iterator < tab_poly.taille() ; iterator++ ){
                /* Ici, on s'arrête à iterator pour ne pas surcharger le programme. En effet, il ne sert à rien de rechercher plusieurs fois la même combinaison */
                for(int iterator2 = 0; iterator2 < iterator ; iterator2++ ){
                    if(iterator != iterator2){

                        /* On réalise le calcul de la combinaison. Si celle-ci permet d'obtenir une aire cumulée plus grande tout en respectant la condition de distance,
                        alors on va stocker les informations de celle-ci dans des variables temporaires.*/
                        meilleure_combi = calculCombinaison(tab_poly,iterator,iterator2,aire_cumul,DM);

                        if(meilleure_combi == 1){
                            aire1_tmp=tab_poly[iterator].getAire();
                            aire2_tmp=tab_poly[iterator2].getAire();
                            aire_cumul = aire1_tmp + aire2_tmp;
                            strcpy(nom_reg1,tab_poly[iterator].getName());
                            strcpy(nom_reg2,tab_poly[iterator2].getName());
                            bonne_reg1 = tab_poly[iterator].getOrdre();
                            bonne_reg2 = tab_poly[iterator2].getOrdre();
                        }
                    } 
                }
            }
            
            /* On réalise maintenant l'affichage de la sortie. */
            int reg_aire_max = trouverAireMax(tab_poly);

            /* Cas particulier d'une région très grande et de beaucoup de petites */
            if(aire_cumul < tab_poly[reg_aire_max].getAire() ){
                cout << round(tab_poly[reg_aire_max].getAire()) << endl;
                cout << tab_poly[reg_aire_max].getName() << endl;
            }

            /* Si on n'est pas dans ce cas-la */
            else{

                /* Cas particulier rencontré dans un test */
                if(aire_cumul == 0){
                    int reg_aire_max = trouverAireMax(tab_poly);
                    cout << round(tab_poly[reg_aire_max].getAire()) << endl;
                    cout << tab_poly[reg_aire_max].getName() << endl;
                }

                /* Cas général : On doit afficher dans l'ordre d'apparition dans le fichier .txt */
                else{
                    cout << round(aire_cumul) << endl;  
                    if(bonne_reg1 > bonne_reg2){
                        cout << nom_reg2 << endl;
                        cout << nom_reg1 << endl;
                    }else{
                        cout << nom_reg1 << endl;
                        cout << nom_reg2 << endl;
                    }
                }
            }
            break;
        }

        /* Le cas regroupant ceux supérieurs à 2. Ce cas à été étudié mais n'a pas abouti */
        default:
            cout << "Ce programme ne supporte pas 3 régions ou plus!" << endl;
            break;
    }

    return 0;
}

/**
 * Réalise l'étude de la combinaison de deux polygones.
 *
 * A partir de cette fonction va être appelée la distance minimale de ces deux polygones mais également le calcul de leur aire.
 *
 * @param tab_poly   : Tableau de polygones sur lequel on travail
 *        interator  : Position du premier polygone dans le tableau
 *        iterator2  : Position du deuxième polygone dans le tableau
 *        aire_cumul : Plus grande aire trouvée qui remplit les conditions
 *        DM         : Distance Maximale passée en paramètres
 * @return : booléen disant si l'on a trouvé ou non une meilleure solution que la précédente 
 */
int calculCombinaison(Tableau <Polygone> tab_poly, int iterator, int iterator2, double aire_cumul, double DM){

    /* Variable instanciées pour cette fonction */
    int dist_tmp;
    int nb_pts_poly1 = tab_poly[iterator].getTaille();
    int nb_pts_poly2 = tab_poly[iterator2].getTaille();
    double aire1_tmp;
    double aire2_tmp;
    double aire_cumul_tmp;

    /* Double boucle correspondant à l'étude pour chaque point des deux polygones. */
    for(int iter_poly1 = 0; iter_poly1 < nb_pts_poly1 ; iter_poly1++){
        for(int iter_poly2 = 0; iter_poly2 < nb_pts_poly2 ; iter_poly2++){
            if( iter_poly1 != iter_poly2 ){

                /* On appelle la fonction de distance pour les deux points étudiés */
                dist_tmp = distance2Polygones(tab_poly,iter_poly1,iter_poly2,iterator,iterator2);

                /* Calcul de l'aire cumulée et comparaison avec les résultats déjà obtenus */
                aire1_tmp=tab_poly[iterator].getAire();
                aire2_tmp=tab_poly[iterator2].getAire();
                aire_cumul_tmp = aire1_tmp + aire2_tmp;
                if( dist_tmp <= DM && aire_cumul_tmp > aire_cumul ){ // Dans ce cas, on a un nouvel empire
                    return 1;
                }
            }
        }
    }
    return 0;
}

/**
 * Va calculer la distance minimale entre 2 côtés.
 *
 * Pour chaque point passé en paramètre, on va créer un segment entre celui-ci et le point suivant dans le polygone. On va ensuite chercher la distance minimale 
 * entre ces deux segments en comparant chaque point instancié avec le "segment opposé".
 * On utilise l'algorithme décrit dans le sujet.
 *
 * @param tab_poly  : Tableau de polygones sur lequel on travail
 *        i_pol1    : Numéro du point du polygone 1 que l'on va étudier
 *        i_pol2    : Numéro du point du polygone 2 que l'on va étudier
 *        num_poly1 : Numéro du polygone 1 dans le tableau des polygones
 *        num_poly2 : Numéro du polygone 2 dans le tableau des polygones
 * @return dist_min : Distance minimale entre les deux segments générés à partir des points passés en paramètre.
 */

int distance2Polygones(Tableau <Polygone> tab_poly,int i_pol1, int i_pol2, int num_poly1, int num_poly2){
        
        /* Variables instanciées pour cette fonction */
        int dist_tmp;
        int dist_min = 99999;
        Point A;
        Point B;
        Point C;
        Point D;

        /* Premier segment : AB */
        A = tab_poly[num_poly1].tab_points[i_pol1];
        if(i_pol1 == tab_poly[num_poly1].getTaille() -1 ){
            B = tab_poly[num_poly1].tab_points[0];
        }else{
            B = tab_poly[num_poly1].tab_points[i_pol1+1];
        }

        /* Deuxième segment : CD */
        C = tab_poly[num_poly2].tab_points[i_pol2];
        if(i_pol2 == tab_poly[num_poly2].getTaille() -1){
           D = tab_poly[num_poly2].tab_points[0];
        }else{
           D = tab_poly[num_poly2].tab_points[i_pol2+1];
        }

        /* Calcul des distances pour chaque point entre celui-ci et le segment opposé */
        dist_tmp = distancePointSegment(A,C,D);
        if(dist_tmp < dist_min){dist_min = dist_tmp;}
        dist_tmp = distancePointSegment(B,C,D); 
        if(dist_tmp < dist_min){dist_min = dist_tmp;}
        dist_tmp = distancePointSegment(C,A,B);
        if(dist_tmp < dist_min){dist_min = dist_tmp;}
        dist_tmp = distancePointSegment(D,A,B);
        if(dist_tmp < dist_min){dist_min = dist_tmp;}
    return dist_min;
}

/**
 * Calcule la distance d'un point par rapport à un segment.
 *
 * Cette fonction suit l'algorithme donné par le sujet.
 *
 * @param X : Point dont on va calculer la distance par rapport au segment
 *        Y : Point correspondant au bord du segment
 *        Z : Point correspondant au bord du segment
 * @return  : La distance minimale entre le point X et le segment Y.
 */
int distancePointSegment(Point X, Point Y, Point Z){
    Vecteur XY = Vecteur(X,Y); 
    Vecteur YZ = Vecteur(Z,Y); 
    double ratio = ( XY.prod_scal(YZ) / YZ.prod_scal(YZ) );
    if( XY.prod_scal(YZ) < 0){
    }
    if(ratio>1){ratio = 1;}
    if(ratio<0){ratio = 0;}
    Point V(ratio * YZ.x , ratio * YZ.y);
    Point P = Y + V;
    return ( fabs( P.distance(X) ) );
}

/**
 * Cherche le polygone ayant l'aire maximale.
 *
 * @param     tab_poly  : Tableau de polygones sur lequel on travail
 * @return ind_max_tmp  : Indice correspondant au polynome ayant l'aire maximale du tableau de polygones.
 */
int trouverAireMax(Tableau <Polygone> tab){
    int max_tmp = 0;
    int ind_max_tmp = 0;
    for(int i = 0; i<tab.taille();i++){
        if(tab[i].getAire() > max_tmp){
            max_tmp = tab[i].getAire();
            ind_max_tmp = i;
        }
    }
    return ind_max_tmp;
}


