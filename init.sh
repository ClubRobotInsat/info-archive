#!/bin/sh
# init.sh
# initialise une connexion avec l'epia

ROBOT_ADR="robot@10.30.42.42"
IA_PATH="./robot/2009/IA/strategie_exemple"
PARAM_PATH="./robot/2009/IA/params_webcam_rouge.txt ./robot/2009/IA/params_webcam_vert.txt ./robot/2009/IA/parametres_reconnaissance.txt"
ROBOT_PATH="$ROBOT_ADR:~"

# copie de l'ia
gnome-terminal -e scp $IA_PATH $ROBOT_PATH

# copie des reglages
gnome-terminal -e scp $PARAM_PATH $ROBOT_PATH

# connexion ssh
ssh $ROBOT_ADR
