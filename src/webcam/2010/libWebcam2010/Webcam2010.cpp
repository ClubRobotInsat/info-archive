// Webcam2010.cpp

#include "Webcam2010.h"
#include "StaticAssert.h"
#include "log/Log.h"
#include <stdlib.h>
#include <string.h>
using namespace std;

// Configurations du reglement :
#define N MAIS_NOIR  // 1 = mais noir
#define b MAIS_BLANC // 0 = mais blanc

// Quelques assertions :
struct __ValidateWebcamConstants2010__ {
	__ValidateWebcamConstants2010__() {
		STATIC_ASSERT(NB_CONFIG_MAIS_LATERAUX == 9);
		STATIC_ASSERT(NB_MAIS_LATERAUX == 6);

		STATIC_ASSERT(NB_CONFIG_MAIS_CENTRAUX == 4);
		STATIC_ASSERT(NB_MAIS_CENTRAUX == 4);
	}
};

// Configurations : configMaisLateraux[i] correspond à la carte i+1 (sur le règlement) pour les mais lateraux.
static CouleurMais configMaisLateraux[NB_CONFIG_MAIS_LATERAUX][NB_MAIS_LATERAUX] = {
    //  0  1  2  3  4  5  -> positions, numeros dans les dessins de Webcam2010.h
    {b, N, b, b, N, b}, // configuration 0
    {N, b, b, b, N, b}, // configuration 1
    {b, b, N, b, N, b}, // etc
    {b, b, N, N, b, b},
    {N, b, b, N, b, b},
    {b, N, b, N, b, b},
    {b, N, b, b, b, N},
    {N, b, b, b, b, N},
    {b, b, N, b, b, N} // configuration 8
};

static CouleurMais configMaisCentraux[NB_CONFIG_MAIS_CENTRAUX][NB_MAIS_CENTRAUX] = {
    //  0  1  2  3  -> positions, numeros dans les dessins ci-dessus
    {N, b, N, b},
    {b, N, N, b},
    {N, b, b, N},
    {b, N, b, N}};

#undef N
#undef b

// ---------------------------------------------------------------------

Webcam2010::Webcam2010()
        : webcam(NULL)
        , numConfigLateraux(NB_CONFIG_MAIS_LATERAUX)
        , numConfigCentraux(NB_CONFIG_MAIS_CENTRAUX)
        , scoreConfigLateraux(0)
        , scoreConfigCentraux(0) {
	for(int i = 0; i < NB_MAIS_LATERAUX; i++)
		configDetecteeLateraux[i] = MAIS_UNDEF;

	for(int i = 0; i < NB_MAIS_CENTRAUX; i++)
		configDetecteeCentraux[i] = MAIS_UNDEF;
}

Webcam2010::~Webcam2010() {
	if(webcam != NULL)
		webcam->Close();

	delete webcam;
}

// Initialisation de la webcam 2010.
// Si forceConfigFile est à true, dans le cas où une webcam est demandée
// (i.e. il y a UVC, V4L, TCPCAM...etc dans les arguments), on quitte si un fichier
// de configuration n'est pas précisé avec "CAMCONF fichier.conf".
bool Webcam2010::Init(int argc, char* argv[], bool forceConfigFile) {
	logInfo("Initialisation de la webcam 2010...");

	configFilename = "";

	// Création de la webcam :
	webcam = NewWebcam(argc, argv, false);

	if(!webcam)
		return false;

	int w = webcam->GetWidth();
	int h = webcam->GetHeight();

	// Initialisation des images filtrées :
	imageNoir.Init(w, h);
	imageBlanc.Init(w, h);

	// --------- Lecture du fichier de configuration : ---------------
	// - recherche de "CAMCONF fichier.conf" dans la ligne de commande :
	for(int i = 1; i < argc; i++) {
		string s = argv[i];
		if(s == "CAMCONF") {
			if(i == argc - 1) {
				cout << "Utilisation : " << argv[0] << "[autres arguments] CAMCONF fichier.conf" << endl;
				return false;
			}
			configFilename = argv[i + 1];
			break;
		}
	}

	// - lecture du fichier de calibrage :
	if(configFilename == "") {
		if(forceConfigFile) {
			logError("pas de fichier de calibrage webcam precise, on quitte.\n"
			         "Pour preciser un fichier de calibrage webcam,\n"
			         "precisez \"CAMCONF fichier.conf\" dans les arguments");
			exit(EXIT_FAILURE);
		} else {
			logInfo("pas de fichier de calibrage webcam precise, utilisation des valeurs par défaut");
			calibrage.Print();
		}
	} else {
		if(!calibrage.Lire(configFilename.c_str(), w, h))
			return false;
	}

	// - copie des paramètres de calibrage dans les images filtrées :
	UpdateParamsImages();

	couleurEquipe = COULEUR_ROBOT_BLEU;

	for(int i = 0; i < 5; i++) {
		// On demande à la webcam de récupérer l'image
		if(!webcam->QueryImage()) {
			fprintf(stderr, "Erreur lors de la demande au périphérique d'envoi d'une nouvelle image\n");
			return false;
		}

		// On attend que l'image arrive
		if(!webcam->WaitForImageSync()) {
			fprintf(stderr, "Erreur lors de la capture d'une frame par la webcam\n");
			return false;
		}
	}
	return true;
}

