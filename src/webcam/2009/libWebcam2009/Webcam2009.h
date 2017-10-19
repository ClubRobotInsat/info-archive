// Webcam2009.h

#ifndef WEBCAM_2009_H
#define WEBCAM_2009_H

#include "ImageFiltree.h"

/**
 **
 ** 	RECONNAISSANCE DE CONFIGURATION
 **
 **/


// Numérotation des taches :
/*
    ___________________________________
    |						 		  |
    |						 		  |
    |	9 10 11						  |
    |	6  7  8						  |
    |	3  4  5						  |
    |	0  1  2						  |
    |								  |
    |RR								VV|
    |RR_____________________________VV|

*/

// Structure représentant une position sur l'image de la webcam (en pixels) du barycentre
// de la tache correspondant à un palet.
struct PosPaletWebcam {
	int x, y;
};

struct PosReconnaissanceConfiguration {
	short x;
	short y;
	short angle;
};

bool ChargerReconnaissanceConfiguration(const char* filename);

float GetRayonErreurPalet(int i);
PosReconnaissanceConfiguration GetPosReconnaissance();
PosPaletWebcam GetPositionPalet(int i);

/**
 **
 ** 	FILTRAGE
 **
 **/

// Fonction permettant la lecture d'un fichier de configuration qui contient des informations
// sur la méthode de filtrage d'une image.
bool ChoisirParametresImage(const char* filename,               // Nom du fichier de configuration
                            ImageFiltree* pimage,               // Image dont on va modifier les paramètres
                            ImageFiltree::ParamsTaches* params, // Paramètres sur les taille et les aires limites des
                                                                // taches
                            bool* psuper_flouter);              // Doit-on utiliser ImageFiltree::SuperFlouterBlanc() ?

// Ecriture dans un fichier de configuration
void EcrireParametresImage(const char* filename, const ImageFiltree& pimage, const ImageFiltree::ParamsTaches& params, bool super_flouter);

#endif // WEBCAM_2009_H
