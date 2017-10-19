// Photo2013.cpp - programme qui prend une image et l'enregistre au format PPM puis essaye de voir la couleur des
// bougies

#include "Webcam2013.h"
#include "../../../robot/2013/Chronos/librobot2013/Robot2013.h"
#include "../../../robot/Outils/log/Log.h"
#include "ImageFiltree.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

Webcam2013::Webcam2013() : webcam(NULL), robot(NULL) {}

Webcam2013::Webcam2013(int argc, char* argv[], Robot2013* robot) : webcam(NULL) {
	int compteur = 0;
	if(robot != NULL)
		this->robot = robot;
	else
		logError("On initialise la webcam avec un robot null!!");

	for(int i = 0; i < NB_BOUGIES; i++) {
		couleurs1[i] = couleurs2[i] = couleurs3[i] = OTHER;
	}

	while(!(initializeWebcam(argc, argv) || (compteur >= 5)) && !robot->TiretteTiree())
		compteur++;
}

Webcam2013::~Webcam2013() {
	if(webcam != NULL)
		webcam->Close();

	delete webcam;
}

bool Webcam2013::initializeWebcam(int argc, char* argv[]) {
	// Nouvelle webcam
	webcam = NewWebcam(argc, argv);

	if(webcam == NULL)
		return false;

	// Récupération d'une image
	webcam->QueryImage();
	if(!webcam->WaitForImageSync()) {
		logError("Erreur lors de la capture d'une frame par la webcam");
		return false;
	}

	// Enregistrement de l'image
	SavePPM("capture.ppm", webcam->GetData(), webcam->GetWidth(), webcam->GetHeight());

	// Traitement des données
	//	bool pointsRecvd = false;
	string line, allNewFileContent = ""; //, allNewFileContent="";
	logDebug3("Suppression du vieux fichier signal.txt");
	system("rm -rf signal.txt");
	logDebug3("En attente du fichier signal.txt");
	ifstream f_sign;
	while(!f_sign.is_open()) {
		if(robot->TiretteTiree()) {
			logWarn("La tirette est tirée alors qu'on attendait le signal, on aura donc la config par défaut");
			return false;
		}
		f_sign.open("signal.txt", ios_base::in);
		msleep(50);
	}

	f_sign.close();
	logDebug3("Fichier signal.txt trouvé, lecture du points.txt");

	ifstream f("points.txt");
	if(f.is_open()) {
		while(getline(f, line)) {
			allNewFileContent += line;
			allNewFileContent += '\n';
		}
		f.close();
	} else {
		logError("Impossible d'ouvrir le fichier points.txt!");
		return false;
	}
	//		logDebug3("En attente d'un nouveau fichier points.txt");
	//		while (!pointsRecvd && !robot->TiretteTiree()) {
	//			f.open("points.txt");
	//			if (f.is_open()) {
	//				while (getline (f,line)) {
	//					allNewFileContent += line;
	//					allNewFileContent += '\n';
	//				}
	//				f.close();
	//			}
	//			if ((allNewFileContent != allFileContent) && allNewFileContent != "") {
	//				pointsRecvd = true;
	//			}
	//			else {
	//				allNewFileContent = "";
	//			}
	//		}

	if(robot->TiretteTiree()) {
		logWarn("Tirette tiree, on abandonne la lecture d'une image de la webcam");
		return false;
	}

	logDebug3("Nouveau fichier points.txt trouvé!");
	vector<string> lines;
	stringstream ss(allNewFileContent);
	string item;
	while(getline(ss, item, '\n')) {
		lines.push_back(item);
	}
	struct Position pos;
	char tmp;
	for(size_t i = 0; i < lines.size(); i++) {
		stringstream ss(lines.at(i));
		ss >> pos.x >> tmp >> pos.y;
		positions.push_back(pos);
	}

	// Récupération d'une image
	logDebug3("On recupère une image");
	webcam->QueryImage();
	if(!webcam->WaitForImageSync()) {
		logError("Erreur lors de la capture d'une frame par la webcam");
		return false;
	}

	logDebug3("Nombre de positions initial : ", positions.size());
	while(!(positions.size() <= NB_BOUGIES))
		positions.pop_back();
	logDebug3("Nombre de positions final : ", positions.size());

	for(size_t i = 0; i < lines.size(); i++)
		logDebug3("positions[", i, "]=(", positions[i].x, ", ", positions[i].y, ")");

	return true;
}