// Renvoie la distance entre le point donné et le barycentre de la tache à laquelle il appartient,
// ou -1.0f si le point donné n'appartient à aucune tache.
// Si le point appartient à plusieurs taches, on renvoie la plus petite distance.
float Webcam2010::GetCarreDistanceAuBarycentre(const ImageFiltree* image, int x, int y) const {
	const list<ImageFiltree::Tache>& taches = image->GetListeTaches();
	bool found = false;
	float squareDistClosest = -1.0f;

	list<ImageFiltree::Tache>::const_iterator it_end = taches.end();

	for(list<ImageFiltree::Tache>::const_iterator it = taches.begin(); it != it_end; it++) {
		const ImageFiltree::Tache& tache = (*it);
		if((x >= tache.x) && (x <= tache.x + tache.w) && (y >= tache.y) && (y <= tache.y + tache.h)) {
			float dx = float(x - tache.x_barycentre);
			float dy = float(y - tache.y_barycentre);

			float squareDist = dx * dx + dy * dy;

			if(!found || squareDistClosest > squareDist)
				squareDistClosest = squareDist;

			found = true;
		}
	}

	return found ? squareDistClosest : -1.0f;
}

// Mise à jour des paramètres de filtrage de imageNoir et imageBlanc
void Webcam2010::UpdateParamsImages() {
	Color noirMin, noirMax;
	Color blancMin, blancMax;

	calibrage.GetCouleurNoirMinMax(&noirMin, &noirMax);
	imageNoir.SetColorMin(noirMin);
	imageNoir.SetColorMax(noirMax);

	calibrage.GetCouleurBlancMinMax(&blancMin, &blancMax);
	imageBlanc.SetColorMin(blancMin);
	imageBlanc.SetColorMax(blancMax);
}

void Webcam2010::SetCalibrage(const CalibrageWebcam& calibrage) {
	this->calibrage = calibrage;

	// Mise à jour des paramètres des images filtrées :
	UpdateParamsImages();
}

// Envoie une requete a la webcam pour recupere une image, recoit l'image, la SuperFloute et detecte les taches
bool Webcam2010::AcquisitionEtTraitement() {
	// On demande à la webcam de récupérer l'image
	if(!webcam->QueryImage()) {
		fprintf(stderr, "Erreur lors de la demande au périphérique d'envoi d'une nouvelle image\n");
		return false;
	}

	// On attend que l'image arrive
	if(!webcam->WaitForImageSync()) {
		fprintf(stderr, "Erreur lors de la capture d'une frame par la webcam\n");
		return false;
	}

	// Flipping horizontal de l'image s'il le faut :
	int camWidth = webcam->GetWidth();
	int camHeight = webcam->GetHeight();
	unsigned char* data = webcam->GetData();
	if(calibrage.IsFlipHorizontal()) {
		for(int y = 0; y < camHeight; y++) {
			for(int x = 0; x < camWidth / 2; x++) {
				int cx = camWidth - 1 - x;
				int i = x + y * camWidth;
				int ci = cx + y * camWidth;

				unsigned char rTemp = data[i * 3 + 0];
				unsigned char gTemp = data[i * 3 + 1];
				unsigned char bTemp = data[i * 3 + 2];

				data[i * 3 + 0] = data[ci * 3 + 0];
				data[i * 3 + 1] = data[ci * 3 + 1];
				data[i * 3 + 2] = data[ci * 3 + 2];

				data[ci * 3 + 0] = rTemp;
				data[ci * 3 + 1] = gTemp;
				data[ci * 3 + 2] = bTemp;
			}
		}
	}

	// Préparation d'une structure paramsTaches à partir des valeurs du calibrage :
	ImageFiltree::ParamsTaches paramsTaches;
	paramsTaches.width_min = calibrage.widthMin;
	paramsTaches.height_min = calibrage.heightMin;
	paramsTaches.aire_min = calibrage.widthMin * calibrage.heightMin;

	paramsTaches.width_max = calibrage.widthMax;
	paramsTaches.height_max = calibrage.heightMax;
	paramsTaches.aire_max = calibrage.widthMax * calibrage.heightMax;

	// Binarisation, floutage, application du masque, repérage des taches
	imageNoir.UpdateFromData(webcam->GetData());
	imageNoir.SuperFlouterBlanc();
	imageNoir.ApplyMaskRGB(calibrage.GetMaskDataRGB());
	imageNoir.RepererTaches(paramsTaches);

	imageBlanc.UpdateFromData(webcam->GetData());
	imageBlanc.SuperFlouterBlanc();
	imageBlanc.ApplyMaskRGB(calibrage.GetMaskDataRGB());
	imageBlanc.RepererTaches(paramsTaches);

	return true;
}

