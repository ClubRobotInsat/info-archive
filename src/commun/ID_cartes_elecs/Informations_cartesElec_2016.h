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

/* GROS ROBOT ; CHUCK*/
#define ID_CARTE_CHUCK_DEPLACEMENT 1
#define ID_CARTE_CHUCK_SERVOS_GAUCHE 2 // id0= bras droit  id2=bras gauche (c'est écrit sur les servos)
#define ID_CARTE_CHUCK_SERVOS_DROIT 3  // ID1=pelle arrière   ID 0= parasol
#define ID_CARTE_CHUCK_IO 4
#define ID_CARTE_CHUCK_PNEUMATIQUE_HAUT 5
#define ID_CARTE_CHUCK_PNEUMATIQUE_BAS 6
#define ID_CARTE_CHUCK_EVITEMENT 7
#define ID_CARTE_CHUCK_ELECTROVANNES 8

#define ID_SERVO_G_PINCE_DROITE 0
#define ID_SERVO_G_PINCE_GAUCHE 2
#define ID_SERVO_G_CUILLERE 3
#define ID_SERVO_D_PELLE 1
#define ID_SERVO_D_PARASOL 4


#endif /* HEADER_COMMUN */
