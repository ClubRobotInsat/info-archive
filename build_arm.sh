#!/bin/bash

cores=4

dgreen='\e[1;32m'
egreen='\e[0m' 

if !( [[ "$1" =~ ^[-+]?[0-9]+$ ]] )
then
	echo -e "$dgreen Compiling with -j 4 $egreen"
else
	cores=$1
	echo -e "$dgreen Compiling with -j $cores $egreen"
fi

if `! test -d build_arm`  
	then	mkdir build_arm
fi

cd build_arm
echo -e "$dgreen Generating CMakeFile $egreen"
cmake .. -DBITS=64 -DCMAKE_TOOLCHAIN_FILE=Cross-Compilation.cmake 

echo -e "$dgreen Building IAPrincipal $egreen"
make IAPrincipal -j $cores

echo -e "$dgreen Building IAWiimote $egreen"
make IAWiimote -j $cores
