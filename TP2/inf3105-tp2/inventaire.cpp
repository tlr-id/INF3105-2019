/*
  INF3105 - Structures de données et algorithmes
  UQAM | Département d'informatique
  Automne 2019 | TP2 | inventaire.cpp
*/
#include "inventaire.h"

/**
 * Calcule le minimum entre deux valeurs.
 * 
 * On utilise une fonction inline car il s'agit d'une simple fonction que l'on doit effectuer le plus rapidement possible,
 * sans réquisition de contexte.
 * 
 * @param a  : Première valeur
 *        b  : Deuxième valeur
 * @return x : Valeur la plus petite entre a et b
*/
inline int min(int a, int b)
{
    if(a>=b){
        return b;
    }else{
        return a;
    } 
}  

/////////////////////////////////////
//
//      Placard_d_Ingredient
//
/////////////////////////////////////

/* Constructeur par défaut */
Placard_d_Ingredient::Placard_d_Ingredient(){
      quanti_stock = 0;
      expi_stock = Date();
}

/* Constructeur avec des arguments */
Placard_d_Ingredient::Placard_d_Ingredient(int qte, Date d){
    this->quanti_stock = qte;
    this->expi_stock = d;
}

/* Redéfinition opérateur < */
bool Placard_d_Ingredient::operator<(const Placard_d_Ingredient& autre) const{
    return ( (this->expi_stock < autre.expi_stock) );
}

/* Redéfinition opérateur == */
bool Placard_d_Ingredient::operator==(const Placard_d_Ingredient& autre) const{
    return( (this->expi_stock == autre.expi_stock) );
}

/* Redéfinition opérateur != */
bool Placard_d_Ingredient::operator!=(const Placard_d_Ingredient& autre) const{
    return( (this->expi_stock != autre.expi_stock) );
}

/* Redéfinition opérateur = */
Placard_d_Ingredient& Placard_d_Ingredient::operator=(const Placard_d_Ingredient& autre){
      this->quanti_stock = autre.quanti_stock;
      this->expi_stock = autre.expi_stock;
      return *this;
}

/* Redéfinition opérateur += */
Placard_d_Ingredient& Placard_d_Ingredient::operator+=(const Placard_d_Ingredient& autre){
    quanti_stock += autre.quanti_stock; 
    return *this;
}

/////////////////////////////////////
//
//          Ingredient
//
/////////////////////////////////////

/* Constructeur par défaut */
Ingredient::Ingredient(){
      quanti_matos = 0;
      nom_matos = "NULL";
      expiration_matos = Date();
}

/* Constructeur avec des arguments */
Ingredient::Ingredient(int nb, std::string nom, Date expi){
      quanti_matos = nb;
      nom_matos = nom;
      expiration_matos = expi;
}

/* Redéfinition opérateur < */
bool Ingredient::operator<(const Ingredient& autre) const{
    return ( (nom_matos < autre.nom_matos) );
}

/* Redéfinition opérateur == */
bool Ingredient::operator==(const Ingredient& autre){
    return(this->nom_matos == autre.nom_matos);
}

/* Redéfinition opérateur = */
Ingredient& Ingredient::operator=(const Ingredient& autre){
    this->quanti_matos = autre.quanti_matos;
    this->nom_matos = autre.nom_matos;
    this->expiration_matos = autre.expiration_matos;
    return *this;
}

/* Redéfinition opérateur >> */
std::istream& operator >> (std::istream& is, ListeIngredients& liste)
{ 
    std::string chaine;
    is >> chaine;
    while(is && chaine!="---"){
        int quantite;
        is >> quantite;
        if(quantite != 0){
            liste.arbre_ingr.inserer(Ingredient(quantite,chaine,Date()));
        }
        is >> chaine;
    }
    return is;
}

/////////////////////////////////////
//
//          Inventaire
//
/////////////////////////////////////

/* Redéfinition opérateur = */
Inventaire& Inventaire::operator=(const Inventaire& tmp){
    this->stockage = tmp.stockage;
    return *this;
}

