#!/bin/bash

color=green
if $(test $# -eq 1)
then
	if [ "$1" = "orange" ]
	then
		color=orange
	fi
elif ! [ "$1" = "green" ]
then
	echo "Usage : simu_launcher.sh [green|orange]"
	echo "Default behaviour: --robot green"
fi

dir="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
echo -e "To launch Petri, run \033[0;44m'petrilab'\033[0;0m"
echo ""

if $(test -e "$dir"/build/src/simulateur/Simulator)
then
	echo "Launching simu..."
	cd "$dir"/build/src/simulateur/
	./Simulator --robot on --world on --color "$color" --load ../table_2018.json
elif $(test -e "$dir"/cmake-build-debug/src/simulateur/Simulator)
then
	echo "Launching simu..."
	cd "$dir"/cmake-build-debug/src/simulateur/
	./Simulator --robot on --world on --color "$color" --load ../table_2018.json
fi
