/**
 * @file 	mon_header.h
 * @brief 	Header commun aux codes info et elec pour partager des informations communes
 *
 * @details     Permet par exemple d'avoir dans un m�me fichier les ID des diff�rentes cartes
 *              ou bien encore leur types de messages.
 */

/**
 * Date historique de fondation de ce fichier l�gendaire : le 2 avril 2015.
 *
 */

#ifndef HEADER_COMMUN
#define HEADER_COMMUN

////////////////////////
// ID DES CARTES ELEC //
////////////////////////

/* GROS ROBOT ; INYANGA */
#define ID_CARTE_INYANGA_DEPLACEMENT 1
#define ID_CARTE_INYANGA_SERVOS 2
#define ID_CARTE_INYANGA_IO 3
#define ID_CARTE_INYANGA_EVITEMENT 4
#define ID_CARTE_INYANGA_ASCENSEUR 5


#define ID_SERVO_INYANGA_PINCE 0
#define ID_SERVO_INYANGA_LACET 1   // On fait bouger le cylindre de gauche à droite
#define ID_SERVO_INYANGA_TANGAGE 2 // On fait descendre le cylindre de haut en bas
#define ID_SERVO_INYANGA_FUSEE 3
#define ID_SERVO_INYANGA_GAUCHE 4 // servos à l'arrière pour attraper les minerais
#define ID_SERVO_INYANGE_DROITE 5

#define ID_IO_COULEUR 1
#define ID_IO_TIRETTE 2

#endif /* HEADER_COMMUN */
