#!/bin/bash

#Arguments
compile_principal=0
compile_wii=0
compile_test=0
cores=4

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
		elif ( [[ "$arg" =~ ^[-+]?[0-9]+$ ]] )
			then cores=$arg
		else
			echo "Usage : $0 <nbr of cores> [all|principal|wii|test]"
			exit
		fi
	done
else
	echo "Usage : $0 <nbr of cores> [all|principal|wii|test]"
	compile_principal=1
	compile_wii=1
	compile_test=1
fi

Green='\e[1;32m'
End='\e[0m' 

echo -e "${Green}Compiling with -j ${cores}${End}"

dir=$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)
cd ${dir}

rm -rf build_arm.LAST_SAVE && mv build_arm build_arm.LAST_SAVE && mkdir build_arm && cd build_arm

echo -e "${Green} Generating CMakeFile${End}"
cmake .. -DBITS=64 -DRASPI="1" -DCMAKE_TOOLCHAIN_FILE="../Cross-Compilation.cmake"

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

