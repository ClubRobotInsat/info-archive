# info
------------
Installation de tout ce qu'il faut
------------

```
sudo apt-get update
sudo apt-get install ninja-build mono-complete cmake subversion git libbox2d-dev gcc-5-multilib g++-5-multilib openjdk-7-jre build-essential libglfw-dev libglu1-mesa-dev libxrandr-dev libgtk2.0-dev libusb-1.0-0-dev clang-format-3.7
```
------------
Clonage du repo
------------
```
svn checkout https://www.etud.insa-toulouse.fr/svn/roboinsat/CodeCommun --username votre_login_insa
git clone https://votre_login_github@github.com/ClubRobotInsat/info.git
cd info
./boostrap.sh
```

------------
Installation de CLion (facultatif, mais recommandé)
------------
CLion est un IDE développé par JetBrains, qui est à la fois flexible, simple d'utilisation, multiplateforme et qui a la particularité d'utiliser nativement CMake, le système de build utilisé par notre code.
Il est normalement payant, mais une version éducation (licence valide 1 an, renouvelable tant que l'on est étudiant) est disponible.

Pour commencer, aller sur cette page ```https://www.jetbrains.com/clion/download/``` et télécharger l'IDE.

Pendant le téléchargement, aller sur la page ```https://www.jetbrains.com/shop/eform/students``` pour demander une licence éducation.
Le mail à fournir est celui de l'INSA (@etud.insa-toulouse.fr) pour que JetBrains accepte de délivrer la licence. Va voir sur ta boîte mail, un message contenant un lien de confirmation à suivre est arrivé. Une fois que l'adresse mail est confirmée, un autre mail est envoyé et il permet de recevoir les infos de la licence, qui pourront alors être saisies dans CLion.

Une fois l'IDE installé, on choisit d'ouvrir un projet existant et il faut sélectionner le dossier ```info``` résultat du clonage du Git.

Il reste à ce moment un peu de configuration à effectuer : dans le menu ```File -> Settings```, dérouler l'item ```Build, Execution, Deployment``` et sélectionner ```CMake```.
Dans le champ ```CMake options```, mettre le texte ```-DDEBUG=on -DBITS=64```.

L'IDE est presque configuré. Maintenant, lancer un build, et l'interrompre (car il va échouer).
La première ligne de la sortie console (tout en haut) nous intéresse: sélectionner et copier la valeur de l'argument ```--build```, et dans un terminal saisir la commande ```ln -s <valeur de l'argument --build> <chemin du parent dossier info>/bin```.

Voilà, maintenant l'IDE est configuré et la compilation du code info devrait fonctionner à 100 %.


--------------------------
Documentation Box2D (simu)
--------------------------
http://box2d.org/manual.pdf


--------------------------
Pour lancer le simulateur en mode Debug
--------------------------
Je considère que vous avez lancé petri.

1.Compiler, si ce n'est déjà fait, la target Simu2016
2.Plusieurs options s'offrent à vous : lancer le simu depuis clion ou aller (quand vous êtes dans le dossier /info) dans ../bin/simulateur/simulateur) et taperer
"./Simu2016" 
3.Une fois le simu lancé, on se connecte sur http://etud.insa-toulouse.fr/~slacoste/simu/

--------------------------
Architecture de la refonte
--------------------------

diagramme ""uml"" : https://drive.google.com/folderview?id=0Byjvg2rL9zBvdzhSZ1JDVEhUTXc&usp=sharing
