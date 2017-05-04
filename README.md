# info
------------
Installation de tout ce qu'il faut
------------

```
sudo apt-get update
sudo apt-get install mono-complete cmake subversion git libbox2d-dev gcc-5-multilib g++-5-multilib libbluetooth-dev build-essential libglfw-dev libglu1-mesa-dev curl libxrandr-dev libgtkmm-3.0-dev libusb-1.0-0-dev clang-format

```



Pour installer g++-6 et gcc-6, la méthode est la suivante :

```
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt-get update
sudo apt-get install gcc-6 g++-6
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-6 60 --slave /usr/bin/g++ g++ /usr/bin/g++-6
```

### Facultatif :
Pour installer la lib Wiic nécessaire pour les démos à la wiimote
```
pushd .
cd /tmp
git clone https://github.com/grandelli/WiiC.git
cd WiiC
cmake src
make
sudo make install
```
Pour installer le hook (script lancer à chaque commit) qui formate le code comme il faut (**facultatif mais forte   ment recommandé**) :
```
./install.sh
```

Pour installer clang en tant que compilateur :
```
sudo apt-get install clang-3.8
sudo update-alternatives --install /usr/bin/c++ c++ /usr/bin/clang++-3.8 100
```
Pour l'éditeur petri :
```
sudo apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv-keys 3FA7E0328081BFF6A14DA29AA6A19B38D3D831EF
echo "deb http://download.mono-project.com/repo/debian wheezy main" | sudo tee /etc/apt/sources.list.d/mono-xamarin.list
```
------------
Clonage du repo
------------
```
svn checkout https://www.etud.insa-toulouse.fr/svn/roboinsat/CodeCommun --username votre_login_insa
git clone https://votre_login_github@github.com/ClubRobotInsat/info.git
cd info

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
```
sudo usermod -a -G dialout NOM_UTILISATEUR
```

### Si ça ne compile pas : 
- Aller dans Settings > Build, Execution, Deployment > CMake. Dans "CMake options", rajouter le paramètre `-DBITS=64`
- Problème de clock skew ? Il suffit de lancer `find -exec touch \{\} \;` dans le dossier `info`. Attention, cela peut être un petit peul ong.

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

------------
Cross-Compilation pour Raspberry
------------
### Installation des outils :
```
sudo apt-get install gcc-5-arm-linux-gnueabihf g++-5-arm-linux-gnueabihf build-essential git
```
Pour compiler IAWiimote il faut copier les librairies précompilées pour ARM dans le dossier `/usr/arm-linux-gnueabihf/lib`.


Pour cela il suffit d'exécuter :
 
```sudo cp sudo cp precompiled-libraries/libbluetooth.so.3 precompiled-libraries/libwiicpp.so precompiled-libraries/libwiic.so /usr/arm-linux-gnueabihf/lib```

#### Installation des libs pour la WiiMote
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
###Cross-compilation
Un script build_arm.sh est fourni, il compilera IAPrincipal et IAWiimote dans info/build_arm.
Il est possible de donner le nombre de coeur à utiliser au script :
```
 ./build_arm.sh 8
```

### Envoyer le script sur le Raspberry
#### Configuration de ton ordinateur
Il faut commencer par ouvrir le gestionnaire de connections de ton ordinateur. Pour cela il faut cliquer sur l'icône "Network" dans ta barre des tâches et sélectionner "Edit Connections". Une fois dans ce menu, il est possible de definir une nouvelle connexion Ethernet qui sera partagée avec le Raspberry.


On clique donc sur "Add" et on choisi "Ethernet".
Une fois dans le menu de paramètrage de la connexion il faut modifier quelques champs :


1. Connection name : A remplir pour différencier la connection des autres


2. Onglet Ethernet :


  * Device : Normalement il n'y en a que un à chosir
  S'il y en a plusieurs, il faut tous les essayer jusqu'à ce que ça marche.

3. Onglet IPv4 Settings : 


  * Method : Choisir "Shared to other computers"



Les autres champs peuvent être laissés tel quel.

Une fois la configuration terminée dans ton gestionnaire de connexion, active la configuration créée.

Ensuite, pour pouvoir se ssh sur le RaspberryPI il suffit de lancer le script :
```
./ssh_into_rpi.sh
```
Le mot de passe de l'utilisateur robot est "robot".



Pour copier des fichiers il faut lancer un autre script, avec en argument le fichier a envoyé. Celui-ci se trouvera alors sur le bureau. 
```
./send_file_to_rpi.sh build/robot/Principal/IAPrincipal
```
Le mot de passe de l'utilisateur robot est "robot".
