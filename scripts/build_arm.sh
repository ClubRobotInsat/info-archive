#!/bin/bash

cores=4

Green='\e[1;32m'
End='\e[0m' 

if !( [[ "$1" =~ ^[-+]?[0-9]+$ ]] )
then
	echo -e "${Green} Compiling with -j 4 ${End}"
else
	cores=$1
	echo -e "${Green} Compiling with -j ${cores} ${End}"
fi

dir=$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)
cd ${dir}

if `! test -d build_arm`  
	then	mkdir build_arm
fi

cd build_arm
echo -e "${Green} Generating CMakeFile ${End}"
cmake .. -DBITS=64 -DRASPI="1" -DCMAKE_TOOLCHAIN_FILE="../Cross-Compilation.cmake"

echo -e "${Green} Building IAPrincipal ${End}"
make IAPrincipal -j ${cores}

echo -e "${Green} Building IAWiimote ${End}"
make IAWiimote -j ${cores}

echo -e "${Green} Building IATestRobot ${End}"
make IATestRobot -j ${cores}
