/*
  INF3105 - Structures de données et algorithmes
  UQAM / Département d'informatique
  Automne 2019
  Squelette de départ pour le TP2.
  
  Il n'est pas nécessaire de remettre ce fichier.
*/

#include "date.h"
#include <sstream>
#include <vector> // std::vecteur permis uniquement pour ce test

int main()
{
    std::cout << "Vérification de la classe Date" << std::endl;
    std::stringstream str("0j_00h00m 0j_00h01m 0j_00h02m 0j_00h40m 0j_00h59m 0j_01h00m 0j_01h01m 0j_01h02m 0j_02h00m 0j_02h01m 0j_02h03m 0j_02h10m 0j_02h12m 1j_00h00m 1j_01h01m 2j_00h00m");
    std::vector<Date> dates;
    while(!str.eof()){
        Date d;
        str >> d >> std::ws;
        std::cout << d << std::endl;
        if(str) dates.push_back(d);
    }
    
    // Test du comparateur
    for(unsigned int i=0;i<dates.size();i++)
        for(unsigned int j=0;j<dates.size();j++){
             if(i<j){
                 if(!(dates[i]<dates[j]))
                     std::cout << "Erreur : dates[" << i << "]<dates[" << j << "] : " << dates[i] << "<" << dates[j] << " retourne faux!" << std::endl;
             }else if(j<i){
                 if(!(dates[j]<dates[i]))
                     std::cout << "Erreur : dates[" << j << "]<dates[" << i << "] : " << dates[j] << "<" << dates[i] << " retourne faux!" << std::endl;
             }else{ // i==j
                 if(dates[j]<dates[i])
                     std::cout << "Erreur : dates[" << j << "]<dates[" << i << "] : " << dates[j] << "<" << dates[i] << " retourne faux!" << std::endl;
             }
        }
    std::cout << " --- FIN ---" << std::endl;
    return 0;
}

