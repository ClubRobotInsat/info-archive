# Info

## Clonage du repo
```
git clone https://votre_login_github@github.com/ClubRobotInsat/CodeCommun.git
git clone https://votre_login_github@github.com/ClubRobotInsat/info.git
cd info
git clone https://github.com/ClubRobotInsat/websimulator
```

##  Installation des différents outils
L'installation se fait grâce au script d'installation
```
scripts/install.sh tools
```

### Facultatif :
Pour installer le hook (script lancé à chaque commit) qui formate le code comme il faut (**facultatif mais fortement recommandé**) :
```
scripts/install.sh format
```
Pour installer la lib Wiic nécessaire pour les démos à la wiimote
```
scripts/install.sh wii
```
Pour l'éditeur petri (versions pour les systèmes Ubuntu 14.04 **Trusty** et Ubuntu 17.04 **Zesty** seulement)
```
scripts/install.sh petri
```

Pour installer clang en tant que compilateur :
```
sudo apt-get install clang-3.8
sudo update-alternatives --install /usr/bin/c++ c++ /usr/bin/clang++-3.8 100
```
Pour installer g++-6 et gcc-6, la méthode est la suivante :

```
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt-get update
sudo apt-get install gcc-6 g++-6
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-6 60 --slave /usr/bin/g++ g++ /usr/bin/g++-6
```

### Installation de CLion (facultatif, mais recommandé)
CLion est un IDE développé par JetBrains, qui est à la fois flexible, simple d'utilisation, multiplateforme et qui a la particularité d'utiliser nativement CMake, le système de build utilisé par notre code.
Il est normalement payant, mais une version d'éducation (licence valide 1 an, renouvelable tant que l'on est étudiant) est disponible.

Pour commencer, aller sur cette page ```https://www.jetbrains.com/clion/download/``` et télécharger l'IDE.

Pendant le téléchargement, aller sur la page ```https://www.jetbrains.com/shop/eform/students``` pour demander une licence éducation.
Le mail à fournir est celui de l'INSA (@etud.insa-toulouse.fr) pour que JetBrains accepte de délivrer la licence. Va voir sur ta boîte mail, un message contenant un lien de confirmation à suivre est arrivé. Une fois que l'adresse mail est confirmée, un autre mail est envoyé et il permet de recevoir les infos de la licence, qui pourront alors être saisies dans CLion.

Une fois l'IDE installé, on choisit d'ouvrir un projet existant et il faut sélectionner le dossier ```info``` résultat du clonage du Git.

Il reste à ce moment un peu de configuration à effectuer : dans le menu ```File -> Settings```, dérouler l'item ```Build, Execution, Deployment``` et sélectionner ```CMake```.
Dans le champ ```CMake options```, mettre le texte ```-DDEBUG=on -DBITS=64```.

Pour pouvoir tester les IA via la laison série (RS232) en utilisant Clion (sans les droits root), il faut ajouter votre utilisateur au groupe ayant accès à la laison série (dialout) :
```
sudo usermod -a -G dialout NOM_UTILISATEUR
```

### Si ça ne compile pas : 
- Aller dans Settings > Build, Execution, Deployment > CMake. Dans "CMake options", rajouter le paramètre `-DBITS=64`
- Problème de clock skew ? Il suffit de lancer `find -exec touch \{\} \;` dans le dossier `info`. Attention, cela peut être un petit peu long.

## Pour lancer l'IA en mode Debug sur le simu
1. Tout compiler (choisir la target build all)
2. Ouvrir un premier terminal dans le dossier info, puis se placer dans le dossier ```cd build/src/simulateur```.
3. Lancer le simu : ```./Simu2017 --robot on --color blue --world on```.
4. Ouvrir un deuxième terminal dans le dossier info, puis lancer Petri : ```../../../third_parties/petri/petri```.
5. Dans Petri, charger l'IA (IA.petri ou IADebug.petri, situé dans ```/info/robot/Principal/petri```).
6. Dans Petri, aller dans ```Debug -> Create Host```, normalement tout est déjà configuré, vous n'avez qu'à cliquer sur **Create host**. Sinon, il faut aller chercher les binaires de l'IA : ```/info/build/src/robot/Principal/IAPrincipal``` et indiquer comme arguments ```LOCAL --color blue SIMU```
7. Voilà c'est terminé, vous n'avez plus qu'à lancer votre réseau petri et admirer le résultat dans le simu !

------------

## Cross-Compilation pour Raspberry
### Installation des outils :
```
sudo apt-get install gcc-5-arm-linux-gnueabihf g++-5-arm-linux-gnueabihf build-essential git
```
Pour compiler IAWiimote il faut copier les librairies précompilées pour ARM dans le dossier `/usr/arm-linux-gnueabihf/lib`.


Pour cela il suffit d'exécuter :

```sudo cp precompiled-libraries/libbluetooth.so.3 precompiled-libraries/libwiicpp.so precompiled-libraries/libwiic.so /usr/arm-linux-gnueabihf/lib```
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
Il est possible de donner le nombre de coeurs à utiliser au script :
```
 cd scripts
 ./build_arm.sh 8
```


Si il y a des problèmes de compilations, il faut commencer par supprimer le dossier /build_arm.


### Envoyer le script sur le Raspberry
#### Configuration de ton ordinateur
Il faut commencer par ouvrir le gestionnaire de connections de ton ordinateur. Pour cela il faut cliquer sur l'icône "Network" dans ta barre des tâches et sélectionner "Edit Connections". Une fois dans ce menu, il est possible de définir une nouvelle connexion Ethernet qui sera partagée avec le Raspberry.


On clique donc sur "Add" et on choisit "Ethernet".
Une fois dans le menu de paramétrage de la connexion il faut modifier quelques champs :


1. Connection name : A remplir pour différencier la connection des autres


2. Onglet Ethernet :


  * Device : Normalement il n'y en a que un à chosir.
  S'il y en a plusieurs, il faut tous les essayer jusqu'à ce que ça marche.

3. Onglet IPv4 Settings :


  * Method : Choisir "Shared to other computers"



Les autres champs peuvent être laissés tel quel.

Une fois la configuration terminée dans ton gestionnaire de connexion, active la configuration créée.

Ensuite, pour pouvoir se ssh sur le RaspberryPI il suffit de lancer le script :
```
./ssh_into_rpi.sh
```
Le mot de passe de l'utilisateur robot est "**robot**".

Pour copier des fichiers il faut lancer un autre script, avec en argument le fichier à envoyer. Celui-ci se trouvera alors sur le bureau.
```
./send_file_to_rpi.sh build/robot/Principal/IAPrincipal
```
