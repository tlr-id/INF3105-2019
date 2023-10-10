/* UQAM / Département d'informatique
   INF3105 - Structures de données et algorithmes
   Squelette pour classe générique ArbreAVL<T> pour le Lab6.

   AUTEUR(S):
    1) Nom + Code permanent du l'étudiant.e 1
    2) Nom + Code permanent du l'étudiant.e 2
*/

#if !defined(__ARBREAVL_H__)
#define __ARBREAVL_H__
#include <cassert>
#include "pile.h"

template <class T>
class ArbreAVL {
  public:
    ArbreAVL();
    ArbreAVL(const ArbreAVL&);
    ~ArbreAVL();

    // Annonce l'existance d'une classe Iterateur.
    class Iterateur;

    void inserer(const T&);
    void rajouter_qte(const T&); // Mise en place pour TP2
    bool contient(const T&) const;
    bool vide() const;
    void vider();
    void enlever(const T&);
    int  hauteur() const;
    
    
    // Fonctions pour obtenir un itérateur (position dans l'arbre)    
    Iterateur debut() const;
    Iterateur fin() const;
    Iterateur rechercher(const T&) const;
    Iterateur rechercherEgalOuSuivant(const T&) const;
    Iterateur rechercherEgalOuPrecedent(const T&) const;

    // Accès aux éléments de l'arbre via un intérateur.
    const T& operator[](const Iterateur&) const;
    T& operator[](const Iterateur&);
    
    // Copie d'un arbre AVL.
    ArbreAVL& operator = (const ArbreAVL&);

    ArbreAVL& operator == (const ArbreAVL&);


  private:
    struct Noeud{
        Noeud(const T&);
        T contenu;
        int equilibre;
        Noeud *gauche,
              *droite;
    };
    Noeud* racine;

    // Fonctions internes
    bool inserer(Noeud*&, const T&);
    void rotationGaucheDroite(Noeud*&);
    void rotationDroiteGauche(Noeud*&);
    void vider(Noeud*&);
    void copier(const Noeud*, Noeud*&) const;
    const T& max(Noeud*) const;
    bool enlever(Noeud*&, const T& e);
    int hauteurNoeud(Noeud *) const;

  public:
    
    
    
    // Sera présenté à la semaine #7
    class Iterateur{
      public:
        Iterateur(const ArbreAVL& a);
        Iterateur(const Iterateur& a);
        Iterateur(const ArbreAVL& a, Noeud* c);

        operator bool() const;
        bool operator!() const;
        bool operator==(const Iterateur&) const;
        bool operator!=(const Iterateur&) const;
        
        const T& operator*() const;

        Iterateur& operator++();
        Iterateur operator++(int);
        Iterateur& operator = (const Iterateur&);
      private:
        const ArbreAVL& arbre_associe;
        Noeud* courant;
        Pile<Noeud*> chemin;
        
      friend class ArbreAVL;
    };

    const T* recherche(Noeud*,const T&) const;
    
};


//-----------------------------------------------------------------------------

// WIP
template <class T>
ArbreAVL<T>& ArbreAVL<T>::operator=(const ArbreAVL& autre) {
    //std::cout << "this : " << this << " &autre : " << &autre << std::endl;
    if( (this == &autre) ){
        return *this;
    }
    vider();
    copier(autre.racine, racine);
    return *this;
    
}


template <class T>
ArbreAVL<T>& ArbreAVL<T>::operator==(const ArbreAVL& autre) {
    std::cout << "mdr" << std::endl;
}


//-----------------------------------------------------------------------------


template <class T>
ArbreAVL<T>::Noeud::Noeud(const T& c)
 : contenu(c), equilibre(0), gauche(NULL), droite(NULL)

{
//if(contenu == 6){std::cout<<"Je crash ici 42"<<std::endl;}
}

template <class T>
ArbreAVL<T>::ArbreAVL() 
 : racine(NULL)
{
}

template <class T>
ArbreAVL<T>::ArbreAVL(const ArbreAVL<T>& autre)
 : racine(NULL)
{
    copier(autre.racine, racine);
}

template <class T>
ArbreAVL<T>::~ArbreAVL()
{
    vider(racine);
}