// Fonction qui effectue l'acquisition d'une image et determine la configuration du reglement la plus approchante de
// celle qui a ete detectee
bool Webcam2010::ExtraireConfigurations() {
	// Prise d'une photo et traitement floutage + detection des taches
	if(!AcquisitionEtTraitement())
		return false;

	cout << "-------------------------" << endl;
	cout << "Extraction de la configuration a la webcam :" << endl;
	cout << imageNoir.GetListeTaches().size() << " tache(s) image noir" << endl;
	cout << imageBlanc.GetListeTaches().size() << " tache(s) image blanc" << endl;

	// On choisit la précision d'affichage des nombres flottants pour pouvoir faire un joli
	// tableau dans la console :)
	streamsize precision = cout.precision();
	cout.precision(strlen("score noir"));

	// Reset des configs et des scores associes aux configurations detectees et qui ne servent qu'a differencier deux
	// configs possibles a un instant donne
	scoreConfigLateraux = 0;
	scoreConfigCentraux = 0;
	numConfigLateraux = NB_CONFIG_MAIS_LATERAUX;
	numConfigCentraux = NB_CONFIG_MAIS_CENTRAUX;

	// Première itération : mais latéraux
	// Deuxième itération : mais centraux
	const char* tabMsg[2] = {"lateraux", "centraux"};
	const int tabNbMais[2] = {NB_MAIS_LATERAUX, NB_MAIS_CENTRAUX};
	const int tabNbConfig[2] = {NB_CONFIG_MAIS_LATERAUX, NB_CONFIG_MAIS_CENTRAUX};
	Vector2i* tabPosMais[2] = {calibrage.maisLateraux, calibrage.maisCentraux};
	CouleurMais* tabConfigDetectee[2] = {configDetecteeLateraux, configDetecteeCentraux};
	unsigned int* tabNumConfig[2] = {&numConfigLateraux, &numConfigCentraux};
	unsigned int* tabScoreConfig[2] = {&scoreConfigLateraux, &scoreConfigCentraux};

	for(int iter = 0; iter < 2; iter++) {
		const char* msg = tabMsg[iter];
		const int nbMais = tabNbMais[iter];
		const int nbConfig = tabNbConfig[iter];
		Vector2i* posMais = tabPosMais[iter];
		CouleurMais* configDetectee = tabConfigDetectee[iter];
		unsigned int& numConfig = *tabNumConfig[iter];
		unsigned int& scoreConfig = *tabScoreConfig[iter];

		cout << "Mais " << msg << " : " << endl;
		cout << "score noir | score blanc" << endl;

		// Détermination des couleurs des mais, i.e.
		// remplissage des tableaux configDetecteeLateraux[] et configDetecteeCentraux[],
		for(int i = 0; i < nbMais; i++) {
			int posMaisX = posMais[i].x;
			int posMaisY = posMais[i].y;

			float scoreNoir = GetCarreDistanceAuBarycentre(&imageNoir, posMaisX, posMaisY);
			float scoreBlanc = GetCarreDistanceAuBarycentre(&imageBlanc, posMaisX, posMaisY);

			cout << scoreNoir << " | " << scoreBlanc;

			if(scoreBlanc < 0 && scoreNoir < 0) {
				configDetectee[i] = MAIS_UNDEF;
				cout << " : undef";
			} else if(scoreNoir < 0) {
				configDetectee[i] = MAIS_BLANC;
				cout << " : blanc";
			} else if(scoreBlanc < 0) {
				configDetectee[i] = MAIS_NOIR;
				cout << " : noir";
			} else if(scoreBlanc < scoreNoir) {
				configDetectee[i] = MAIS_BLANC;
				cout << " : blanc";
			} else {
				configDetectee[i] = MAIS_NOIR;
				cout << " : noir";
			}
			cout << endl;
		}

		// Calcul de la configuration la plus approchante de ce qui vient d'etre déterminé au-dessus :
		unsigned int score = 0;
		for(int i = 0; i < nbConfig; i++) {
			score = 0;
			for(int j = 0; j < nbMais; j++) {
				CouleurMais couleurMaisConfig;
				if(iter == 0)
					couleurMaisConfig = configMaisLateraux[i][j];
				else
					couleurMaisConfig = configMaisCentraux[i][j];

				score += int(configDetectee[j] == couleurMaisConfig);
			}
			if(score > scoreConfig) {
				scoreConfig = score;
				numConfig = i;
			}
		}
	}

	cout << "Configuration mais lateraux : " << numConfigLateraux << endl;
	cout << "Score de cette configuration : " << scoreConfigLateraux << endl;

	cout << "Configuration mais centraux : " << numConfigCentraux << endl;
	cout << "Score de cette configuration : " << scoreConfigCentraux << endl;

	cout.precision(precision);

	cout << "-------------------------" << endl;

	return true;
}
