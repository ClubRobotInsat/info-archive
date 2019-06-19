# Dossier `Principal`

Actuellement, il n'y a plus que 2 fichiers intéressants :
* `librobot/Robot.[cpp|h]` : classe qui hérite d'un robot commun avec les bonnes valeurs d'initialisation
* `debug/CalibrationDepla.[cpp|h]` : programme utilitaire pour récupérer les valeurs de calibration ; déprécié depuis la refonte du déplacement

Il faudra donc déplacer `librobot/Robot.[cpp|h]` directement dans `src/robot/RobotPrincipal.[cpp|h]`.