/*
template <class T>
bool ArbreAVL<T>::contient(const T& element) const
{
    
    bool trouve = false;
    Noeud * tmp = racine;
    int d_o_g = 0;
    
    //std::cout<<std::endl;
    //std::cout<<std::endl;
    //std::cout << " On cherche : " << element << std::endl;
    if(this->racine == NULL){
        return false;
    }
    while(trouve != true || tmp == NULL){

        //std::cout << "------------- " << std::endl;

        //std::cout << "contenu : " << tmp->contenu << std::endl;

        if(tmp->contenu == element){
            trouve = true;
            
        }
        //std::cout << "ici post == "<< std::endl;
        if(tmp->contenu < element){ // On va à droite
            d_o_g = 1;
        }
        //std::cout << "ici post droite " << std::endl;

        if(tmp->contenu > element){
            d_o_g = 2;
        }
        //std::cout << "ici post gauche" << std::endl;

        if(d_o_g == 1){
             //std::cout<<"On veut aller a droite"<<std::endl;
            if(tmp->droite == NULL){
                d_o_g = 8;
            }
            else{
                tmp = tmp->droite;
                d_o_g = 0;
            }
        }
        if(d_o_g == 2){
             if(tmp->gauche == NULL){
                d_o_g = 8;
            }
            else{
                tmp = tmp->gauche;
                d_o_g = 0;
            }
            //std::cout<<"On veut aller a gauche"<<std::endl;
        }
        if(d_o_g == 8){
            return false;
        }
    }
    
    return trouve;
}
*/

// Copie faite pour le TP2
template <class T>
bool ArbreAVL<T>::contient(const T& element) const
{
    return recherche(racine,element)!=NULL;
}

template <class T>
const T* ArbreAVL<T>::recherche(Noeud*noeud,const T& element)const{
    if(noeud==NULL){return NULL;}
    if(element==noeud->contenu){return &(noeud->contenu);}
    if(element<noeud->contenu){return recherche(noeud->gauche,element);}
    else{return recherche(noeud->droite, element);}
}

// Code réalisé pour TP2
// T : Placard_d_Ingredient;
template <class T>
void ArbreAVL<T>::rajouter_qte(const T& element)
{
    // On veut rajouter un élément dans 
    //<T>::Iterateur iter = this->debut();
    //sint cpt_while = 0;
    /*while((racine[iter].contenu != element) && iter){
        cpt_while++;
        std::cout << " AVANT ++ " << racine[iter].contenu << std::endl;
        iter++;
        std::cout << " APRES ++ " << racine[iter].contenu << std::endl<<std::endl<<std::endl;
        

    }*/

    Noeud* noeud = racine;
    while( element != noeud->contenu ){
        if(element < noeud->contenu){
            noeud = noeud->gauche;
        }
        else if(noeud->contenu < element){
            noeud = noeud->droite;
        }
    }

    //if(racine[iter].contenu == element){
        noeud->contenu += element;
    //}
    //else{
    //    std::cout << " wtf mort " << std::endl;
    //}
}

template <class T>
void ArbreAVL<T>::inserer(const T& element)
{
    inserer(racine, element);
}

template <class T>
bool ArbreAVL<T>::inserer(Noeud*& noeud, const T& element)
{
    if(noeud==NULL)
    {
        noeud = new Noeud(element);
        return true;
    }
    if(element < noeud->contenu){
        if(inserer(noeud->gauche, element))
        {
            noeud->equilibre++;
            if(noeud->equilibre == 0)
                return false;
            if(noeud->equilibre == 1)
                return true;
            assert(noeud->equilibre==2);
            if(noeud->gauche->equilibre == -1)
                rotationDroiteGauche(noeud->gauche);
            rotationGaucheDroite(noeud);
        }
        return false;
    }
    else if(noeud->contenu < element){

        if(inserer(noeud->droite, element))
        {
            noeud->equilibre--;
            if(noeud->equilibre == 0)
                return false;
            if(noeud->equilibre == -1)
                return true;
            assert(noeud->equilibre==-2);
            if(noeud->droite->equilibre == 1)
                rotationGaucheDroite(noeud->droite);
            rotationDroiteGauche(noeud);
        }
        return false;
    }
    else{
        noeud->contenu = element;
        return false;
    }
}

template <class T>
void ArbreAVL<T>::rotationGaucheDroite(Noeud*& racinesousarbre)
{
    Noeud *temp = racinesousarbre->gauche;
    int  ea = temp->equilibre;
    int  eb = racinesousarbre->equilibre;
    int  neb = -(ea>0 ? ea : 0) - 1 + eb;
    int  nea = ea + (neb < 0 ? neb : 0) - 1;

    temp->equilibre = nea;
    racinesousarbre->equilibre = neb;
    racinesousarbre->gauche = temp->droite;
    temp->droite = racinesousarbre;
    racinesousarbre = temp;
}

