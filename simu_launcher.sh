#!/bin/bash

url="http://etud.insa-toulouse.fr/~slacoste/simu/"

platform=$(uname)

if [[ "$platform" == "Linux" ]]; then
    xdg-open "$url"
elif [[ "$platform" == "Darwin" ]]; then
    open "$url"
fi

dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

echo "Launching Petri..."
"$dir"/petri/petri -o robot/Principal/petri/IADebug.petri
