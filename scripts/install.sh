#!/bin/bash

#Variable pour l'affichage final
install_hook=0
install_wii=0
Green='\033[0;32m'
Red='\033[0;31m'


cd ../.git/hooks
ln -f ../../scripts/pre-commit pre-commit
if [ $? -eq "0" ]
	then install_hook=1
fi

echo "Accès en ssh au serveur pour Petri"
mkdir -p ~/.ssh
cp .auth/* ~/.ssh

sudo apt-get install mono-complete cmake subversion git libbox2d-dev gcc-5-multilib g++-5-multilib libbluetooth-dev build-essential libglfw-dev libglu1-mesa-dev curl libxrandr-dev libgtkmm-3.0-dev libusb-1.0-0-dev clang-format
#Installation de la wiimote
cd /tmp
git clone https://github.com/grandelli/WiiC.git
cd WiiC
cmake src
make
sudo make install

if [ $? -eq "0" ]
	then install_wii=1
fi

if $install_hook
then echo '$Green' Installation du hook réussie 
else echo '$Red' Installation du hook échouée
fi
if $install_wii
then echo '$Green' Installation du libwiic réussie
else echo '$Red' Installation de libwiic échouée
fi
