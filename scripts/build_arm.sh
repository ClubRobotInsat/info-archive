#!/bin/bash

print_usage () {
    echo "Usage : $0 [all|primary|wii|test|ia_test|lidar]"
}

#Arguments
compile_all=0
compile_primary=0
compile_wii=0
compile_test=0
compile_ia_test=0
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
        elif [ "$arg" = "ia_test" ]
            then compile_ia_test=1
        elif [ "$arg" = "lidar" ]
            then compile_lidar=1
        else
            print_usage
            exit
        fi
    done
else
    print_usage
    compile_primary=1
    compile_wii=1
    compile_test=1
    compile_ia_test=1
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

building_process () {
    target="$1"
    echo -e "${Green}Building '${target}'${End}"
    make "${target}" -j ${cores}
    if [ $? -ne "0" ]
        then echo -e "${Red}Failed to build '${target}'${End}"
    fi
}

if [ $compile_all -eq 1 ]; then
    building_process "all"
fi

if [ $compile_primary -eq 1 ]; then
    building_process "IAPrimary"
fi

if [ $compile_wii -eq 1 ]; then
    building_process "IAWiimote"
fi

if [ $compile_test -eq 1 ]; then
    building_process "unit_testing_all"
fi

if [ $compile_ia_test -eq 1 ]; then
    building_process "IATest"
fi

if [ $compile_lidar -eq 1 ]; then
    building_process "printRobots"
fi

