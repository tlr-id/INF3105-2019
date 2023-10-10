#!/bin/bash
##########################################################################
# UQAM - Département d'informatique
# INF3105 - Structures de données et algorithmes
# Automne 2018
# TP1
# http://ericbeaudry.uqam.ca/INF3105/tp1/
# beaudry.eric@uqam.ca
#
# Script d'évaluation
#
# Instructions:
# 1. Déposer ce script avec les fichiers problèmes dans un répertoire 
#    distinct (ex: tests).
# 2. Se placer dans le répertoire contenant votre programme ou contenant
#    la liste des soumissions Oto (*.tp_oto).
# 3. Lancer ce script (ex: ../tests/evaluer.sh).
##########################################################################

# Obtenir le chemin du répertoire contenant le présent script et les fichiers tests
pushd `dirname $0`
repertoire_tests=`pwd`
popd

echo "UQAM | Département d'informatique"
echo "INF3105 | Structures de données et algorithmes"
echo "Évaluation du TP1"
echo

if [ `pwd` -ef $repertoire_tests ];
then
    echo "Ce script doit être dans un répertoire différent de celui contenant votre tp1."
    echo "Ce script a été arrêté afin de ne pas écraser les fichiers test[A-Z]+.txt!"
    exit -2;
fi

########### Détection du valideur de résultats #######
# Exécutable du programme de validation
valideur="${repertoire_tests}/valideur"
if [ -x "${valideur}-`uname`-`uname -p`" ]
then
    valideur="${valideur}-`uname`-`uname -p`"
else
    if [ -x "${valideur}-`uname`" ]
    then
        valideur="${valideur}-`uname`"
    fi
fi
######################################################


# Détection si l'utilitaire time sous Linux est disponible (peut afficher 0.00)
echo "Détection de /usr/bin/time..."
/usr/bin/time -f %U echo 2>&1 > /dev/null
souslinux=$?

# Fixer les limites : 120s, 2048Mo mémoire et 20Mo fichier
#ulimit -t 180 -v 2097152 -f 20480
#echo "ulimit (limites courantes des ressources) :"
#ulimit -t -v -f
#echo "-----"

# Détection du CPU
if [ -e /proc/cpuinfo ] ; then
    cpuinfo=`grep "model name" /proc/cpuinfo | sort -u | cut -d ":" -f 2`
else
    cpuinfo="?"
fi

function Nettoyer
{
    echo "Nettoyage"
    # Au cas où le Makefile des étudiants ne ferait pas un «make clean» correctement.
    #make clean

    rm -f *.o* *.gch tp[1-3] *-req?+.txt
    # Au besoin, nettoyer les précédents fichiers logs
    rm -f log*.txt
}

date=`date +%Y%m%d_%H%M%S`

## Activer + de tests pour le correction complète ...

requetes="carte00_1 carte00_2 carte00_2-100 carte00_2-50 carte00_2-10 carte00_2-1 carte00_3"
requetes="${requetes} carte01_1 carte01_2 carte01_2-1"
requetes="${requetes} carte02_1 carte02_2 carte02_2-500 carte02_2-10 carte02_3 carte02_3-100 carte02_4-200 carte02_5-200"
requetes="${requetes} carte03_1 carte03_2 carte03_2-500 carte03_2-10 carte03_3 carte03_3-100 carte03_4-200 carte03_5-200"
requetes="${requetes} carte04_1 carte04_2-500"
requetes="${requetes} carte05_1 carte05_2-500"
requetes="${requetes} carte06_1 carte06_2-5000 carte06_3-4000 carte06_4-3000 carte06_5-2000 carte06_7-2000 carte06_8-2000 carte06_9-2000 carte06_10-2000 carte06_11-2000 carte06_12-2000"
requetes="${requetes} carte07_1 carte07_2 carte07_2-10 carte07_3-2000 carte07_4-10 carte07_4-100"
requetes="${requetes} carte08_1 carte08_2-10 carte08_3-10 carte08_3-500 carte08_4-10 carte08_4-500 carte08_4-20000"