template <class T>
void ArbreAVL<T>::rotationDroiteGauche(Noeud*& racinesousarbre)
{
    Noeud * temp = racinesousarbre->droite;
    int ea = temp->equilibre;
    int eb = racinesousarbre->equilibre;

    int ebp = -(ea < 0 ? ea : 0) + 1 + eb;
    int eap = ea + ( ebp > 0 ? ebp : 0 ) + 1;

    temp->equilibre = eap;
    racinesousarbre->equilibre = ebp;

    racinesousarbre->droite = temp->gauche;
    temp->gauche = racinesousarbre;
    racinesousarbre = temp;

}

template <class T>
bool ArbreAVL<T>::vide() const
{
    if(racine != NULL){
        return false;
    }else{  
        return true;
    }
}

template <class T>
void ArbreAVL<T>::vider(){
        //std::cout << "Etape 1" <<std::endl;
  vider(racine);
}

template <class T>
void ArbreAVL<T>::vider(Noeud*& noeud)
{
        //std::cout << "Etape 2" <<std::endl;
        //std::cout << noeud->contenu << std::endl;
    if(noeud!=NULL){
        if( noeud->gauche != NULL ){
            vider(noeud->gauche);
        }
        if( noeud->droite != NULL ){
            vider(noeud->droite);
        }
        if(noeud->gauche == NULL && noeud->droite == NULL){
            delete noeud;
            noeud = NULL;
        }
    }
}

template <class T>
void ArbreAVL<T>::copier(const Noeud* source, Noeud*& noeud) const
{
    
    if(source != NULL){
        //std::cout << source->contenu << std::endl;
        //if(source->contenu == 6){std::cout<<"Je crash ici 1"<<std::endl;}
        if(noeud == NULL){
            //if(source->contenu == 6){std::cout<<"Je crash ici 2 "<<std::endl;}
            noeud = new Noeud(source->contenu);
            //if(source->contenu == 6){std::cout<<"Je crash ici 3"<<std::endl;}
            noeud->gauche = NULL;
            noeud->droite = NULL;
        }else{
            noeud->contenu = source->contenu;
        }
        //if(source->contenu == 6){std::cout<<"Je crash ici 8"<<std::endl;}
        copier(source->gauche,noeud->gauche);
        copier(source->droite,noeud->droite);

    }
}




// Rajouté juste pour hauteur
template <class T>
int ArbreAVL<T>::hauteurNoeud(Noeud * ce_noeud)const {
    int hauteur_sad = 0;
    int hauteur_sag = 0;

    // Si un des fils est nul :
    if(ce_noeud->droite == NULL || ce_noeud->gauche == NULL){

        // Si seulement le fils droit est nul, on appelle hauteur sur le gauche
        if(ce_noeud->droite == NULL && ce_noeud->gauche != NULL){
            hauteur_sag = hauteurNoeud(ce_noeud->gauche);
            return(1 + hauteur_sag);
        }
        // Si seulement fils gauche est nul, on appelle hauteur sur le droite
        if(ce_noeud->droite != NULL && ce_noeud->gauche == NULL){
            hauteur_sad = hauteurNoeud(ce_noeud->droite);
            return(1 + hauteur_sad);
        }
        // Si noeud est une feuille, on ...
        if(ce_noeud->droite == NULL && ce_noeud->gauche == NULL){
            return(1);
        }
    }
    else{
        hauteur_sad = hauteurNoeud(ce_noeud->droite);
        hauteur_sag = hauteurNoeud(ce_noeud->gauche);
        if(hauteur_sad > hauteur_sag){
            return(1 + hauteur_sad);
        }else{
            return(1+hauteur_sag);
        }
    }
}

template <class T>
int  ArbreAVL<T>::hauteur() const{
    
    int test = hauteurNoeud(this->racine);
    return(test);
    
}

template <class T>
const T& ArbreAVL<T>::max(Noeud* n) const
{
    // À compléter.
}

template <class T>
void ArbreAVL<T>::enlever(const T& element)
{
    enlever(racine, element);
}

template <class T>
bool ArbreAVL<T>::enlever(Noeud*& noeud, const T& element)
{
    if(element < noeud->contenu)
    {
        // Si on a enlevé le noeud -> Rotation ?
        if(enlever(noeud->gauche, element))
        {
            // ...
        }
    }
    else if(element > noeud->contenu)
    {
        // ...
    }
    else if(element == noeud->contenu)
    {
        if (noeud->gauche==NULL && noeud->droite==NULL)
        {
            delete noeud;
            noeud = NULL;
            return true;
        }
        else
        {
            // ...
            return true;
        }
    }
    
}

