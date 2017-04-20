#!/bin/bash

color=blue
if `test $# -eq 1`
then
	if [ "$1" = "yellow" ]
	then
		color=yellow
	fi
elif ! [ "$1" = "blue" ]
then
	echo "Usage : simu_launcher.sh [blue|yellow]"
	exit 1
fi

dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

echo "Launching web_server..."
#../websimulator/serve.sh &

echo "Launching Petri..."
"$dir"/petri/petri &

echo salut

if `test -e "$dir"/build/Simu2017`
then
	echo "Launching simu..."
	"$dir"/build/Simu2017 --robot on --world on --color "$color"
elif `test -e "$dir"/cmake-build-debug/simulateur/Simu2017`
then
	echo "Launching simu..."
	"$dir"/cmake-build-debug/simulateur/Simu2017 --robot on --world on --color "$color"
fi
