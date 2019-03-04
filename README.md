[![Build Status](https://travis-ci.org/ClubRobotInsat/info.svg?branch=develop)](https://travis-ci.org/ClubRobotInsat/info)
[![Coverage Status](https://coveralls.io/repos/github/ClubRobotInsat/info/badge.svg?branch=develop)](https://coveralls.io/github/ClubRobotInsat/info?branch=develop)
[![CodeCov](https://codecov.io/gh/ClubRobotInsat/info/branch/develop/graph/badge.svg)](https://codecov.io/gh/ClubRobotInsat/info)
[![CodeFactor](https://www.codefactor.io/repository/github/clubrobotinsat/info/badge/develop)](https://www.codefactor.io/repository/github/clubrobotinsat/info/overview/develop)
[![Github license](https://img.shields.io/github/license/ClubRobotInsat/info.svg)](https://github.com/ClubRobotInsat/info/blob/develop/LICENSE)
[![Github Issues](https://img.shields.io/github/issues/ClubRobotInsat/info.svg)](https://github.com/ClubRobotInsat/info/issues)
[![Website](https://img.shields.io/website-up-down-green-red/https/clubrobotinsat.github.io.svg
)](https://clubrobotinsat.github.io/)
![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/ClubRobotInsat/info.svg)

![logo](https://etud.insa-toulouse.fr/~club_robot/sites/default/files/club_robot_smallsmall.png)

Ce document permets de recenser l'ensemble des commandes utiles pour prendre en main le code informatique.

* :package: [Clonage du repo](#clonage-du-repo)
* :hammer: [Installation des différents outils](#installation-des-differents-outils)
  - [Logiciels nécessaires](#logiciels-necessaires) **[REQUIS]**
  - [Formattage du code](#formattage-du-code) _[FACULTATIF] **[RECOMMANDE]**_
  - [_gcc_ et _g++_ 6/7 pour compiler](#gcc-recent-en-tant-que-compilateur) _[FACULTATIF] **[RECOMMANDE]**_
  - [L'IDE CLion](#clion) _[FACULTATIF] **[RECOMMANDE]**_
  - [_clang_ pour compiler](#clang-en-tant-que-compilateur) _[FACULTATIF]_
  - [Lib Wiimote](#lib-wiimote) _[FACULTATIF]_
  - [Petrilab](#petrilab) _[FACULTATIF]_
* :file_folder: [Prise en main rapide du code](#prise-en-main-rapide-du-code)
  - [Arborescence du projet](#arborescence-du-projet)
* :collision: [Problèmes de compilation connus](#problemes-de-compilation-connus)
* :art: [Lancer l'IA en mode Debug sur le simu](#lancer-lia-en-mode-debug-sur-le-simu)
* :dart: [Cross-Compilation pour Raspberry](#cross-compilation-pour-raspberry)
  - [Installation des outils ARM](#installation-des-outils-arm)
  - [Cross-compilation](#cross-compilation)
  - [Envoyer le script sur le Raspberry](#envoyer-le-script-sur-le-raspberry)

------------

# Clonage du repo
```bash
git clone https://votre_login_github@github.com/ClubRobotInsat/info.git
cd info
```

------------

#  Installation des differents outils
:rotating_light: **Etapes requises** :rotating_light:

L'installation est automatique à condition d'être sur un système Ubuntu.

## Logiciels necessaires
Installation de tous les outils nécessaires au développement
```bash
scripts/install.sh tools
```

------------

:wrench: **Etapes facultatives mais recommandées** :wrench:
## Formattage du code
Installation du _hook_, un script lancé à chaque commit pour formater le code comme il faut
```bash
scripts/install.sh format
```

## _gcc_ recent en tant que compilateur
### version 6
```bash
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt-get update
sudo apt-get install gcc-6 g++-6
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-6 60 --slave /usr/bin/g++ g++ /usr/bin/g++-6
```

### version 7
Si vous êtes sur Ubuntu 17.04 (ou plus récent) :
```bash
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
```
Sinon :
```bash
sudo add-apt-repository ppa:jonathonf/gcc-7.1
```

Puis :
```bash
sudo apt-get update
sudo apt-get install gcc-7 g++-7
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-7 60 --slave /usr/bin/g++ g++ /usr/bin/g++-7
```

## CLion
CLion est un IDE développé par JetBrains, qui est à la fois flexible, simple d'utilisation, multiplateforme et qui a la particularité d'utiliser nativement CMake, le système de build utilisé par notre code.
Il est normalement payant, mais une version d'éducation (licence valide 1 an, renouvelable tant que l'on est étudiant) est disponible.

Pour commencer, aller sur cette page ```https://www.jetbrains.com/clion/download/``` et télécharger l'IDE.

Pendant le téléchargement, aller sur la page ```https://www.jetbrains.com/shop/eform/students``` pour demander une licence éducation.
Le mail à fournir est celui de l'INSA (@etud.insa-toulouse.fr) pour que JetBrains accepte de délivrer la licence. Va voir sur ta boîte mail, un message contenant un lien de confirmation à suivre est arrivé. Une fois que l'adresse mail est confirmée, un autre mail est envoyé et il permet de recevoir les infos de la licence, qui pourront alors être saisies dans CLion.

Une fois l'IDE installé, on choisit d'ouvrir un projet existant et il faut sélectionner le dossier ```info``` résultat du clonage du Git.

Il reste à ce moment un peu de configuration à effectuer : dans le menu ```File -> Settings```, dérouler l'item ```Build, Execution, Deployment``` et sélectionner ```CMake```. Il faut ensuite remplir certains champs :
* dans ```CMake options```, mettre le texte ```-DDEBUG=on -DBITS=64```
* dans ```Generation path```, mettre le texte ```build```
* dans ```Build options```, mettre le texte ```-j 4``` (pour compiler avec 4 coeurs).

Pour pouvoir tester les IA via la laison série (RS232) en utilisant Clion (sans les droits root), il faut ajouter votre utilisateur au groupe ayant accès à la laison série (dialout) :
```bash
sudo usermod -a -G dialout NOM_UTILISATEUR
```

------------

:memo: **Etapes facultatives** :memo:
## _Clang_ en tant que compilateur
Installation d'un autre compilateur par défaut
```bash
sudo apt-get install clang-3.8
sudo update-alternatives --install /usr/bin/c++ c++ /usr/bin/clang++-3.8 100
```

## Lib Wiimote
Installation de la lib `WiiC` nécessaire pour les démos à la wiimote
```bash
scripts/install.sh wii
```

## Petrilab
Installation du logiciel d'édition de réseaux petri, qui permets de créer graphiquement des stratégies.
Systèmes d'exploitation pris en charge actuellement :
* Ubuntu 14.04 **Trusty**
* Ubuntu 16.04 **Xenial**
* Ubuntu 17.04 **Zesty**

```bash
scripts/install.sh petri
```

------------

# Prise en main rapide du code
## Arborescence du projet
![arborescence](doc/arborescence.png)

------------

# Problemes de compilation connus
- Aller dans Settings > Build, Execution, Deployment > CMake. Dans "CMake options", rajouter le paramètre `-DBITS=64`
- Problème de clock skew ? Il suffit de lancer `find -exec touch \{\} \;` dans le dossier `info`. Attention, cela peut être un petit peu long.

------------

# Lancer l'IA en mode Debug sur le simu
1. Tout compiler (choisir la target `build all`)
2. Ouvrir un premier terminal dans le dossier `info`
3. Lancer le simu :
  * _méthode simple :_ `scripts/simu_launcher.sh green`
  * _méthode personnalisée :_ ```build/src/simulateur/Simulator --robot on --world on --load build/src/table_2018.json```
4. Ouvrir un deuxième terminal et lancer Petri : `petrilab`
5. Dans Petri, charger l'IA (IA2018.petri par exemple) située dans ```/info/src/robot/Principal/petri```
6. Dans Petri encore, aller dans ```Debug -> Create Host```, normalement tout est déjà configuré, vous n'avez qu'à cliquer sur **Create host**. Sinon, il faut aller chercher les binaires de l'IA : ```/info/build/src/robot/Principal/IAPrincipal``` et indiquer comme arguments ```LOCAL --color blue SIMU```

------------

# Cross-Compilation pour Raspberry
## Installation des outils ARM
Installation des logiciels nécessaires pour cross-compiler et copie des librairies précompilées pour ARM dans le dossier `/usr/arm-linux-gnueabihf/lib`
```bash
scripts/install.sh raspi
```

## Cross-compilation
Un script build_arm.sh est fourni, il compile `IAPrincipal` et `IAWiimote` dans `/info/build_arm`.
Il est possible de donner le nombre de coeurs à utiliser au script :
```bash
 cd scripts
 ./build_arm.sh 8
```

Si il y a des problèmes de compilations, il faut commencer par supprimer le dossier `/build_arm`.

## Envoyer le script sur le Raspberry
Il faut commencer par ouvrir le gestionnaire de connections de ton ordinateur. Pour cela il faut cliquer sur l'icône "Network" dans ta barre des tâches et sélectionner "Edit Connections". Une fois dans ce menu, il est possible de définir une nouvelle connexion Ethernet qui sera partagée avec le Raspberry.

On clique donc sur "`Add`" et on choisit "`Ethernet`".
Une fois dans le menu de paramétrage de la connexion il faut modifier quelques champs :

1. Connection name : A remplir pour différencier la connection des autres
2. Onglet **Ethernet** :
  * `Device` : Normalement il n'y en a que un à chosir.
  S'il y en a plusieurs, il faut tous les essayer jusqu'à ce que ça marche.
3. Onglet **IPv4 Settings** :
  * `Method` : Choisir "Shared to other computers"

Les autres champs peuvent être laissés tel quel.

Une fois la configuration terminée dans ton gestionnaire de connexion, active la configuration créée.

Ensuite, pour pouvoir se ssh sur le RaspberryPI il suffit de lancer le script :
```bash
scripts/ssh_into_rpi.sh
```
Le mot de passe de l'utilisateur robot est "**robot**".

Pour copier des fichiers il faut lancer un autre script, avec en argument le fichier à envoyer. Celui-ci se trouvera alors sur le bureau.
```
scripts/send_file_to_rpi.sh build/robot/Principal/IAPrincipal
```
