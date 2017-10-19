// Webcam2010.h

#ifndef WEBCAM_2010_H
#define WEBCAM_2010_H

#include "../../../commun/2010/Constantes2010.h"
#include "CalibrageWebcam.h"
#include "ImageFiltree.h"
#include "Webcam.h"

// Nombre de configurations
#define NB_CONFIG_MAIS_LATERAUX 9
#define NB_CONFIG_MAIS_CENTRAUX 4

// Numerotation des mais :
/*
  Mais lateraux :
  ________________________________
  |BB                          JJ|
  |BB                          JJ|
  |                              |
  |  0                           |
  |     3                        |
  |  1     5                     |
  |     4     X                  |
  |  2     X                     |
  |     X     X                  |
  |______________________________|


  Mais centraux :
  ________________________________
  |BB                          JJ|
  |BB                          JJ|
  |                              |
  |  X                           |
  |     X                        |
  |  X     X                     |
  |     X     2                  |
  |  X     1                     |
  |     0     3                  |
  |______________________________|

*/

enum CouleurMais { MAIS_NOIR = 1, MAIS_BLANC = 0, MAIS_UNDEF = -1 };

class Webcam2010 {
private:
	IWebcam* webcam;
	ImageFiltree imageNoir;    // Pour les mais noir
	ImageFiltree imageBlanc;   // Pour les mais blancs
	CalibrageWebcam calibrage; // Les paramètres que l'on veut lire / écrire
	CouleurRobot couleurEquipe;

	unsigned int numConfigLateraux;   // numero de la configuration detectee pour les mais lateraux
	unsigned int numConfigCentraux;   // numero de la configuration detectee pour les mais centraux
	unsigned int scoreConfigLateraux; // score associe a la configuration de mais lateraux
	unsigned int scoreConfigCentraux; // score associe a la configuration de mais centraux

	// Ces tableaux contiennent une valeur de couleur (1, 0 ou -1, resp. MAIS_NOIR, MAIS_BLANC et MAIS_UNDEF)
	// pour chaque mais.
	CouleurMais configDetecteeLateraux[NB_MAIS_LATERAUX];
	CouleurMais configDetecteeCentraux[NB_MAIS_CENTRAUX];

	string configFilename;

	// Renvoie la distance entre le point donné et le barycentre de la tache à laquelle il appartient,
	// ou -1.0f si le point donné n'appartient à aucune tache.
	// Si le point appartient à plusieurs taches, on renvoie la plus petite distance.
	float GetCarreDistanceAuBarycentre(const ImageFiltree* image, int x, int y) const;

	// Mise à jour des paramètres de filtrage de imageNoir et imageBlanc
	void UpdateParamsImages();

public:
	// Constructeur
	Webcam2010();

	// Destructeur
	virtual ~Webcam2010();

	// Initialisation de la webcam 2010.
	// Si forceConfigFile est à true, dans le cas où une webcam est demandée
	// (i.e. il y a UVC, V4L, TCPCAM...etc dans les arguments), on quitte si un fichier
	// de configuration n'est pas précisé avec "CAMCONF fichier.conf".
	bool Init(int argc, char* argv[], bool forceConfigFile = true);

	// Getters :
	const IWebcam* GetWebcam() const {
		return webcam;
	}

	const ImageFiltree* GetImageNoir() const {
		return &imageNoir;
	}
	ImageFiltree* GetImageNoir() {
		return &imageNoir;
	}

	const ImageFiltree* GetImageBlanc() const {
		return &imageBlanc;
	}
	ImageFiltree* GetImageBlanc() {
		return &imageBlanc;
	}

	const CalibrageWebcam* GetCalibrage() const {
		return &calibrage;
	}

	// Setter pour le calibrage :
	void SetCalibrage(const CalibrageWebcam& calibrage);

	// Envoie une requete a la webcam pour recuperer une image, recoit l'image, la SuperFloute et detecte les taches.
	// Appelée par ExtraireConfigurations().
	bool AcquisitionEtTraitement();

	// Fonction qui effectue l'acquisition d'une image et determine la configuration du reglement
	// la plus approchante de celle qui a ete detectee.
	bool ExtraireConfigurations();

	// Getters sur les numeros de configurations et les scores associes
	const unsigned int GetConfigLateraux() const {
		return numConfigLateraux;
	}
	const unsigned int GetConfigCentraux() const {
		return numConfigCentraux;
	}
	const unsigned int GetScoreConfigLateraux() const {
		return scoreConfigLateraux;
	}
	const unsigned int GetScoreConfigCentraux() const {
		return scoreConfigCentraux;
	}

	// recupere le nom du fichier de config qui a ete passe en parametre de la lignes de commmande
	string getConfigFilename() const {
		return configFilename;
	}
};

#endif // WEBCAM_2010_H
