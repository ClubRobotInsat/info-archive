#!/bin/bash

#Arguments
install_apt=0
install_hook=0
install_petri=0
install_wii=0
install_raspi=0
install_opencv=0

usage="Usage : $0 [all|tools|petri|format|wii|raspi|opencv]"

if [ $# -ne "0" ]
    then for arg in $*
    do
        if [ "$arg" = "all" ]
            then install_apt=1
            install_petri=1
            install_hook=1
            install_wii=1
            install_raspi=1
            install_opencv=1
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
	elif [ "$arg" = "opencv" ]
             then install_opencv=1
        else
            echo $usage
            exit
        fi
    done
else
    echo $usage
fi

#echo -e
Green='\033[0;42m'
Red='\033[0;41m'
Yellow='\033[0;43m'
End='\033[0;0m'

#Déplacement dans le dossier info/scripts
dir=$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)
VERSION=$(lsb_release -sd | cut -d' ' -f2)
VERSION="${VERSION:0:2}"

#Tracking
hook_installed=0
wii_installed=0
petri_installed=0
raspi_installed=0

if [ $install_apt -eq 1 ]
    then echo -e "${Yellow}Installation des logiciels nécessaires${End}"

    if [ "$VERSION" -lt "18" ]; then
        echo -e "${Yellow}Installing gcc-7 suite from the ppa:jonathonf/gcc-7.1${End}"
        sudo add-apt-repository --yes ppa:jonathonf/gcc-7.1
    fi

    sudo apt update
    sudo apt install --yes cmake

    # Installation de CMake 3.9.3 au minimum
    CMAKE_VERSION=$(cmake --version | head -n1 | cut -d' ' -f3)
    MIN_CMAKE_VERSION="3.9.2"
    if [ ! "$CMAKE_VERSION" = "$(echo -e "$CMAKE_VERSION\n$MIN_CMAKE_VERSION" | sort -V | tail -1)" ]; then
        echo -e "${Yellow}Installing CMake 3.9.3${End}"
        sudo apt purge cmake
        version=3.9
        build=3
        cd /tmp
        wget https://cmake.org/files/v$version/cmake-$version.$build.tar.gz
        tar -xzvf cmake-$version.$build.tar.gz
        cd cmake-$version.$build/
        ./bootstrap
        make -j $(nproc)
        sudo make install
    fi
    sudo apt-get install --yes git libbox2d-dev gcc-7 g++-7 libbluetooth-dev build-essential libglu1-mesa-dev curl libgtkmm-3.0-dev libusb-1.0-0-dev clang-format libirrlicht-dev # libglfw-dev
    if [ "$VERSION" -lt "18" ]; then
        sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-7 70 --slave /usr/bin/g++ g++ /usr/bin/g++-7
    fi

    sudo usermod -a -G dialout ${USER}
fi

if [ $install_hook -eq 1 ]; then
    echo -e "${Yellow}Installation du pre-commit hook${End}"
    cd ${dir}/.git/hooks
    ln -f ../../scripts/pre-commit pre-commit

    if [ $? -eq "0" ]; then
        echo -e "${Green}Installation du hook réussie${End}"
        hook_installed=1
    else
        echo -e "${Red}Installation du hook échouée${End}"
    fi
fi

if [ $install_petri -eq 1 ]; then
    echo -e "${Yellow}Installation de PetriLab${End}"
    cd ${dir}/scripts/petrilab/
    ./install.sh
    if [ $? -eq "0" ]; then
        echo -e "${Green}Installation de petri réussie${End}"
        petri_installed=1
    else
        echo -e "${Red}Installation de petri échouée${End}"
    fi
fi

if [ $install_wii -eq 1 ]; then
    echo -e "${Yellow}Installation de la wiimote${End}"
    sudo apt install libbluetooth-dev
    cd /tmp
    git clone https://github.com/grandelli/WiiC.git
    cd WiiC
    cmake src
    make
    sudo make install
    sudo ldconfig

    if [ $? -eq "0" ]; then
        echo -e "${Green}Installation de libwiic réussie${End}"
        wii_installed=1
    else
        echo -e "${Red}Installation de libwiic échouée${End}"
    fi
fi

if [ $install_raspi -eq 1 ]; then
    echo -e "${Yellow}Installation des outils ARM${End}"
    sudo apt-get install --yes gcc-7-arm-linux-gnueabihf g++-7-arm-linux-gnueabihf build-essential git net-tools dnsmasq-base
    cd ${dir}
    sudo cp precompiled-libraries/libbluetooth.so.3 /usr/arm-linux-gnueabihf/lib

    if [ $? -eq "0" ]; then
        echo -e "${Green}Installation de la cross-compilation pour Raspberry réussie${End}"
        raspi_installed=1
    else
        echo -e "${Red}Installation de la cross-compilation pour Raspberry échouée${End}"
    fi
fi


if [ $install_opencv -eq 1 ]; then
	OCV_MIN_VERSION_REQUIRED=2
	echo -e "${Yellow}Verification des outils OpenCV${End}"
	
	if [ `dpkg-query -W libopencv-dev | awk '{print $1}'` == "libopencv-dev" ]; then
		OCV_VERSION=$(dpkg-query -W libopencv-dev | awk '{print $2}' | awk -F . '{print $1}')
		if [ $OCV_VERSION -ge $OCV_MIN_VERSION_REQUIRED ]; then
			echo -e "${Green}OpenCV déjà installé avec la version minimale requise${End}"
		else
			echo -e "${Red}OpenCV installé avec une version obsolete${End}"
		fi
	else
	
		echo -e "${Yellow}Installation des outils OpenCV${End}"
		sudo apt-get install libopencv-dev 
	fi

fi

















