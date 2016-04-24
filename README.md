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

git clone https://github.com/ClubRobotInsat/websimulator
```
## Problèmes fréquents : 

#### Erreur de compilation petri
Si lors de l'exécution de bootstrap.sh certaines erreurs de compilation apparaissent telles que celles ci-dessous, c'est que votre installation de mono n'est pas à jour :
```
../Sources/Code/Type.cs(119,35): error CS1525: Unexpected symbol `?'
../Sources/Document/DocumentSection/Debugger/DebugEditor.cs(69,33): error CS1525: Unexpected symbol `?'
```
La compilation des réseaux de pétri requiert mono >= 4.x.x. Si mono --version vous renvoie 3.x.x, effectuez donc la manipulation suivante :
```
sudo add-apt-repository ppa:keks9n/monodevelop-latest
sudo apt-get update
sudo apt-get install mono-devel
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
Pour lancer l'IA en mode Debug sur le simu
--------------------------
1. Tout compiler (choisir la target build all)
2. Ouvrir un premier terminal dans le dossier info, puis se placer dans le dossier ```cd ../bin/simulateur```.
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

## liste des méthodes à tester :

avancer(Distance)                                   ... Ok.
reculer(Distance)                                   ... Ok.
arreter()                                           ... Ok.
tournerRelatif(Angle)                               ... Ok.
tournerAbsolu(Angle)                                ... Ok.
allerA(Distance, Distance)                          ...
allerA(Vector2m)                                    ...
avancerA(Vector2m, SensAvance)                      ...
avancerA(Vector2m)                                  ...
reculerA(Vector2m)                                  ...
bougerObjet(std::string const&, Distance, Distance) ...
recalageMarcheArriere()                             ...
setVitesseAngulaireLente()                          ...
resetVitesseAngulaire()                             ...
setVitesseLineaireLente()                           ...
resetVitesseLineaire()                              ...
setVitesseLineaire(Speed)                           ...
descendreCuillere()                                 ...
monterCuillere()                                    ...
descendrePelle()                                    ...
monterPelle()                                       ...
sortirParasol()                                     ...
rentrerParasol()                                    ...
sortirBrasDroit()                                   ...
sortirBrasGauche()                                  ...
rentrerBrasDroit()                                  ...
rentrerBrasGauche()                                 ...
activerPompe(POSITION)                              ...
desactiverPompe(POSITION)                           ...
recupererCubes()                                    ...
relacherCubes()                                     ...
ouvrirVanne(VANNE)                                  ...
ouvrirtoutesVannes()                                ...
fermerVanne(VANNE)                                  ...
fermerToutesVannes()                                ...