##### La fonction qui évalue un TP ######
function EvaluerTP
{
    ## Forcer la recompilation.
    #rm tp1 *.o
    #make clean   
    
    date2=`date +%Y%m%d_%H%M%S`
    logfile="log-${date2}.txt"
    echo "Les résultats seront déposés dans $logfile..."

    echo "Machine : " `hostname`
    echo "#Machine : " `hostname`  > $logfile
    echo "CPU :$cpuinfo" 
    echo "#CPU :$cpuinfo"  >> $logfile
    echo "Date début : ${date2}"
    echo "#Date début : ${date2}"  >> $logfile
    echo "Limite de `ulimit -t` secondes par test"
    echo "#Limite de `ulimit -t` secondes par test"  >> $logfile
    echo
    echo "#" >> $logfile

    # Pour statistiques : nombre de lignes de code...
    echo "Taille des fichiers source :" >> ${logfile}
    wc *.{c*,h*}  >> ${logfile}

    taille=`wc *.{c*,h*} | grep total`
    sommaire="$taille"
    
    make tp1
    
    # Vérification de l'existance du programme.
    if [ ! -e tp1 ]
	    then
	    echo "  Erreur : le fichier tp1 est inexistant!"
	    return
    fi
    if [ ! -x tp1 ]
	    then
	    echo "  Erreur : le fichier tp1 n'est pas exécutable!"
	    return
    fi


	echo 
	echo "#" >> $logfile

	echo -e "Carte    \tTaille\tArgs\tSuperficie\tOpt?\tCPU\tMém.(k)"
	echo -e "Carte    \tTaille\tArgs\tSuperficie\tOpt?\tCPU\tMém.(k)" >> $logfile

	for requete in $requetes;
	do
    	carte="${requete%_*}.txt"
    	requete="${requete#*_}"
    	args=`echo $requete | sed s/'-'/' '/g`
    
	    fichiercarte=$repertoire_tests/$carte
    	taillecarte=`cat $fichiercarte | wc -l`
    
	    fichierresultat="${carte%.txt}_${requete}+.txt"
    	fichiersolution="$repertoire_tests/${carte%.txt}_${requete}+.txt"
    
        {
            # Fixer les limites : 60 s, 512Mo mémoire et 20ko fichier
            #ulimit -t 60 -v 524288 -f 20

	        if [ $souslinux -eq 0 ]; then
        	    #echo "./tp1 $fichiercarte $args > $fichierresultat"
	            tcpu="`ulimit -t 80 -v 524288 -f 20 && (/usr/bin/time -f "%U\t%Mk" ./tp1 $fichiercarte $args > $fichierresultat) 2>&1 | tail -n 1`"
        	else
            	tcpu=`(ulimit -t 80 -v 524288 -f 20 && time -p ./tp1 $fichiercarte $args > $fichierresultat) 2>&1 | egrep user | cut -f 2 -d " "`
	        fi
	        
	    }
    
    	superficieresultat="?"
	    optimal="?"
    	if ( [ -x ${valideur} ] && [ -e $fichierresultat ] )
	    then
        	superficieresultat=`$valideur $fichiercarte $fichierresultat $args`
        	if( [ -e $fichiersolution ] )
	        then
    	        comptesolution=`$valideur $fichiercarte $fichiersolution $args`
        	    [ "$superficieresultat" != "$comptesolution" ];
            	optimal=$?
        	fi
    	else
        	superficieresultat=`head -n 1 $fichierresultat`
        	if( [ -e $fichiersolution ] )
        	then
            	diff -tibw $fichierresultat $fichiersolution 2>&1 > /dev/null
        		if [ $? -eq 0 ];
        		then
        	    	optimal="OK"
         		else
         	    	optimal="Echec"
        		fi
        	fi
    	fi

    	echo -e "$carte\t$taillecarte\t$requete\t$superficieresultat\t$optimal\t$tcpu"
    	echo -e "$carte\t$taillecarte\t$requete\t$superficieresultat\t$optimal\t$tcpu" >> $logfile
    	sommaire="${sommaire}\t${superficieresultat}\t${optimal}\t${tcpu}"
	done
}

# Lister les soumissions Oto (répertoires terminant par .tp_oto)
tps=`ls -1 | grep .tp_oto`
# Si aucun répertoire .tp_oto n'existe, essayer le répertoire courant (auto-évaluation)
if [ ! -n "$tps" ]; then
    tps="."
fi

# Génération de l'entête du rapport
date=`date +%Y%m%d_%H%M%S`
echo "#Rapport de correction INF3105 / TP1" > "rapport-${date}.txt"
echo -e "#Date:\t${date}" >> "rapport-${date}.txt"
echo -e "#Machine :\t" `hostname` >> "rapport-${date}.txt"
echo -e "#CPU :\t$cpuinfo" >> "rapport-${date}.txt"
echo >> "rapport-${date}.txt"

# Génération des titres des colonnes
echo -e -n "#\t" >> "rapport-${date}.txt"
for requete in ${requetes}; do
   echo -e -n "\t${requete}\t\t\t" >> "rapport-${date}.txt"
done
echo >> "rapport-${date}.txt"
echo -e -n "#Soumission\tTaille sources" >> "rapport-${date}.txt"
for requete in ${requetes}; do
   echo -e -n "\tSup\tOpt?\tCPU\tMem" >> "rapport-${date}.txt"
done
echo >> "rapport-${date}.txt"

# Itération sur chaque TP
for tp in $tps; do
    sommaire=""
    echo "## CORRECTION : $tp"
    pushd $tp
    	EvaluerTP
#       Nettoyer
    popd
    #echo -e ">> ${sommaire}"
    echo -e "${tp}\t${sommaire}" >> "rapport-${date}.txt"
done

