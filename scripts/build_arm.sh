#!/bin/bash

#Arguments
compile_principal=0
compile_wii=0
compile_test=0
compile_lidar=0
cores=$(nproc)

if [ $# -ne "0" ]
	then for arg in $*
	do
		if [ "$arg" = "all" ]
			then compile_principal=1
			compile_wii=1
			compile_test=1
		elif [ "$arg" = "principal" ]
			then compile_principal=1
		elif [ "$arg" = "wii" ]
			then compile_wii=1
		elif [ "$arg" = "test" ]
			then compile_test=1
		elif [ "$arg" = "lidar" ]
			then compile_lidar=1
		else
			echo "Usage : $0 [all|principal|wii|test|lidar]"
			exit
		fi
	done
else
	echo "Usage : $0 [all|principal|wii|test|lidar]"
	compile_principal=1
	compile_wii=1
	compile_test=1
	compile_lidar=1
fi

Green='\e[1;32m'
Yellow='\e[1;33m'
End='\e[0m'

echo -e "${Green}Compiling with -j ${cores}${End}"

dir=$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)
cd ${dir}

mkdir -p build_arm && cd build_arm

echo -e "${Green} Generating CMakeFile${End}"
cmake .. -DRASPI="1" -DCMAKE_TOOLCHAIN_FILE="../Cross-Compilation.cmake"

echo -e "${Green} BuildAll${End}"
make all -j 4
if [ $? -ne "0" ]
	then echo -e "${Red}Failed to BuildAll${End}"
fi

echo -e "${Yellow}TODO: upgrade this script to compile only some targets for strategy.${END}"
exit 1

if [ $compile_principal -eq 1 ]
	then echo -e "${Green} Building IAPrincipal${End}"
	make IAPrincipal -j ${cores}
	if [ $? -ne "0" ]
		then echo -e "${Red}Failed to build IAPrincipal${End}"
	fi
fi

if [ $compile_wii -eq 1 ]
	then echo -e "${Green} Building IAWiimote${End}"
	make IAWiimote -j ${cores}
	if [ $? -ne "0" ]
		then echo -e "${Red}Failed to build IAPrincipal${End}"
	fi
fi

if [ $compile_test -eq 1 ]
	then echo -e "${Green} Building IATestRobot${End}"
	make IATestRobot -j ${cores}
	if [ $? -ne "0" ]
		then echo -e "${Red}Failed to build IAPrincipal${End}"
	fi
fi

if [ $compile_lidar -eq 1 ]
	then echo -e "${Green} Building affLidar${End}"
	make printRobots -j ${cores}
	if [ $? -ne "0" ]
		then echo -e "${Red}Failed to build printRobots${End}"
	fi
fi

