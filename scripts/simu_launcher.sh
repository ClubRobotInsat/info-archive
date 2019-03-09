#!/bin/bash

color=yellow
if $(test $# -eq 1); then
	if [ "$1" = "purple" ]; then
		color=purple
	fi
elif ! [ "$1" = "yellow" ]; then
	echo "Usage : simu_launcher.sh [yellow|purple]"
	echo "Default behaviour: --robot yellow"
fi

dir="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
echo -e "To launch Petri, run \033[0;44m'petrilab'\033[0;0m"
echo ""

if $(test -e "$dir"/build/src/simulateur/Simulator); then
	echo "Launching simu..."
	cd "$dir"/build/src/simulateur/
	./Simulator --robot on --world on --color "$color" --load ../../../src/commun/table_2019.json
elif $(test -e "$dir"/cmake-build-debug/src/simulateur/Simulator); then
	echo "Launching simu..."
	cd "$dir"/cmake-build-debug/src/simulateur/
	./Simulator --robot on --world on --color "$color" --load ../../../src/commun/table_2019.json
fi
