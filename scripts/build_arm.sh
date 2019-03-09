#!/bin/bash

#Arguments
compile_all=0
compile_primary=0
compile_wii=0
compile_test=0
compile_lidar=0
cores=$(nproc)

if [ $# -ne "0" ]
	then for arg in $*
	do
		if [ "$arg" = "all" ]
			then compile_all=1
		elif [ "$arg" = "primary" ]
			then compile_primary=1
		elif [ "$arg" = "wii" ]
			then compile_wii=1
		elif [ "$arg" = "test" ]
			then compile_test=1
		elif [ "$arg" = "lidar" ]
			then compile_lidar=1
		else
			echo "Usage : $0 [all|primary|wii|test|lidar]"
			exit
		fi
	done
else
	echo "Usage : $0 [all|primary|wii|test|lidar]"
	compile_primary=1
	compile_wii=1
	compile_test=1
	compile_lidar=1
fi

Green='\e[1;32m'
Yellow='\e[1;33m'
End='\e[0m'

echo -e "${Green}Compiling with '-j ${cores}'${End}"

dir=$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)
cd ${dir}

mkdir -p build_arm && cd build_arm

echo -e "${Green} Generating CMakeFile${End}"
cmake .. -DRASPI="1" -DCMAKE_TOOLCHAIN_FILE="../Cross-Compilation.cmake"

if [ $compile_all -eq 1 ]
    then echo -e "${Green}Building all targets${End}"
    make all -j ${cores}
    if [ $? -ne "0" ]
        then echo -e "${Red}Failed to build all targets${End}"
    fi
fi

if [ $compile_primary -eq 1 ]
	then echo -e "${Green}Building IAPrimary${End}"
	make IAPrimary -j ${cores}
	if [ $? -ne "0" ]
		then echo -e "${Red}Failed to build IAPrimary${End}"
	fi
fi

if [ $compile_wii -eq 1 ]
	then echo -e "${Green}Building IAWiimote${End}"
	make IAWiimote -j ${cores}
	if [ $? -ne "0" ]
		then echo -e "${Red}Failed to build IAWiimote${End}"
	fi
fi

if [ $compile_test -eq 1 ]
	then echo -e "${Green} Building unit_testing_all${End}"
	make unit_testing_all -j ${cores}
	if [ $? -ne "0" ]
		then echo -e "${Red}Failed to build unit_testing_all${End}"
	fi
fi

if [ $compile_lidar -eq 1 ]
	then echo -e "${Green} Building Lidar::printRobots${End}"
	make printRobots -j ${cores}
	if [ $? -ne "0" ]
		then echo -e "${Red}Failed to build Lidar::printRobots${End}"
	fi
fi