/**
 * Vérifie la présence de suffisamment d'un ingrédient dans l'inventaire.
 * 
 * La présence d'un ingrédient dans l'inventaire est validée si on a assez de quantité consommable (non périmée).
 * On utilise un itérateur sur l'AVL de l'ingrédient en question, et on parcourt l'arbre tant que l'on a pas la quantité voulue.
 * Il est possible que la quantité d'ingrédient nécessaire ne se trouve pas dans un placard, mais au cumulé de plusieurs. 
 * Il faut donc prendre ceci en compte.
 * 
 * @param nom_ingredient_recherche : Nom de l'ingrédient que l'on recherche
 *        qte_a_chercher           : Quantité de cette ingrédient dont on a besoin
 *        date_d                   : Date de la réservation, afin de savoir si l'ingrédient est périmé ou pas
 * @return no_soucis               : Booléen traduisant ou-non la validité de la réservation
*/
bool Inventaire::verifier_ingredient_inventaire(std::string nom_ingredient_recherche, int qte_a_chercher, Date date_d){
    bool no_soucis = true;
    int qte_trouvee = 0;

    /* Iterateur qui pointe sur le début de l'AVL correspondant au placard de l'ingrédient que l'on récupère. */
    ArbreAVL<Placard_d_Ingredient>::Iterateur iter = stockage[nom_ingredient_recherche].debut();
     
    /* Tant qu'on a pas le bon nombre ou qu'on a pas de soucis, on parcourt l'ArbreAVL */
    while( iter ){
        Date expiration_tiroir = stockage[nom_ingredient_recherche][iter].expi_stock;
        int quantite_tiroir = stockage[nom_ingredient_recherche][iter].quanti_stock;

         /* Si la date est bonne alors on peut se servir dans le tiroir */
        if( date_d < expiration_tiroir ){
            /* Si on a moins de quantité que prévue */
            if( quantite_tiroir < qte_a_chercher ){
                int a_prendre = min(qte_a_chercher - qte_trouvee,quantite_tiroir);
                qte_trouvee = qte_trouvee + a_prendre;
                stockage[nom_ingredient_recherche][iter].quanti_stock -= a_prendre;
            }
            // Si plus que prévu on se sert 
            if( quantite_tiroir >= qte_a_chercher){
                int a_prendre = qte_a_chercher-qte_trouvee;
                qte_trouvee = qte_trouvee + a_prendre;
                stockage[nom_ingredient_recherche][iter].quanti_stock -= a_prendre;
            }

        }   
        /* Si on a les bonnes quantitées, on peut quitter le while */
        if(qte_a_chercher == qte_trouvee){
            return true;
        }else{
            // On avance dans l'arbre si on peut
            iter++;
        }    
    }
    if(qte_a_chercher == qte_trouvee){
        no_soucis = true;
    }
    else{
        no_soucis = false;
    }
    return no_soucis;
}    

/**
 * Réalise l'opération virtuelle de retirer des placards, c'est à dire d'enlever les ingrédients de la commande de l'inventaire.
 * 
 * On utilise un itérateur sur l'AVL de l'ingrédient en question, et on parcourt l'arbre tant que l'on a pas la quantité voulue.
 * Il est possible que la quantité d'ingrédient nécessaire ne se trouve pas dans un placard, mais au cumulé de plusieurs. 
 * Il faut donc prendre ceci en compte.
 * 
 * @param nom_ingredient_recherche : Nom de l'ingrédient que l'on recherche
 *        qte_a_chercher           : Quantité de cette ingrédient dont on a besoin
 *        date_d                   : Date de la réservation, afin de savoir si l'ingrédient est périmé ou pas
 * @return no_soucis               : Booléen traduisant ou-non la validité de la réservation
*/
bool Inventaire::aller_chercher_dans_les_placards(std::string nom_ingredient_recherche, int qte_a_chercher, Date date_d){
    bool no_soucis = true;
    int qte_trouvee = 0;

    /* Iterateur qui pointe sur le début de l'AVL correspondant au placard de l'ingrédient que l'on cherche. */
    ArbreAVL<Placard_d_Ingredient>::Iterateur iter = stockage[nom_ingredient_recherche].debut();

    /* Tant qu'on a pas le bon nombre ou qu'on a pas de soucis, on parcourt l'ArbreAVL */
    while( iter ){
        Date expiration_tiroir = stockage[nom_ingredient_recherche][iter].expi_stock;
        int quantite_tiroir = stockage[nom_ingredient_recherche][iter].quanti_stock;

         /* Si la date est bonne alors peut utiliser le tiroir */
        if( date_d < expiration_tiroir ){

            /* Si les quantités du tiroir ne sont pas suffisantes, on prend ce qu'on trouve */
            if( quantite_tiroir < qte_a_chercher ){
                int a_prendre = min(qte_a_chercher - qte_trouvee,quantite_tiroir);
                qte_trouvee = qte_trouvee + a_prendre;
                stockage[nom_ingredient_recherche][iter].quanti_stock -= a_prendre;
            }
            /* Si les quantités du tiroir sont suffisantes, on prend ce dont on a besoin */
            if( quantite_tiroir >= qte_a_chercher){
                int a_prendre = qte_a_chercher-qte_trouvee;
                qte_trouvee = qte_trouvee + a_prendre;
                stockage[nom_ingredient_recherche][iter].quanti_stock -= a_prendre;
            }
        }   
        /* Si on a les bonnes quantités, on peut quitter le while */
        if(qte_a_chercher == qte_trouvee){
            return true;
        }else{
            // On avance dans l'arbre si on peut
            iter++;
        }  
    }
    if(qte_a_chercher == qte_trouvee){
        no_soucis = true;
    }
    else{
        no_soucis = false;
    }
    return no_soucis;
}

/* Redéfinition opérateur >> */
std::istream& operator >> (std::istream& is, Inventaire& inventaire){
    std::string chaine;
    is >> chaine;

    while(is && chaine!="---"){
        int quantite;
        Date expiration;
        is >> quantite >> expiration;

        if(quantite!=0){
            Placard_d_Ingredient nv_aliment(quantite,expiration);

            if( inventaire.stockage[chaine].contient(nv_aliment) ){
                // Fonction mise en place pour tp2 dans avl.h
                inventaire.stockage[chaine].rajouter_qte(nv_aliment);
            }else{
                inventaire.stockage[chaine].inserer(nv_aliment);
            }    
        }
        is >> chaine;
    }
    return is;
}