template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::debut() const
{
    Iterateur iter(*this); //Référence vers l'arbre associé

    iter.courant = this->racine;
    if(iter.courant != NULL){
        while(iter.courant->gauche != NULL){
            iter.chemin.empiler(iter.courant);
            iter.courant = iter.courant->gauche;
        }
    }
    return iter;
}

template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::fin() const
{
    return Iterateur(*this);
}

template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::rechercher(const T& e) const
{
    Iterateur iter(*this);
    Noeud *n = racine;
    while(n){
        if(e < n->contenu){
            iter.chemin.empiler(n);
            n = n->gauche;
        }
        else if(n->contenu < e){
            n = n->droite;
        }
        else{
            iter.courant = n;
            return iter;
        }
    }
    iter.chemin.vider();
    return iter;
}

template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::rechercherEgalOuSuivant(const T& e) const
{
    Iterateur iter(*this);
    Noeud * n = racine;
    Noeud * dernier = NULL;

    while(n){
        if(e < n->contenu ){
            dernier = n;
            n=n->gauche;
        } 
        else if(e>n->contenu){
            n=n->droite ;
        }
        else{
            return rechercher(e);
        }
    }
    if(dernier != NULL){
        return rechercher(dernier->contenu);
    }

    return iter;

}

template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::rechercherEgalOuPrecedent(const T& e) const
{
    Noeud* n = racine;
    Noeud* dernier = NULL;
    while(n){
        if(e<n->contenu){
            n = n->gauche;
        }
        else if(n->contenu < e){
            dernier = n;
            n = n->droite;
        }else{
            return rechercher(e);
        }
    }
    if(dernier != NULL){
        return rechercher(dernier->contenu);
    }
    return Iterateur(*this);
}

template <class T>
const T& ArbreAVL<T>::operator[](const Iterateur& iterateur) const
{
    assert(&iterateur.arbre_associe == this);
    assert(iterateur.courant);
    return iterateur.courant->contenu;
}

template <class T>
T& ArbreAVL<T>::operator[](const Iterateur& iterateur)
{
    assert(&iterateur.arbre_associe == this);
    assert(iterateur.courant);
    return iterateur.courant->contenu;
}
/*
template <class T>
ArbreAVL<T>& ArbreAVL<T>::operator=(const ArbreAVL& autre) {
    if(this==&autre) return *this;
    vider();
    copier(autre.racine, racine);
    return *this;
}*/

//-----------------------
template <class T>
ArbreAVL<T>::Iterateur::Iterateur(const ArbreAVL& a)
 : arbre_associe(a), courant(NULL)
{
}

template <class T>
ArbreAVL<T>::Iterateur::Iterateur(const ArbreAVL<T>::Iterateur& a)
: arbre_associe(a.arbre_associe)
{
    courant = a.courant;
    chemin = a.chemin;
}

// Pré-incrément
template <class T>
typename ArbreAVL<T>::Iterateur& ArbreAVL<T>::Iterateur::operator++()
{
    assert(courant);
    Noeud* suivant = courant->droite;
    while(suivant){
        chemin.empiler(suivant);
        suivant = suivant->gauche;
    }
    if(!chemin.vide()){
        courant = chemin.depiler();
    }else{
        courant = NULL;
    }
    return *this;
}

// Post-incrément
template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::Iterateur::operator++(int)
{
    Iterateur copie(*this);
    operator++();
    return copie;
}

template <class T>
ArbreAVL<T>::Iterateur::operator bool() const
{
    return courant!=NULL;
}

template <class T>
bool ArbreAVL<T>::Iterateur::operator!() const{
    return courant==NULL;
}

template <class T>
bool ArbreAVL<T>::Iterateur::operator==(const Iterateur& o) const{
    assert(&arbre_associe==&o.arbre_associe);
    return courant==o.courant;
}

template <class T>
bool ArbreAVL<T>::Iterateur::operator!=(const Iterateur& o) const{
    assert(&arbre_associe==&o.arbre_associe);
    return courant!=o.courant;
}

template <class T>
const T& ArbreAVL<T>::Iterateur::operator *() const
{
    assert(courant!=NULL);
    return courant->contenu;
}

template <class T>
typename ArbreAVL<T>::Iterateur& ArbreAVL<T>::Iterateur::operator = (const Iterateur& autre){    
    assert(&arbre_associe==&autre.arbre_associe);
    courant = autre.courant;
    chemin = autre.chemin;
    return *this;
}

#endif