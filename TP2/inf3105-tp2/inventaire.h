/*
  INF3105 - Structures de données et algorithmes
  UQAM | Département d'informatique
  Automne 2019 | TP2 | inventaire.h
*/

#if !defined(_INVENTAIRE__H_)
#define _INVENTAIRE__H_

#include <iostream>
#include "date.h"
#include "arbreavl.h"
#include "arbremap.h"
#include "pile.h"

class Inventaire;
class ListeIngredients;

class Placard_d_Ingredient{
  private :
    int quanti_stock;
    Date expi_stock;

  public :
    Placard_d_Ingredient();
    Placard_d_Ingredient(int, Date);

    bool operator<(const Placard_d_Ingredient&) const;
    bool operator==(const Placard_d_Ingredient&) const;
    bool operator!=(const Placard_d_Ingredient&) const;
    Placard_d_Ingredient& operator=(const Placard_d_Ingredient&);
    Placard_d_Ingredient& operator+=(const Placard_d_Ingredient&);
  friend Inventaire;
};

class Ingredient{
  private :
    int quanti_matos;
    std::string nom_matos;
    Date expiration_matos;

  public :
    Ingredient();
    Ingredient(int,std::string,Date);

    bool operator<(const Ingredient&) const;
    bool operator==(const Ingredient&);
    Ingredient& operator=(const Ingredient&);
  
    friend bool cuisiner(std::string quoi_recette,int combien,ArbreMap<std::string, ListeIngredients> recettes,Inventaire * inventaire, Date);
    friend bool verifier_stock(std::string quoi_recette,int combien,ArbreMap<std::string, ListeIngredients> recettes,Inventaire * inventaire, Date);
    friend class Inventaire;
};

class ListeIngredients{
  private:
    ArbreAVL<Ingredient> arbre_ingr;

  friend std::istream& operator >> (std::istream&, ListeIngredients&);
  friend bool cuisiner(std::string quoi_recette,int combien,ArbreMap<std::string, ListeIngredients> recettes,Inventaire * inventaire, Date);
  friend bool verifier_stock(std::string quoi_recette,int combien,ArbreMap<std::string, ListeIngredients> recettes,Inventaire * inventaire, Date);
  friend class Inventaire;
};



class Inventaire{
  private:
      ArbreMap< std::string,ArbreAVL<Placard_d_Ingredient> > stockage;
  public:
    Inventaire& operator=(const Inventaire&);
  
    bool aller_chercher_dans_les_placards(std::string,int,Date);
    bool verifier_ingredient_inventaire(std::string,int,Date);
  
  friend std::istream& operator >> (std::istream&, Inventaire&);
  friend bool cuisiner(std::string quoi_recette,int combien,ArbreMap<std::string, ListeIngredients> recettes,Inventaire * inventaire, Date);
};

#endif

