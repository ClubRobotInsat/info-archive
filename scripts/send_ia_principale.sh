#!/bin/bash

dir=$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)
cd ${dir}

echo ""
echo "Compilation du réseau Petri"
petri/petri --generate --compile --compiler /usr/bin/arm-linux-gnueabihf-g++-5 -v --output ../../../build_arm/ robot/Principal/petri/IA2017.petri

#/usr/bin/arm-linux-gnueabihf-g++-5 -c -fPIC -iquote'robot/Principal/petri/src' -I'robot/Principal/petri/src' -iquote'./petri' -I'./petri' -iquote'./commun' -I'./commun' -std=c++14 -o './build_arm/poserCylindre.o'  'robot/Principal/petri/src/poserCylindre.cpp'
#/usr/bin/arm-linux-gnueabihf-g++-5 -c -fPIC -iquote'robot/Principal/petri/src' -I'robot/Principal/petri/src' -iquote'./petri' -I'./petri' -iquote'./commun' -I'./commun' -std=c++14 -o './build_arm/IA2017.o'  'robot/Principal/petri/src/IA2017.cpp'

cd build_arm

echo ""
echo "Génération de la lib dynamique"
#/usr/bin/arm-linux-gnueabihf-g++-5 -shared -fPIC -L'../Editor/bin' -iquote'../' -I'../' 'IA2017.o' -o 'IA2017.so'

echo ""
cd ../scripts
./send_file_to_rpi.sh ../build_arm/IA2017.so
./send_file_to_rpi.sh ../build_arm/robot/Principal/IAPrincipal
./send_file_to_rpi.sh ../build_arm/petri/Runtime/libPetriRuntime.so
