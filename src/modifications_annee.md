# Brief
Ce fichier permets de synthétiser toutes les modifications à apporter au code pour passer de l'année `N` à l'année `N+1`.

# Modification des IDs des cartes et des servos
## Communication élec-info
Le premier fichier à modifier permets de gérer les IDs des cartes et des servos ; ce fichier est en commun avec les électroniciens.

```bash
cd <dossier info>/../CodeCommun
cp Informations_cartesElec_2017.h Informations_cartesElec_2018.h
```

Puis mettre à jour les différentes constantes en fonction de l'année en cours.

## Mise à jour du code info
### Update des constantes élec
* Aller dans `/info/src/commun/RobotPrincipal/Constantes.h` puis modifier l'include du fichier de CodeCommun: `#include "../../../../CodeCommun/Informations_cartesElec_2018.h"`
* Modifier l'`enum` des cartes avec les nouvelles IDs
* Modifier l'`ENUM_NS` **IDCartes** _(elle devrait ne pas trop bouger entre les années depuis l'utilisation d'une unique carte mère)_
* Modifier l'`ENUM_CLASS_NS` **IDCartesServo** en mettant les noms de servos spécifiques
* Mettre à jour la partie `ConstantesPrincipal` :
    - La partie `CONTACTEURS` devrait rester identique
    - Dans la partie `SERVOS` :
        - modifier l'`enum class Servo` en rajoutant tous les IDs partagés avec les élecs
        - rajouter une `enum class` par servo pour donner toutes les positions de chaque servo (avec un champ `NBR`)
        - créer les `extern Angle const positionBLABLA[enumToInt(BLABLA::NBR)]`
    - Dans la partie `MOTEURS` :
        - rajouter une `enum class` par moteur pour les positions
        - créer les `extern Angle const positionBLABLA[enumToInt(BLABLA::NBR)]`
        - Si on a **besoin d'un ascenseur** (avec poulie) :
            - il faut nommer l'`enum class PositionAscenseur` _(utilisée dans `/info/src/Robot/Principal/librobot/Ascenseur.h`)_
            - dans `/info/src/robot/Commun/CMakeLists.txt`, il faut rajouter `librobot/Ascnseur.cpp` dans le `set(ROBOT_PRINC_SOURCE)`
* Mettre à jour la partie `CARTES`
* Définir les valeurs angulaires de toutes les positions dans `/info/src/commun/RobotPrincipal/Constantes.cpp`
* Aller dans `/info/src/simulateur/communication/modules/ModuleContactor.cpp` et updater l'**ID_CARTE_IO** en fonction de l'année

### Update du robot dans le simulateur
```bash
cd /info/src/simulateur/communication
cp Robot2017.h Robot2018.h
cp Robot2017.cpp Robot2018.cpp
```

* Modifier le `/info/src/simulateur/CMakeLists.txt` pour changer le nom du robot de l'année et reload le projet CMake
* Update du `Robot2018.[h|cpp]` et rajouter les modules si nécessaire
* Changer les pré-définitions de la `class Robot2018` et le nom de l'objet dans les fichiers suivants :
    - `/info/src/simulateur/communication/modules/ModuleContactor.[h|cpp]`
    - `/info/src/simulateur/communication/modules/ModulePneumatique.[h|cpp]`
    - `/info/src/simulateur/communication/modules/ModuleServos.[h|cpp]`
    - `/info/src/simulateur/communication/modules/ModuleValve.[h|cpp]`

### Update des couleurs
Modifier l'`ENUM_CLASS_NS` avec les couleurs de l'année dans le fichier `/info/src/commun/ConstantesCommunces.h`, compiler et remplacer les couleurs partout où nécessaire (IAs, simu, parseur JSON, ...)
