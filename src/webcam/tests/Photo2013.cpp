// Webcam2013.cpp - programme qui prend une image et l'enregistre au format PPM puis essaye de voir la couleur des
// bougies

#include "../../robot/Commun.h"
#include "ImageFiltree.h"
#include "Webcam.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace std;
#define NB_BOUGIES 10
struct Position {
	int x, y;
};
enum Couleurs { BLEU = 0, ROUGE = 1, OTHER = 2 };
char* couleurToString(enum Couleurs c) {
	if(c == 0)
		return const_cast<char*>("BLEU");
	else if(c == 1)
		return const_cast<char*>("ROUGE");
	else
		return const_cast<char*>("UNDEFINED");
}

void SavePPM(const char* filename, const unsigned char* data, int width, int height);
void FillColorTab(const unsigned char* data, int width, enum Couleurs tab[], vector<struct Position> positions[]);
bool CompareColorTab(const enum Couleurs tab1[], const enum Couleurs tab2[], int size);
enum Couleurs* FindConfiguration(vector<struct Position> positions,
                                 enum Couleurs couleurs1[],
                                 enum Couleurs couleurs2[],
                                 enum Couleurs couleurs3[],
                                 IWebcam* webcam);

int main(int argc, char* argv[]) {
	IWebcam* webcam = NewWebcam(argc, argv);

	// Récupération d'une image
	webcam->QueryImage();
	if(!webcam->WaitForImageSync()) {
		fprintf(stderr, "Erreur lors de la capture d'une frame par la webcam\n");
		exit(EXIT_FAILURE);
	}

	// Enregistrement de l'image
	SavePPM("capture.ppm", webcam->GetData(), webcam->GetWidth(), webcam->GetHeight());

	// Libération des ressources
	delete webcam;

	// Traitement des données
	bool pointsRecvd = false;
	string line, allFileContent = "", allNewFileContent = "";
	ifstream f("points.txt");
	if(f.is_open()) {
		while(getline(f, line)) {
			allFileContent += line;
			allFileContent += '\n';
		}
		f.close();
	}
	while(!pointsRecvd) {
		f.open("points.txt");
		if(f.is_open()) {
			while(getline(f, line)) {
				allNewFileContent += line;
				allNewFileContent += '\n';
			}
			f.close();
		}
		if((allNewFileContent != allFileContent) && allNewFileContent != "") {
			pointsRecvd = true;
		} else {
			allNewFileContent = "";
		}
		sleep(0.5);
	}
	vector<struct Position> positions;
	vector<string> lines;
	stringstream ss(allNewFileContent);
	string item;
	while(getline(ss, item, '\n')) {
		lines.push_back(item);
	}
	struct Position p;
	char tmp;
	for(size_t i = 0; i < lines.size(); i++) {
		stringstream ss(lines.at(i));
		ss >> p.x >> tmp >> p.y;
		positions.push_back(p);
	}

	webcam = NewWebcam(argc, argv);
	// Récupération d'une image
	webcam->QueryImage();
	if(!webcam->WaitForImageSync()) {
		fprintf(stderr, "Erreur lors de la capture d'une frame par la webcam\n");
		exit(EXIT_FAILURE);
	}

	enum Couleurs couleurs1[NB_BOUGIES];
	enum Couleurs couleurs2[NB_BOUGIES];
	enum Couleurs couleurs3[NB_BOUGIES];
	for(int i = 0; i < NB_BOUGIES; i++) {
		couleurs1[i] = couleurs2[i] = couleurs3[i] = OTHER;
	}
	while(!(positions.size() <= NB_BOUGIES))
		positions.pop_back();
	enum Couleurs* configuration = FindConfiguration(positions, couleurs1, couleurs2, couleurs3, webcam);

	if(configuration) {
		for(size_t i = 0; i < NB_BOUGIES; i++) {
			cout << couleurToString(configuration[i]) << endl;
		}
	}

	delete webcam;

	return 0;
}

enum Couleurs* FindConfiguration(vector<struct Position> positions,
                                 enum Couleurs couleurs1[],
                                 enum Couleurs couleurs2[],
                                 enum Couleurs couleurs3[],
                                 IWebcam* webcam) {
	FillColorTab(webcam->GetData(), webcam->GetWidth(), couleurs1, &positions);
	FillColorTab(webcam->GetData(), webcam->GetWidth(), couleurs2, &positions);

	if(!CompareColorTab(couleurs1, couleurs2, positions.size())) {
		FillColorTab(webcam->GetData(), webcam->GetWidth(), couleurs3, &positions);
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
}

void FillColorTab(const unsigned char* data, int width, enum Couleurs tab[], vector<struct Position>* positions) {
	for(size_t i = 0; i < positions->size(); i++) {
		Position p = positions->at(i);
		ColorHSV pixel =
		    Color(data[3 * (p.y * width + p.x) + 0], data[3 * (p.y * width + p.x) + 1], data[3 * (p.y * width + p.x) + 2]);
		if(pixel.h > 300 || pixel.h < 50)
			tab[i] = ROUGE;
		else if(pixel.h > 150 || pixel.h < 300)
			tab[i] = BLEU;
		else
			tab[i] = OTHER;
	}
}

bool CompareColorTab(const enum Couleurs tab1[], const enum Couleurs tab2[], int size) {
	bool equals = true;
	for(int i = 0; i < size; i++) {
		if(tab1[i] != tab2[i])
			equals = false;
	}
	return equals;
}

void SavePPM(const char* filename, const unsigned char* data, int width, int height) {
	// Ouverture du fichier
	FILE* f = fopen(filename, "wb");
	if(!f) {
		fprintf(stderr, "Erreur : impossible de creer %s\n", filename);
		return;
	}

	// Ecriture du header
	fprintf(f, "P6\n%d %d\n255\n", width, height);

	// Ecriture des datas
	for(int y = 0; y < height; y++)
		fwrite(&data[y * width * 3], 1, width * 3, f);

	// Fermeture du fichier
	fclose(f);
}
