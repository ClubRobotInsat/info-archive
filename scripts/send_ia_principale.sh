#!/bin/bash

dir=$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)
cd ${dir}

# echo -e
Green='\033[0;42m'
Red='\033[0;41m'
Yellow='\033[0;43m'
End='\033[0;0m'

echo ""
echo -e "${Yellow}Compilation du réseau Petri${End}"
mkdir -p build_arm/src/robot/Principal
# -u pour update : génère, compile et déploi la librairie dynamique
petrilab -uv --profile ARM\ release src/robot/Principal/petri/IA2018.petri

if [ $? -eq "0" ]
	then echo -e "${Green}Compilation réussie${End}"
	else
		echo -e "${Red}Compilation échouée${End}"
		exit 1
fi

echo ""
cd scripts
./send_file_to_rpi.sh ../build_arm/src/robot/Principal/IA2018.so
./send_file_to_rpi.sh ../build_arm/src/robot/Principal/IAPrincipal
# TODO : mettre à jour cette ligne pour envoyer la version ARM de petriRuntime
./send_file_to_rpi.sh /usr/lib/libPetriRuntime.so
