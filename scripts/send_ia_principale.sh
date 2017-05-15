#!/bin/bash

scripts/send_file_to_rpi.sh build_arm/robot/Principal/IAPrincipal

echo "Compilation du petri"
/usr/bin/arm-linux-gnueabihf-g++-5 -c -fPIC -iquote'robot/Principal/petri/src' -I'robot/Principal/petri/src' -iquote'./petri' -I'./petri' -iquote'./commun' -I'./commun' -std=c++14 -o './build_arm/IA2017.o'  'robot/Principal/petri/src/IA2017.cpp' 

cd build_arm

echo "Génération de la lib dynamique"
/usr/bin/arm-linux-gnueabihf-g++-5 -shared -fPIC -L'../Editor/bin' -iquote'../' -I'../' 'IA2017.o' -o 'IA2017.so'

cd ..

scripts/send_file_to_rpi.sh build_arm/IA2017.so
