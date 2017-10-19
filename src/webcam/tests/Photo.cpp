// Photo.cpp - programme qui prend une image et l'enregistre au format PPM

#include "ImageFiltree.h"
#include "Webcam.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace std;

void SavePPM(const char* filename, const unsigned char* data, int width, int height);

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

	return 0;
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