vector<struct Position> Webcam2013::getPositions() {
	int compteur = 0;
	while(positions.empty() && compteur < 100)
		;
	if(compteur >= 100) {
		logError("On attend les positions depuis 100 boucles, on retourne NULL");
	}
	return positions;
}

enum Couleurs* Webcam2013::FindConfiguration() {
	if(webcam != NULL) {
		webcam->QueryImage();
		webcam->WaitForImageSync();
		FillColorTab(webcam->GetData(), webcam->GetWidth(), couleurs1);
		webcam->QueryImage();
		webcam->WaitForImageSync();
		FillColorTab(webcam->GetData(), webcam->GetWidth(), couleurs2);

		if(!CompareColorTab(couleurs1, couleurs2, positions.size())) {
			webcam->QueryImage();
			webcam->WaitForImageSync();
			FillColorTab(webcam->GetData(), webcam->GetWidth(), couleurs3);
			if(!CompareColorTab(couleurs1, couleurs3, positions.size())) {
				if(!CompareColorTab(couleurs2, couleurs3, positions.size())) {
					cout << "GROS PROBLEME, ON N'A DEUX FOIS LES MEMES VALEURS" << endl;
					return NULL;
				} else {
					return couleurs3;
				}
			} else {
				return couleurs3;
			}
		} else {
			return couleurs2;
		}
	} else {
		return NULL;
	}
}

void Webcam2013::FillColorTab(const unsigned char* data, int width, enum Couleurs tab[]) {
	int sommeH, totalH;
	//	logDebug4("Remplissage d'un tableau de couleurs");
	for(size_t element = 0; element < positions.size(); element++) {
		Position p = positions.at(element);
		sommeH = 0;
		totalH = 0;
		for(int i = -3; i < 4; i++) {
			for(int j = -3; j < 4; j++) {
				ColorHSV pixel = Color(data[3 * ((p.y + j) * width + (p.x + i)) + 0],
				                       data[3 * ((p.y + j) * width + (p.x + i)) + 1],
				                       data[3 * ((p.y + j) * width + (p.x + i)) + 2]);
				sommeH += pixel.h;
			}
		}
		totalH = sommeH / 49;
		// if (pixel.h > 300 || pixel.h < 50)
		if(totalH > 300 || totalH < 75) {
			tab[element] = ROUGE;
			//			logDebug4("Element à la position : ", element, " ROUGE");
		} else {
			tab[element] = BLEU;
			//			logDebug4("Element à la position : ", element, " BLEU");
		}
	}
}

bool Webcam2013::CompareColorTab(const enum Couleurs tab1[], const enum Couleurs tab2[], int size) {
	bool equals = true;
	for(int i = 0; i < size; i++) {
		if(tab1[i] != tab2[i])
			equals = false;
	}
	return equals;
}

void Webcam2013::SavePPM(const char* filename, const unsigned char* data, int width, int height) {
	// Ouverture du fichier
	FILE* f = fopen(filename, "wb");
	if(!f) {
		logError("Erreur : impossible de creer ", filename);
		return;
	}

	// Ecriture du header
	fprintf(f, "P6\n%d %d\n255\n", width, height);

	// Ecriture des datas
	for(int y = 0; y < height; y++)
		fwrite(&data[y * width * 3], 1, width * 3, f);

	// Fermeture du fichier
	fclose(f);

	logDebug("Fichier ", filename, " créé avec succes!");
}
