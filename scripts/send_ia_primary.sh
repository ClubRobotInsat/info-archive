#!/bin/bash

dir=$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)
cd ${dir}

# echo -e
Red='\033[0;41m'
Green='\033[0;42m'
Yellow='\033[0;43m'
End='\033[0;0m'

scripts/build_arm.sh primary

if [ $? -eq "0" ]; then
    echo -e "${Green}Compilation réussie${End}"
else
	echo -e "${Red}Compilation échouée${End}"
	exit 1
fi

echo ""
./scripts/send_file_to_rpi.sh ./build_arm/src/robot/Strategy/Primary.so
./scripts/send_file_to_rpi.sh ./build_arm/IAPrimary
