// Webcam2009.cpp

#include "Webcam2009.h"
#include "log/Log.h"
#include <stdio.h>
#include <stdlib.h>


// Rayons des cercles qui servent à déterminer si un palet est à une position donnée
// ou pas. Unité : pixel.
static float rayons_erreurs_palets[6] = {10.0f, 10.0f, 10.0f, 10.0f, 10.0f, 10.0f};

// Position et angle que doit avoir le robot
static PosReconnaissanceConfiguration pos_reco = {1239, 1864, -2443};

// Parametres de calibration de la webcam ( hauteur, fov ...)

// Positions sur l'image des barycentres des taches vues pas la webcam, qui représentent les palets.
static PosPaletWebcam positions_palets[6] = {{120, 120}, // Palet 0
                                             {220, 165}, // Palet 1...etc
                                             {270, 190},
                                             {42, 155},
                                             {143, 184},
                                             {195, 203}};

bool ChargerReconnaissanceConfiguration(const char* filename) {
	logInfo("chargement des parametres de reconnaissance de configuration du fichier ", filename, "...");

	FILE* f = fopen(filename, "r");
	if(!f) {
		logWarn("impossible d'ouvrir ", filename, " en lecture");
		return false;
	}

	fscanf(f,
	       "%d, %d\n"
	       "%d, %d\n"
	       "%d, %d\n"
	       "%d, %d\n"
	       "%d, %d\n"
	       "%d, %d\n",

	       &(positions_palets[0].x),
	       &(positions_palets[0].y),
	       &(positions_palets[1].x),
	       &(positions_palets[1].y),
	       &(positions_palets[2].x),
	       &(positions_palets[2].y),
	       &(positions_palets[3].x),
	       &(positions_palets[3].y),
	       &(positions_palets[4].x),
	       &(positions_palets[4].y),
	       &(positions_palets[5].x),
	       &(positions_palets[5].y));
	fclose(f);

	return true;
}

float GetRayonErreurPalet(int i) {
	return rayons_erreurs_palets[i];
}

PosReconnaissanceConfiguration GetPosReconnaissance() {
	return pos_reco;
}

PosPaletWebcam GetPositionPalet(int i) {
	return positions_palets[i];
}


// Fonction permettant la lecture d'un fichier de configuration qui contient des informations
// sur la méthode de filtrage d'une image.
bool ChoisirParametresImage(const char* filename,               // Nom du fichier de configuration
                            ImageFiltree* pimage,               // Image dont on va modifier les paramètres
                            ImageFiltree::ParamsTaches* params, // Paramètres sur les taille et les aires limites des
                                                                // taches
                            bool* psuper_flouter)               // Doit-on utiliser ImageFiltree::SuperFlouterBlanc() ?
{
	logInfo("chargement des parametres de filtrage de l'image depuis le fichier ", filename, "...");

	ColorHSV hsv_min, hsv_max;
	int super_flouter = 0;

	FILE* f = fopen(filename, "r");
	if(!f) {
		logWarn("impossible d'ouvrir ", filename, " en lecture");
		return false;
	}

	fscanf(f,
	       "hsv_min=%f,%f,%f\n"
	       "hsv_max=%f,%f,%f\n"
	       "super_flouter=%d\n"
	       "width_min=%d\n"
	       "height_min=%d\n"
	       "aire_min=%d\n"
	       "width_max=%d\n"
	       "height_max=%d\n"
	       "aire_max=%d\n",

	       &(hsv_min.h),
	       &(hsv_min.s),
	       &(hsv_min.v),
	       &(hsv_max.h),
	       &(hsv_max.s),
	       &(hsv_max.v),
	       &super_flouter,
	       &(params->width_min),
	       &(params->height_min),
	       &(params->aire_min),
	       &(params->width_max),
	       &(params->height_max),
	       &(params->aire_max));

	pimage->SetColorHSVMin(hsv_min);
	pimage->SetColorHSVMax(hsv_max);

	if(psuper_flouter != NULL)
		*psuper_flouter = (bool)super_flouter;

	fclose(f);

	return true;
}

// Ecriture dans un fichier de configuration
void EcrireParametresImage(const char* filename, const ImageFiltree& image, const ImageFiltree::ParamsTaches& params, bool super_flouter) {
	ColorHSV hsv_min = image.GetColorHSVMin();
	ColorHSV hsv_max = image.GetColorHSVMax();

	FILE* f = fopen(filename, "w");
	if(!f) {
		logWarn("impossible d'ouvrir ", filename, " en ecriture !");
		return;
	}

	fprintf(f,
	        "hsv_min=%f,%f,%f\n"
	        "hsv_max=%f,%f,%f\n"
	        "super_flouter=%d\n"
	        "width_min=%d\n"
	        "height_min=%d\n"
	        "aire_min=%d\n"
	        "width_max=%d\n"
	        "height_max=%d\n"
	        "aire_max=%d\n",

	        hsv_min.h,
	        hsv_min.s,
	        hsv_min.v,
	        hsv_max.h,
	        hsv_max.s,
	        hsv_max.v,
	        super_flouter,
	        params.width_min,
	        params.height_min,
	        params.aire_min,
	        params.width_max,
	        params.height_max,
	        params.aire_max);

	fclose(f);
}
