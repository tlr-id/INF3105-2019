/* UQAM / Département d'informatique
   INF3105 - Structures de données et algorithmes
   Squelette pour classe générique ArbreMap<K,V> pour le Lab8 et le TP2.

   AUTEUR(S):
    1) Nom + Code permanent du l'étudiant.e 1
    2) Nom + Code permanent du l'étudiant.e 2
*/

#if !defined(__ARBREMAP_H__)
#define __ARBREMAP_H__

#include "arbreavl.h"

template <class K, class V>
class ArbreMap
{
  private:
    class Entree{
        public :     
            Entree(const K& c):cle(c),valeur(){}
            K cle;
            V valeur;
            bool operator < (const Entree& e) const { return cle < e.cle; }
            bool operator == (const Entree& e) const { return cle == e.cle; }
    };

    ArbreAVL<Entree> entrees;
    // À compléter : voir la Section 8.11 (Arbres associatifs (Mapping Trees)) des notes de cours.


    //ArbreAVL<Entree> entrees;

  public:
    bool contient(const K&) const;

    void enlever(const K&);
    void vider();
    bool vide() const;

    const V& operator[] (const K&) const;
    V& operator[] (const K&);

    class Iterateur {
      public:
        Iterateur(const ArbreMap& a) : iter(a.entrees.debut()) {}
        Iterateur(typename ArbreAVL<Entree>::Iterateur i) : iter(i) {}
        operator bool() const{return iter.operator bool();};
        bool operator!() const;
        
        Iterateur& operator++(){++iter; return *this;}
        Iterateur operator++(int);

        const K& cle() const{return (*iter).cle;}
        const V& valeur() const{return (V&) (*iter).valeur;} // ?? constant ou non ??

      private:
        typename ArbreAVL<Entree>::Iterateur iter;
    }; 
    Iterateur debut(){return Iterateur(*this);}
    Iterateur fin(){return Iterateur(entrees,fin());}
    Iterateur rechercher(const K& cle){return Iterateur(entrees.rechercher(cle));}
    Iterateur rechercherEgalOuSuivant(const K& cle){
        return Iterateur(entrees.rechercherEgalOuSuivant(cle));
    }
    Iterateur rechercherEgalOuPrecedent(const K& cle){
        return Iterateur(entrees.rechercherEgalOuPrecedent(cle));
    }
};



template <class K, class V>
void ArbreMap<K,V>::vider(){
    // À compléter
    entrees.vider();
}

template <class K, class V>
bool ArbreMap<K,V>::vide() const{
    // À compléter
    return entrees.vide();
    // return true;
}

template <class K, class V>
void ArbreMap<K,V>::enlever(const K& c)
{
    // À compléter
}

template <class K, class V>
bool ArbreMap<K,V>::contient(const K& c) const
{
    Entree entree(c);
    return entrees.contient(entree);

    
    
    // À compléter
    //return false;
}

template <class K, class V>
const V& ArbreMap<K,V>::operator[] (const K& c)  const
{
    typename ArbreAVL<Entree>::Iterateur iter=entrees.rechercher(c);
    return entrees[iter].valeur;
}

template <class K, class V>
V& ArbreMap<K,V>::operator[] (const K& c) 
{
    typename ArbreAVL<Entree>::Iterateur iter=entrees.rechercher(Entree(c));
    if(!iter){
        entrees.inserer(Entree(c));
        iter = entrees.rechercher(c);
    }
    return entrees[iter].valeur;
}


#endif

