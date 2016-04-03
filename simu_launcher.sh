#!/bin/bash

url="http://etud.insa-toulouse.fr/~slacoste/simu/"

platform=$(uname)

if [[ "$platform" == "Linux" ]]; then
    xdg-open "$url"
elif [[ "$platform" == "Darwin" ]]; then
    open "$url"
fi

echo "Launching Petri..."
petri/petri -o robot/Principal/petri/IA.petri && fg
