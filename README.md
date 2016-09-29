# info
------------
Installation de tout ce qu'il faut
------------

```
sudo apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv-keys 3FA7E0328081BFF6A14DA29AA6A19B38D3D831EF
echo "deb http://download.mono-project.com/repo/debian wheezy main" | sudo tee /etc/apt/sources.list.d/mono-xamarin.list
sudo apt-get update
sudo apt-get install mono-complete cmake subversion git libbox2d-dev gcc-5-multilib g++-5-multilib libbluetooth-dev build-essential libglfw-dev libglu1-mesa-dev libxrandr-dev libgtkmm-3.0-dev libusb-1.0-0-dev clang-format-3.7
```

Pour installer clang en tant que compilateur :
```
sudo apt-get install clang-3.8
sudo update-alternatives --install /usr/bin/c++ c++ /usr/bin/clang++-3.8 100
```
------------
Clonage du repo
------------
```
svn checkout https://www.etud.insa-toulouse.fr/svn/roboinsat/CodeCommun --username votre_login_insa
git clone https://votre_login_github@github.com/ClubRobotInsat/info.git
cd info
mkdir build
cd build
cmake .. -DDEBUG=on

git clone https://github.com/ClubRobotInsat/websimulator
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
Dans le champ ```CMake options```, mettre le texte ```-DDEBUG=on```.

Pour pouvoir tester les IA via la laison série (RS232) en utilisant Clion (sans les droits root), il faut ajouter votre utilisateur au groupe ayant accès à la laison série (dialout) :
```sudo usermod -a -G dialout NOM_UTILISATEUR```

--------------------------
Pour lancer l'IA en mode Debug sur le simu
--------------------------
1. Tout compiler (choisir la target build all)
2. Ouvrir un premier terminal dans le dossier info, puis se placer dans le dossier ```cd build```.
3. Lancer le simu : ```./Simu2016 -robot on purple -world on```.
4. Ouvrir un deuxième terminal dans le dossier info, puis se placer dans le dossier Petri : ```cd petri/Editor/bin```.
5. Lancer Petri, toujours dans ce deuxième terminal : ```./Petri.exe```
6. Dans Petri, charger l'IA (IA.petri ou IADebug.petri, situé dans ```/info/robot/Principal/petri```).
7. Ouvrir un troisième terminal à coté du dossier info, puis se placer dans le dossier ```websimulator/dist```.
8. Ecrire la commande ```python -m SimpleHTTPServer```.
9. Dans le navigateur Web, se connecter sur ```localhost:8000``` puis cliquer sur connect. Après une petite seconde, vous devriez voir la table apparaître.
10. Dans Petri, aller dans ```Debug -> Creat Host```, normalement tout est déjà configuré, vous n'avez qu'à cliquer sur Create Host. Sinon, faut aller chercher les binaires de l'IA :```/bin/robot/Principal/IAPrincipal``` et indiquer comme argument : ```LOCAL```.
11. Voilà c'est terminé, vous n'avez plus qu'a lancer votre réseau petri et admiré le résultat dans le simu !


## Que faire ? 

#### Débugger le simu et l'IA:

Pour cela, lancer le simu, ouvrir la console qui reçoit les message de l'IA (alt gr + 2 , le combo qui fait ~ avec ALT GR).
Ensuite faut tester toutes les méthodes petri, verifier que le simu les gèrent bien. Si jamais ils les gèrent bien et que rien ne s'affiche c'est que c'est côté robot que ça merde.
Comment savoir si c'est bien geré ? Les méthodes correspondent à la doc des élecs, tout simplement.
Voilà voilà.

#### TODO : fix les méthodes suivantes

carte électrovanne : fait crash le simu ("Une erreur est survenue dans le client du débuggueur : Le débuggueur distant n'est plus disponible !.")
* descendreCuillere()
* monterCuillere()
* descendrePelle()
* monterPelle()
* sortirParasol()
* rentrerParasol()
* sortirBrasDroit()
* sortirBrasGauche()
* rentrerBrasDroit()
* rentrerBrasGauche()

partie pneumatique :  
quand on active/désasctive une pompe seule la pompe BAS est concernée (ID = 6), on ne peut pas modif la pompe HAUTE  
  
je ne sais pas ce que font exactement ces méthodes -Benjamin
* allerA(Distance, Distance)   ... ne marche pas (normal d'après Paul)
* allerA(Vector2m)             ... ne marche pas (normal d'après Paul)
* reculerA(Vector2m)
  
ne marche pas : ça attend dans Petri, il y a le message "[CAN.cpp:221 in marquerAckRecu] : On reçoit un ack pour le message 56, déjà évacué de la fenêtre" dans le terminal
* recupererCubes()
* relacherCubes()  
  
sinon les déplacements se font bien (et les cartes ont accès à la pos + angle de départ du robot, il n'y a pas de souci avec ça)

------------
Cross-Compilation pour Raspberry
------------
###Installation des outils :
```
sudo apt-get install gcc-5-arm-linux-gnueabihf g++-5-arm-linux-gnueabihf build-essential git
```
Pour crosscompiler la wiimote, il faut installer ```libbluetooth-dev``` en version arm, et aussi cross-compiler WiiC en arm.
Ensuite faut mettre les libs dans ```/usr/arm-linux-gnueabihf/lib```

------------
Installation des outils pour le controle du robot à la wiimote
------------

```
sudo apt-get install libbluetooth-dev
cd <WIIC_HOME>
git clone https://github.com/grandelli/WiiC.git
cd WiiC
mkdir build
cd build
cmake ../src
make
sudo make install
sudo ldconfig
```

_<WIIC_HOME>_ désigne le dossier dans lequel installer Wiic