/*
  INF3105 - Structures de données et algorithmes
  UQAM | Département d'informatique
  Automne 2019 | TP2 | tp2.cpp
*/

#include <fstream>
#include "inventaire.h"
#include "arbremap.h"

bool cuisiner(std::string,int,ArbreMap<std::string, ListeIngredients>,Inventaire * ,Date);

bool verifier_stock(std::string,int,ArbreMap<std::string, ListeIngredients>,Inventaire *,Date);

void ajouter_a_stockage(ArbreMap<std::string,Inventaire>, Inventaire);


/**
 * Fonction principale du projet réalisant la lecture du fichier et les différentes opérations requises.
 * 
 * Le fichier est composé d'une liste de 3 différentes opérations :
 *      - recette :
 *          On ajoute une recette, ou on modifie une recette existante dans un ArbreMap.
 *          Une recette est composée d'une liste d'ingrédients accompagné de quantités. 
 * 
 *      - reception :
 *          On reçoit une livraison d'ingrédients. Il faut alors les stocker dans un inventaire.
 *          L'inventaire est ici traduit par la classe Inventaire, qui contient un ArbreMap d'ingrédient, menant à un ArbreAVL de quantité et de dates d'expirations
 *          Une réception est composée d'une liste d'ingrédients accompagné de quantités et de dates d'expiration de la nourriture
 * 
 *      - réservation :
 *          On reçoit une réservation à un instant t. Il faut alors vérifier la présence de suffisamment d'ingrédients commestibles dans l'inventaire.
 *          Lorsque l'on a vérifié la présence de tous les ingrédients de la recette dans l'inventaire, il faut alors aller récupérer les différents ingrédients dans l'inventaire.
 *          Une réservation est composée d'une date t et d'une liste de recette à préparer dans une certaine quantité.
 * 
 * @param  : entree  : Référence vers le fichier passé en entrée du programme
 * @return : booléen : Booléen traduisant le bon déroulement du programme
*/
int tp2(std::istream& entree){

    /* Structure ArbreMap contenant les différentes recettes.
    On utilise une string correspondant au nom de la recette en key, et une liste d'ingrédient en value.
    Une liste d'ingrédient est une classe composée d'un ArbreAVL d'ingrédients ainsi que de différents opérateurs. */
    ArbreMap<std::string, ListeIngredients> recettes;

    /* Structure contenant les ingrédients de notre restaurant. 
    Une instance de la classe Inventaire contient un ArbreMap avec comme clé le nom de l'ingrédient et comme value
    un ArbreAVL de Placard_d_Ingredient. Cet ArbreAVL contient les différentes quantités et les différentes dates d'expiration 
    pour chaque ingrédient.
    Placard_d_Ingredient est une classe rassemblant une date d'expiration, une quantité et différents opérateurs. */
    Inventaire inventaire;
    
    int nocommande = 0;

    while(entree){
         std::string commande;
         entree >> commande;
         if(!entree) break;
         
         /* Portion du code liée à la réception des recettes. On y lit les différents composés (ingrédients et quantités) de la recette.
         On la stocke ensuite dans l'ArbreMap des recettes. a*/
         if(commande=="recette"){
            std::string nomrecette;
            entree >> nomrecette; 
            ListeIngredients ingredients;
            entree >> ingredients;
            recettes[nomrecette] = ingredients;
         }


        /* Portion du code liée à la réception des ingrédients. On y lit les différents ingrédients et quantités reçues */
         else if(commande=="reception"){
            Date date_reception;
            entree >> date_reception;
            //date = date_reception; /* Inutilisé dans notre programme, mais utilisé pour traiter la partie G */
            entree >> inventaire;              
         }


        /* Portion du code liée à une réservation. Une réservation est divisée en plusieurs étapes :
                - Lecture complète des requêtes (nom + quantité de recettes à préparer).
                - Vérification de la présence des ingrédients (quantité + date) dans les placards du restaurant
                - Dans le cas où la vérification se passe bien, on peut passer à la cuisine
                - Dans le cas contraire, alors la recette est un échec
        */
         else if(commande=="reservation"){

            /* Structure contenant le nom et la quantité des recettes demandées par le client */ 
            ArbreMap<std::string,int> commandesPourCuisines;

            /* Booléen traduisant un éventuel soucis dans la réservation */
            bool pas_de_souci = true;

            Date date_preparation;
            entree >> date_preparation;
            //date = date_preparation;       /* Inutile ici */
            
             
            /* On prépare la liste des recettes */
            std::string nomrecette;
            entree >> nomrecette;
            /* Copie de l'inventaire réalisée pour vérifier les placards (cette copie permet de modifier sans risquer de perdre des informations)*/
            Inventaire inventaire_tmp = inventaire;

            while(entree && nomrecette!="---"){
                int quantite=0;
                entree >> quantite;
                commandesPourCuisines[nomrecette] = quantite;
                    
                /* On vérifie que les stocks sont compatibles avec chaque recette */
                if(pas_de_souci){
                    pas_de_souci = verifier_stock(nomrecette,quantite,recettes,&inventaire_tmp,date_preparation);
                }
                entree >> nomrecette;
            }

             /* Si on a les ingrédients suffisants pour réaliser la commande, alors on peut passer à la cuisine */
             if(pas_de_souci){
                
                /* De manière similaire, on parcourt les commandes, mais cette fois pour cuisiner */
                for(ArbreMap<std::string,int>::Iterateur iter = commandesPourCuisines.debut();iter;++iter){
                    pas_de_souci = cuisiner(iter.cle(),iter.valeur(),recettes,&inventaire,date_preparation);
                }
             }else{
                pas_de_souci = false;
             }

             if(!pas_de_souci){
                std::cout << nocommande << " : Echec" << std::endl;
             }
             else{
                std::cout << nocommande << " : OK" << std::endl;
             }
             nocommande++;
         }else{
             std::cout << "Commande '" << commande << "' inconnue!" << std::endl;
             return 2;
         }
    }
    return 0;
}

