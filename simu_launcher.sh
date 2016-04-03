#!/bin/sh
echo "Launching the AI..."
../bin/robot/Principal/IAPrincipal LOCAL &
echo "Launching Petri..."
mono petri/Editor/bin/Petri.exe && fg 
