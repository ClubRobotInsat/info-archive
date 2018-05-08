@[Benjamin BIGEY](https://github.com/Terae) - 22 avril 2018

@[Paul FLORENCE](mailto::perso@florencepaul.com) - Avril/Mai 2018

# Documentations des cartes de [ChocoBot'](https://scontent-cdg2-1.xx.fbcdn.net/v/t1.0-9/30516417_1028608243958446_3203690177092386816_n.png?_nc_cat=0&oh=e480ed380092c42ebe33e2baf028efa8&oe=5B99AE77)

## Sommaire et liens
* :memo: Fichiers source
	- Côté info : [Cartes](https://github.com/ClubRobotInsat/info/tree/propre/src/robot/Cartes)
		- CarteDeplacement2009 : [.h](https://github.com/ClubRobotInsat/info/blob/propre/src/robot/Cartes/CarteDeplacement2009.h) et [.cpp](https://github.com/ClubRobotInsat/info/blob/propre/src/robot/Cartes/CarteDeplacement2009.cpp)
		- CarteServosNova2017 : [.h](https://github.com/ClubRobotInsat/info/blob/master/src/robot/Cartes/CarteServosNova2017.h) et [.cpp](https://github.com/ClubRobotInsat/info/blob/master/src/robot/Cartes/CarteServosNova2017.cpp)
		- CarteMoteur2018 : [.h](https://github.com/ClubRobotInsat/info/blob/propre/src/robot/Cartes/CarteMoteur2018.h) et [.cpp](https://github.com/ClubRobotInsat/info/blob/propre/src/robot/Cartes/CarteMoteur2018.cpp) ; ***TODO***
	- Côté élecs : [code-elec-nucleo/master](https://github.com/ClubRobotInsat/code-elec-nucleo/tree/master/src)
		- [Brushless.h](https://github.com/ClubRobotInsat/code-elec-nucleo/blob/master/src/Brushless.h)
		- [Motor.h](https://github.com/ClubRobotInsat/code-elec-nucleo/blob/master/src/Motor.h)
		- [Servo.h](https://github.com/ClubRobotInsat/code-elec-nucleo/blob/master/src/Servo.h)
* :clipboard: [Attribution des IDs aux cartes](#attribution-des-ids-aux-cartes)
* :clipboard: [Attribution des IDs aux servos](#attribution-des-ids-aux-servos)
* :clipboard: [Attribution des IDs aux moteurs](#attribution-des-ids-aux-moteurs)
* Specs sur les différentes cartes
	- :truck: [Carte déplacement](#carte-deplacement)
		- [IDs des messages](#ids-des-messages)
		- [Détail de certaines commandes](#detail-de-certaines-commandes)
		- [Définition des types énumérés](#definition-des-types-enumeres)
	- :arrow_upper_right: [Carte servos](#carte-servos)
	- :radio_button: [Carte IO](#carte-io)
	- :muscle: [Carte moteurs](#carte-moteur)

## Attribution des IDs aux cartes
Ce choix est arbitraire, il faut juste garantir que la carte déplacement ait l'ID `0x01`.

Pour modifier ces constantes, il faut aller voir du côté [de ce document](https://github.com/ClubRobotInsat/info/blob/propre/src/commun/ID_cartes_elecs/Informations_cartesElec_2018.h).

|  ID  |    Carte    | Commande |
|:----:|:-----------:|:--------:|
| 0x01 | Déplacement |   _CAN_  |
| 0x02 |    Servos   |  Nucléo  |
| 0x03 |      IO     |  Nucléo  |
| 0x04 |  Evitement  |   _CAN_  |
| 0x05 |    Moteur   |  Nucléo  |

## Attribution des IDs aux servos
|  ID  |        Servo       |
|:----:|:------------------:|
| 0x00 | Porte soute droite |
| 0x01 |       Loquet       |
| 0x02 | Porte soute gauche |
| 0x03 |   Abeille gauche   |
| 0x04 |   Abeille droite   |

## Attribution des IDs aux moteurs
|  ID  |                Moteur               |
|:----:|:-----------------------------------:|
| 0x01 |        Moteur asservi gauche        |
| 0x02 |        Moteur asservi droite        |
| 0x03 | Moteur non asservi gauche (avaleur) |
| 0x04 | Moteur non asservi droite (avaleur) |
| 0x05 |    Brushless gauche (ventilateur)   |
| 0x06 |    Brushless droite (ventilateur)   |

## Carte deplacement
### Commandes des messages
| CMD |      Type     |    E/R    |                  Commentaires                 | Nbr données (octets) |
|:--:|:-------------:|:---------:|:---------------------------------------------:|:--------------------:|
|  0 |      PING     |           |                                               |                      |
|  1 |      ACK      | réception |                    TypeAck                    |           1          |
|  2 |  SET_VARIABLE |   envoi   |          uint8_t param + float value          |           5          |
|  3 | DEFINIR_COORD |   envoi   |                                               |           6          |
|  4 |    COMMANDE   |   envoi   |                  _cf détails_                 |        dépend        |
|  5 |  ARRETURGENCE |           |          arrêt géré par **COMMANDE**          |                      |
|  6 | ASSERV_ON_OFF |   envoi   |         `TypeGrandeur` + (bool active)        |           2          |
|  7 |   ENVOI_AUTO  |   envoi   |                  bool on_off                  |           1          |
|  8 |      LED      |           |            gérées par **COMMANDE**            |                      |
|  9 |     BLOQUE    | réception |                                               |           0          |
| 10 |     COORD     | réception |                  _cf détails_                 |           6          |
| 11 |    TERMINE    | réception |                  _cf détails_                 |       0, 1 ou 2      |
| 12 |     MESURE    |    E+R    |                  _cf détails_                 |        dépend        |
| 13 |  GET_VARIABLE |    E+R    | E: demande de la valeur, R: réception (float) |     E : 0, R : 4     |
| 14 |       -       |           |                                               |                      |
| 15 |     RESET     |   envoi   |     redémarre la carte par l'envoi de 0xFF    |           1          |

### Detail de certaines commandes
#### 4 - _COMMANDE_
Cette instruction envoie un `TypeDeplacement` sur le premier octet puis des paramètres en fonction de la nature du déplacement.

Type de déplacement et paramètres des fonctions d'appel de l'instruction :

* `arreter()` : **DEPLACEMENT_ARRETER**, aucun paramètre
* `avancer(Distance)` : **DEPLACEMENT_AVANCER**, distance en mm (sur int16_t)
* `tourner(Angle)` : **DEPLACEMENT_TOURNER**, angle en milliRad sur [-π;π] (sur int16_t)
* `tournerAbsolu(Angle, SensRotation)` : **DEPLACEMENT_TOURNER_ABSOLU**, angle en milliRad sur [-π;π] (sur int16_t), sens (`SensRotation`)
* `pointerVers(Coordonnees, SensRotation)` : **DEPLACEMENT_POINTER_VERS**, positionX en mm (sur int16_t), positionY en mm (sur int16_t), sens (`SensRotation`)
* `vaVers(Distance, Distance, SensAvance)` : **DEPLACEMENT_ALLER_VERS**, positionX en mm (sur int16_t), positionY en mm (sur int16_t), sens (`SensAvance`)
* `allerADecompose(Distance, Distance, SensAvance)` : **DEPLACEMENT_ALLER_A_DECOMPOSE**, positionX en mm (sur int16_t), positionY en mm (sur int16_t), sens (`SensAvance`)
* `allerA(Distance, Distance, SensAvance)` : **DEPLACEMENT_ALLER_A**, positionX en mm (sur int16_t), positionY en mm (sur int16_t), sens (`SensAvance`) [[DEPRECATED]]
* `avancerInfini(Speed, SensAvance)` : **DEPLACEMENT_AVANCER_INFINI**, vitesse en mm/s (sur int16_t), sens (`SensAvance`)
* `tournerInfini(AngularSpeed, SensRotation)` : **DEPLACEMENT_TOURNER_INFINI**, vitesse en milliRad/s (sur int16_t), sens (`SensRotation`)
* `passerPar(Coordonnees, Speed, SensAvance)` : **DEPLACEMENT_PASSER_PAR**, positionX en mm (sur int16_t), positionY en mm (sur int16_t), vitesse en mm/s (sur int16_t), sens (`SensAvance`)
* `pivoterDroite(Angle, SensRotation)` : **DEPLACEMENT_PIVOTER_DROITE**, angle en milliRad sur [-π;π] (sur int16_t), sens (`SensRotation`)
* `pivoterGauche(Angle, SensRotation)` : **DEPLACEMENT_PIVOTER_GAUCHE**, angle en milliRad sur [-π;π] (sur int16_t), sens (`SensRotation`)
* `positionDroiteGauche(Distance, Distance, SensAvance)` : **DEPLACEMENT_DISTANCE_DG**, distDroite en mm (sur int16_t), distGauche en mm (sur int16_t), sens (`SensAvance`)
* `vitesseDroiteGauche(Speed, Speed, SensAvance)` : **DEPLACEMENT_VITESSE_DG**, vitesseDroite en mm/s (sur int16_t), vitesseGauche en mm/s (sur int16_t), sens (`SensAvance`)
* `arretUrgence()` : **DEPLACEMENT_ARRET_URGENCE**
* `controleVitesseLongAng(Speed, AngularSpeed)` : **DEPLACEMENT_CONTROLE_VITESSE**, vitesseLongitudinale en mm/s (sur int16_t), vitesseAngulaire en milliRad/s (sur int16_t)
* `allumerLed(bool, bool)` : led1, led2 _[il faudrait le gérer par la commande **LED**]_

#### 10 - _COORD_
Reçoit 6 octets :
* 2 octets pour la position X (en mm)
* 2 octets pour la position Y (en mm)
* 2 octets pour l'angle (en milli radians)

#### 11 - _TERMINE_
* Si **1 octet** reçu : _Déplacement terminé_ <=> précision atteinte + vitesse longitudinale du robot = 0.0
* Si **2 octets** reçus : _Précision atteinte_ : le robot est dans un rectangle de `Mouvement_PrecisionLongitudinale * Mouvement_PrecisionLongitudinale` (ou angulaire) autour de la cible
* Sinon : _Déplacement terminé_ et _Précision atteinte_

#### 12 - _MESURE_
* utilisé en **réception** :
	- réception de 4 données std::int16_t (octets 1, 3, 5, 7)
	- uniquement utilisé pour lire les données en débug (jamais appelé côté info)
* utilisé en **envoi** :
	- Envoi de la grandeur de type `TypeGrandeur` (1 octet)

### Definition des types enumeres
Utilisée par la commande **ACK** (n°1)
```cpp
enum TypeAck : uint8_t {
	ACK_RIEN,
	ACK_MESSAGE_EXECUTE = 'R',
	ACK_MESSAGE_IGNORE  = 'I'
};
```

```cpp
enum TypeMesure : uint8_t {
	MESURE_POSITION         = 0,
	MESURE_VITESSE_CONSIGNE = 1,
	MESURE_VITESSE_MESUREE  = 2,
	MESURE_COMMANDE         = 3
};
```

```cpp
enum TypeParam : uint8_t {
	PID       = 0,
	CINETIQUE = 1,
	PRECISION = 2,
	MECANIQUE = 3
};
```

Utilisée par les commandes **ASSERV_ON_OFF** (n°6) et **MESURE** (n°12)
```cpp
enum TypeGrandeur : uint8_t {
	POSITION  = 'L',
	ANGLE     = 'A',
	DROITE    = 'D',
	GAUCHE    = 'G',
	VPOSITION = 'l',
	VANGLE    = 'a'
};
```

Utilisée par la commande **COMMANDE** (n°4)
```cpp
enum TypeDeplacement : uint8_t {
	DEPLACEMENT_RIEN,
	DEPLACEMENT_ARRETER           = 0,
	DEPLACEMENT_AVANCER           = 1,
	DEPLACEMENT_TOURNER           = 2,
	DEPLACEMENT_TOURNER_ABSOLU    = 3,
	DEPLACEMENT_POINTER_VERS      = 4,
	DEPLACEMENT_ALLER_VERS        = 5,
	DEPLACEMENT_ALLER_A_DECOMPOSE = 6,
	DEPLACEMENT_ALLER_A           = 7,
	DEPLACEMENT_AVANCER_INFINI    = 8,
	DEPLACEMENT_TOURNER_INFINI    = 9,
	DEPLACEMENT_PASSER_PAR        = 10,
	DEPLACEMENT_PIVOTER_DROITE    = 11,
	DEPLACEMENT_PIVOTER_GAUCHE    = 12,
	DEPLACEMENT_DISTANCE_DG       = 13,
	DEPLACEMENT_VITESSE_DG        = 14,
	DEPLACEMENT_ARRET_URGENCE     = 15,
	DEPLACEMENT_CONTROLE_VITESSE  = 17
};
```

Utilisée par la commande **COMMANDE** (n°4)
```cpp
enum class SensAvance : uint8_t {
	Arriere = 0,
	Avant   = 1
};
```

Utilisée par la commande **COMMANDE** (n°4)
```cpp
enum class SensRotation : uint8_t {
	Horaire = 0,
	Trigo   = 1
};
```

## Carte servos

Les IDS sont toujours sur 1 octet interprété en non signé.

## Trames CAN -> INFO

#### Position

|  Commande  |  Data  |
|:----:|:----:|
|0x02 | Angle sur 2 octets entre 0 et 1023 |

#### Servo bloqué

|  Commande  |  Data  |
|----|----|
|0x04 | ID du servo + Booléen représentant un servo bloqué |

#### Mouvement terminé

|  Commande  |  Data  |
|:----:|:----:|
|0x03 | ID du servo + Booléen représentant un servo qui a fini son mouvement|

## Trames INFO -> CAN

#### Récuperer position

|  Commande  |  Data  | Commentaires |
|:----:|:----:|:----:|
| 0x06 | Id du servo | La carte renvois un message [Position](#position) |

#### Mode Blocage :
***N'existe plus***

#### Réglage de la vitesse :

***N'existe plus***

#### Réglage de la position :
Format de la trame :

|  Commande  |  Data  | Commentaires |
|:----:|:----:|:----:|
| 0x05 | ID du servo + Angle sur 2 octets | L'angle est entre 0 et 1023 |

## Carte IO
Cette carte n'est utilisée que pour savoir si la tirette a été retirée ou non.

|  Cmd  |        Type        |  Commentaires  |
|:----:|:------------------:|:--------------:|
| 0x01 | Lecture contacteur | Réponds 0 ou 1 |

## Carte moteur
|  Cmd |      Type      | Moteur asservi ? |           Commentaires           | Nbr données (octets) |
|:----:|:--------------:|:----------------:|:--------------------------------:|:--------------------:|
| 0x01 | Position ANGLE |        oui       | ID \| angle sur [0;360]° : float |           5          |
| 0x02 |  Position TOUR |        oui       | ID \| nbr tours \| sens (0 ou 1) |           3          |
| 0x03 |       ON       |        non       | ID \| sens rotation (0 ou 1)     |           2          |
| 0x04 |       OFF      |        non       | ID                               |           1          |
| 0x05 |  ON Brushless  |        non       | ID                               |           1          |
| 0x06 | Position Terminée |     oui       | _cf détails_                     |           1          |

### Position des moteurs asservis
La lecture de la position se fait en trois temps :
* la fonction `actualiserPosition(uint8_t moteur)` demande à la carte élec si le déplacement est fini (enfin d'1 octet pour l'ID) avec la commande `0x06`
* l'élec retourne l'ID du moteur concerné et un uint8_t pour dire si la position est atteinte (2 octets)
* il faut enfin appeler la fonction `lirePosition(uint8_t moteur)` pour savoir si la position est atteinte côté info (l'info est stockée en interne)

Pour le sens de rotation, `0` correspond à une rotation **Horaire** et `1` à une rotation **Trigonométrique**.