/**
 * Va vérifier pour une recette donnée la présence suffisante des stocks dans l'inventaire.
 * 
 * On va parcourir les différents ingrédients de la recette, et chercher s'ils sont présents en quantité suffisante.
 * 
 * @param : recette_actuelle   : Nom de la recette que l'on souhaite étudier
 *          combien            : Nombre de fois que la recette a besoin d'être réalisée
 *          livre_recettes     : L'ArbreMap contenant les recettes
 *          inventaire         : L'Inventaire contenant les ingrédients 
 *          date_d             : La date de la réservation
 * @return : stocks_suffisants : Booléen traduisant si oui ou non les stocks sont suffisants pour valider la réservation
*/
bool verifier_stock(std::string recette_actuelle,int combien, ArbreMap<std::string, ListeIngredients> livre_recettes,Inventaire *inventaire, Date date_d){

    /* On crée une liste des ingrédients à vérifier sur laquelle on va itérer par la suite */
    ListeIngredients liste_a_verifier = livre_recettes[recette_actuelle];
    bool stocks_suffisants = true;    
    
    /* On parcourt l'arbre des ingrédients de la recette que l'on étudie */
    ArbreAVL<Ingredient>::Iterateur iter = liste_a_verifier.arbre_ingr.debut();
    while(iter && stocks_suffisants){
        stocks_suffisants = inventaire->verifier_ingredient_inventaire(liste_a_verifier.arbre_ingr[iter].nom_matos ,combien*liste_a_verifier.arbre_ingr[iter].quanti_matos,date_d);
        iter++;
    }
    return stocks_suffisants;
}

/**
 * Va traduire l'opération de cuisine, c'est à dire retirer définitivement les ingrédients de l'inventaire.
 * 
 * On va parcourir les différents ingrédients de la recette et enlever les quantités nécessaires à la préparation de celle-ci
 * 
 * @param : recette_actuelle   : Nom de la recette que l'on souhaite étudier
 *          combien            : Nombre de fois que la recette a besoin d'être réalisée
 *          livre_recettes     : L'ArbreMap contenant les recettes
 *          inventaire         : L'Inventaire contenant les ingrédients 
 *          date_d             : La date de la réservation
 * @return : cuisinable        : Booléen traduisant si oui ou non les stocks ont été correctement trouvés et retirés
*/
bool cuisiner(std::string recette_actuelle,int combien, ArbreMap<std::string, ListeIngredients> liste_recettes,Inventaire * inventaire, Date date_d){

    /* On crée une liste des ingrédients à récupérer sur laquelle on va itérer par la suite */
    ListeIngredients liste_a_recuperer = liste_recettes[recette_actuelle];
    bool cuisinable = true;   

    /* On va chercher les ingrédients de la recette les uns après les autres */
    ArbreAVL<Ingredient>::Iterateur iter = liste_a_recuperer.arbre_ingr.debut() ;
    while(iter && cuisinable){
        cuisinable = inventaire->aller_chercher_dans_les_placards(liste_a_recuperer.arbre_ingr[iter].nom_matos , combien * liste_a_recuperer.arbre_ingr[iter].quanti_matos,date_d);
        ++iter;
    }
    return cuisinable;
}



/**
 * Fonction main() du projet
 * 
 * Réalise "l'interfaçage" entre l'entrée et la fonction tp2() qui est le coeur du projet.
 * Si on passe un fichier en entrée, on lit dans ce fichier, sinon on lit l'entrée standard
 * 
 * @param argc     : Nombre d'arguments passés en paramètres
 *        argv     : Valeur des arguments passés en paramètres
 * @return booléen : Appel à la fonction tp2()
*/
int main(int argc, const char** argv)
{
    if(argc>1){
         std::ifstream entree_fichier(argv[1]);
         if(entree_fichier.fail()){
             std::cerr << "Erreur d'ouverture du fichier '" << argv[1] << "'" << std::endl;
             return 1;
         }
         return tp2(entree_fichier);
    }else
         return tp2(std::cin);
}

