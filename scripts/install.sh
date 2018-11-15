#!/bin/bash

#Arguments
install_apt=0
install_hook=0
install_petri=0
install_wii=0
install_raspi=0

if [ $# -ne "0" ]
    then for arg in $*
    do
        if [ "$arg" = "all" ]
            then install_apt=1
            install_petri=1
            install_hook=1
            install_wii=1
            install_raspi=1
        elif [ "$arg" = "tools" ]
            then install_apt=1
        elif [ "$arg" = "petri" ]
            then install_petri=1
        elif [ "$arg" = "format" ]
            then install_hook=1
        elif [ "$arg" = "wii" ]
            then install_wii=1
        elif [ "$arg" = "raspi" ]
             then install_raspi=1
        else
            echo "Usage : $0 [all|tools|petri|format|wii|raspi]"
            exit
        fi
    done
else
    echo "Usage : $0 [all|tools|petri|format|wii|raspi]"
fi

#echo -e
Green='\033[0;42m'
Red='\033[0;41m'
Yellow='\033[0;43m'
End='\033[0;0m'

#Déplacement dans le dossier info/scripts
dir=$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)

#Tracking
hook_installed=0
wii_installed=0
petri_installed=0
raspi_installed=0

if [ $install_apt -eq 1 ]
    then echo -e "${Yellow}Installation des logiciels nécessaires${End}"
    sudo apt-get update
    sudo apt-get install mono-complete cmake subversion git libbox2d-dev gcc-5-multilib g++-5-multilib libbluetooth-dev build-essential libglfw-dev libglu1-mesa-dev curl libxrandr-dev libgtkmm-3.0-dev libusb-1.0-0-dev clang-format libirrlicht-dev
fi

if [ $install_hook -eq 1 ]
    then echo -e "${Yellow}Installation du pre-commit hook${End}"
    cd ${dir}/.git/hooks
    ln -f ../../scripts/pre-commit pre-commit

    if [ $? -eq "0" ]
        then echo -e "${Green}Installation du hook réussie${End}"
        hook_installed=1
        else echo -e "${Red}Installation du hook échouée${End}"
    fi
fi

if [ $install_petri -eq 1 ]
	then echo -e "${Yellow}Installation de PetriLab${End}"
	cd ${dir}/scripts/petrilab/
	./install.sh
    if [ $? -eq "0" ]
        then echo -e "${Green}Installation de petri réussie${End}"
        petri_installed=1
        else echo -e "${Red}Installation de petri échouée${End}"
    fi
fi

if [ $install_wii -eq 1 ]
    then echo -e "${Yellow}Installation de la wiimote${End}"
    sudo apt install libbluetooth-dev
    cd /tmp
    git clone https://github.com/grandelli/WiiC.git
    cd WiiC
    cmake src
    make
    sudo make install
	sudo ldconfig

    if [ $? -eq "0" ]
        then echo -e "${Green}Installation de libwiic réussie${End}"
        wii_installed=1
        else echo -e "${Red}Installation de libwiic échouée${End}"
    fi
fi

if [ $install_raspi -eq 1 ]
    then echo -e "${Yellow}Installation des outils ARM${End}"
    sudo apt-get install gcc-5-arm-linux-gnueabihf g++-5-arm-linux-gnueabihf build-essential git net-tools dnsmasq-base
    cd ${dir}
    sudo cp precompiled-libraries/libbluetooth.so.3 precompiled-libraries/libwiicpp.so precompiled-libraries/libwiic.so precompiled-libraries/libPetriRuntime.a /usr/arm-linux-gnueabihf/lib

    if [ $? -eq "0" ]
        then echo -e "${Green}Installation de la cross-compilation pour Raspberry réussie${End}"
        raspi_installed=1
        else echo -e "${Red}Installation de la cross-compilation pour Raspberry échouée${End}"
    fi
fi
