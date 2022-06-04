#!/bin/bash
function usage () {
	echo "Source : https://www.admin-linux.fr/bash-de-la-couleur-dans-le-shell/"
	echo "Ce script permet de visualiser les codes couleurs et quelques effets de décoration utilisables dans bash"
	exit 10
}

	if [[ "$1" == "-h" ]] || [[ "$1" == "-help" ]] || [[ "$1" == "--help" ]]; then usage ; fi

# Texte de l'exemple ( >= 3 caractères ) :
TEXT=" Bash " ;

# Couleur du texte :
declare -a FG=('' '1' '4' '5' '7' '30' '31' '32' \
'33' '34' '35' '36' '37') ;

echo 

# Première ligne :
printf "FG \ BG\t%${#TEXT}s" ;
for bg in {40..47} ; do
	printf "%${#TEXT}s" "${bg}  " ;
done
echo ;

# Création du tableau de présentation des combinaisons :
for fg in ${!FG[*]} ; do
	echo -ne "${FG[fg]}\t\033[${FG[fg]}m$TEXT" ;
	for bg in {40..47} ; do
		echo -ne "\033[${FG[fg]};${bg}m$TEXT\033[0m" ;
	done
	echo ;
done

# Comment déclarer une couleur :
cat <<_eof_

FG = texte (foreground) \ BG = arrière-plan (background)

Comment déclarer une couleur :
------------------------------
 Format de déclaration : \\033[XXm où XX prend les valeurs 
       de FG ou BG" ;
 Retour aux paramètres par défaut : \033[0m" ;

Exemples :
	echo -e "\033[31mUn texte en rouge et\033[0m retour a la normale"
	echo -e "\033[34m\033[45mBleu sur fond rose\033[0m"
		peut aussi s'écrire :
	echo -e "\033[34;45mBleu sur fond rose\033[0m"
	echo -e "\033[34mbleu, \033[1mgras, \033[4msouligné, \033[5mclignotant \033[7met surligné\033[0m
 Pour plus de détails : http://www.admin-linux.fr/?p=9011

_eof_