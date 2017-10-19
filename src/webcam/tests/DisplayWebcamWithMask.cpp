// DisplayWebcamWithMask.cpp

#include "Clock.h"
#include "ImageFiltree.h"
#include "Webcam.h"
#include <GL/glfw.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

using namespace std;

#define NB_COULEURS_HAUT 4
#define NB_COULEURS_BAS 6
enum t_couleurs { BLEU = 0, ROUGE };
IWebcam* webcam;
ImageFiltree imageOriginale;
enum t_couleurs couleurTachesHaut[NB_COULEURS_HAUT];
enum t_couleurs couleurTachesBas[NB_COULEURS_BAS];
const ColorHSV lightRed(0, 50, 45);
const ColorHSV darkRed(10, 100, 100);
const ColorHSV lightBlue(210, 15, 15);
const ColorHSV darkBlue(250, 70, 60);
bool isBlue;

GLuint id_texture_original;
unsigned char* buffer_data = NULL;

void Display();
void TreatData(const unsigned char* input_data, unsigned char* displayed_data, int w, int h);

int main(int argc, char* argv[]) {
	int x, y;
	int fps = 0, frames;
	double t0, t;
	char titre[200] = "";
	bool running = true;

	// Récupération des arguments et création de la webcam
	webcam = NewWebcam(argc, argv);

	if(!webcam)
		return -1;

	// Initialisation de GLFW
	glfwInit();

	if(!glfwOpenWindow(webcam->GetWidth(), webcam->GetHeight(), 0, 0, 0, 0, 0, 0, GLFW_WINDOW)) {
		printf("Erreur : impossible d'ouvrir une fenêtre !\n");
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwEnable(GLFW_STICKY_KEYS);

	glfwSwapInterval(0); // disable V-Sync

	glEnable(GL_TEXTURE_2D);

	// Récupération des IDs pour les textures et création de celles-ci
	glGenTextures(1, &id_texture_original);
	glBindTexture(GL_TEXTURE_2D, id_texture_original);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, webcam->GetWidth(), webcam->GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	running = true;
	frames = 0;
	t0 = glfwGetTime();
	buffer_data = new unsigned char[webcam->GetWidth() * webcam->GetHeight() * 3];
	// INITIALISATIONS
	imageOriginale.Init(webcam->GetWidth(), webcam->GetHeight());
	for(int i = 0; i < NB_COULEURS_HAUT; i++)
		couleurTachesHaut[i] = BLEU;
	for(int i = 0; i < NB_COULEURS_BAS; i++)
		couleurTachesBas[i] = BLEU;
	if(strcmp(argv[argc - 1], "ROUGE") == 0)
		isBlue = false;
	else
		isBlue = true;
	printf("DERNIER ARGUMENT : %s ET ISBLUE=%s\n", argv[argc - 1], isBlue ? "true" : "false");

	// Boucle principale
	while(running) {
		t = glfwGetTime();
		glfwGetMousePos(&x, &y);

		// Calcul des FPS
		if((t - t0) > 1.0 || frames == 0) {
			fps = (int)((double)frames / (double)(t - t0));
			sprintf(titre, "Display webcam - FPS : %d | Nombre de taches : %d", fps, imageOriginale.GetListeTaches().size());
			glfwSetWindowTitle(titre);
			t0 = t;
			frames = 0;
		}
		frames++;

		// Récupération de l'image à afficher depuis la webcam
		if(!webcam->WaitForImageSync()) {
			fprintf(stderr, "Erreur lors de la capture d'une frame par la webcam\n");
			exit(EXIT_FAILURE);
		}

		// Affichage
		Display();

		glfwSwapBuffers();

		// On demande à la webcam de récupérer l'image suivante
		if(!webcam->QueryImage()) {
			fprintf(stderr, "Erreur lors de la demande au périphérique d'envoi d'une nouvelle image\n");
			exit(EXIT_FAILURE);
		}

		// Condition de sortie
		running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);

		// Test d'erreurs OpenGL
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
			printf("Erreur OpenGL : %s\n", gluErrorString(error));
	}

	glfwTerminate();

	webcam->Close();

	delete webcam;
	delete[] buffer_data;

	return 0;
}

void Display() {
	// Récupération de la taille de l'image de la webcam
	int w = webcam->GetWidth(), h = webcam->GetHeight();

	// Récupération de la taille de la fenêtre
	int win_width = 0, win_height = 0;
	glfwGetWindowSize(&win_width, &win_height);
	win_height = win_height > 0 ? win_height : 1;

	// Mise à jour du viewport
	glViewport(0, 0, win_width, win_height);

	// Matrice de projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, win_width, 0, win_height);

	// Matrix modelview :
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Color buffer
	glClearColor(0.5, 0.5, 0.5, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3ub(0xFF, 0xFF, 0xFF);
	glEnable(GL_TEXTURE_2D);

	// Traitement
	// Paramètres
	int camWidth = webcam->GetWidth();
	int camHeight = webcam->GetHeight();
	// Préparation d'une structure paramsTaches
	ImageFiltree::ParamsTaches paramsTaches;
	paramsTaches.width_min = 40;
	paramsTaches.height_min = 25;
	paramsTaches.aire_min = 40 * 25;
	paramsTaches.width_max = 150; // 120
	paramsTaches.height_max = 50;
	paramsTaches.aire_max = 150 * 50;
	// Image Filtrée ROUGE (RGB)
	//	Color lightRed(100,0,0);
	//	Color darkRed(255,50,50);
	//	imageRouge.SetColorMin(lightRed);
	//	imageRouge.SetColorMax(darkRed);
	//	imageRouge.UpdateFromData(webcam->GetData());
	//	imageRouge.SuperFlouterBlanc();
	//	imageRouge.RepererTaches(paramsTaches);
	// Image Filtrée BLEUE (HSV)
	//	Color lightBlue(30,30,70);
	//	Color darkBlue(70,70,120);
	//	imageBleue.SetColorMin(lightBlue);
	//	imageBleue.SetColorMax(darkBlue);
	list<ImageFiltree::Tache> tachesHaut;
	list<ImageFiltree::Tache> tachesBas;
	imageOriginale.SetColorHSVMin(isBlue ? lightBlue : lightRed);
	imageOriginale.SetColorHSVMax(isBlue ? darkBlue : darkRed);
	imageOriginale.UpdateFromDataHSV(webcam->GetData());
	imageOriginale.SuperFlouterBlanc();
	imageOriginale.RepererTaches(paramsTaches);

	printf("-----------------------\n");
	const list<ImageFiltree::Tache>& taches = imageOriginale.GetListeTaches();
	for(list<ImageFiltree::Tache>::const_iterator it = taches.begin(); it != taches.end(); it++) {
		const ImageFiltree::Tache& tache = (*it);
		printf("Tache détectée : x=%d, y=%d, hauteur=%d, largeur=%d\n", tache.x, tache.y, tache.h, tache.w);
		if(tachesHaut.empty() || (!tachesHaut.empty() && abs(tachesHaut.back().y - tache.y) < 20)) {
			tachesHaut.push_back(tache);
		} else if((!tachesBas.empty() && abs(tachesBas.back().y - tache.y) < 20) ||
		          (!tachesHaut.empty() && tachesHaut.back().y - 40 > tache.y)) {
			tachesBas.push_back(tache);
		} else if((!tachesHaut.empty() && (tachesHaut.back().y < tache.y - 40))) {
			while(tachesHaut.back().y < tache.y - 40) {
				tachesBas.push_back(tachesHaut.back());
				tachesHaut.pop_back();
			}
			tachesHaut.push_back(tache);
		} else {
			tachesBas.push_back(tache);
		}
	}

	int oldX = 0;
	int index = 0;
	printf("****** TACHES HAUT ******\n");
	for(list<ImageFiltree::Tache>::const_iterator it = tachesHaut.begin(); it != tachesHaut.end(); it++) {
		const ImageFiltree::Tache& tache = (*it);
		// printf("Tache haut : x=%d, y=%d\n", tache.x, tache.y);
		if(!oldX) {
			oldX = tache.x;
			couleurTachesHaut[index] = isBlue ? BLEU : ROUGE;
			printf("%s ", isBlue ? "BLEU" : "ROUGE");
		} else {
			index = 1;
			while(!(tache.x - oldX <= 75 * index) && !(index > NB_COULEURS_HAUT)) {
				printf("%s ", isBlue ? "ROUGE" : "BLEU");
				couleurTachesHaut[index] = isBlue ? ROUGE : BLEU;
				index++;
			}
			printf("%s ", isBlue ? "BLEU" : "ROUGE");
			couleurTachesHaut[index] = isBlue ? BLEU : ROUGE;
		}
		oldX = tache.x;
		index++;
	}
	for(int i = index; i < NB_COULEURS_HAUT; i++)
		couleurTachesHaut[i] = isBlue ? ROUGE : BLEU;
	printf("\n");

	oldX = 0;
	index = 0;
	printf("****** TACHES BAS ******\n");
	for(list<ImageFiltree::Tache>::const_iterator it = tachesBas.begin(); it != tachesBas.end(); it++) {
		const ImageFiltree::Tache& tache = (*it);
		// printf("Tache bas : x=%d, y=%d\n", tache.x, tache.y);
		if(!oldX) {
			oldX = tache.x;
			couleurTachesBas[index] = isBlue ? BLEU : ROUGE;
			printf("%s ", isBlue ? "BLEU" : "ROUGE");
		} else {
			index = 1;
			while(!(tache.x - oldX <= 100 * index) && !(index > NB_COULEURS_BAS)) {
				couleurTachesBas[index] = isBlue ? ROUGE : BLEU;
				printf("%s ", isBlue ? "ROUGE" : "BLEU");
				index++;
			}
			couleurTachesHaut[index] = isBlue ? BLEU : ROUGE;
			printf("%s ", isBlue ? "BLEU" : "ROUGE");
		}
		oldX = tache.x;
		index++;
	}
	for(int i = index; i < NB_COULEURS_BAS; i++)
		couleurTachesBas[i] = isBlue ? ROUGE : BLEU;
	printf("\n");

	// On reverse le sens de calcul des images
	// isBlue = !isBlue;
	// On flushe les listes
	// for (int i=0; i<tachesBas.size(); i++) 	tachesBas.pop_back();
	// for (int i=0; i<tachesHaut.size(); i++) tachesHaut.pop_back();
	// Traiment des données
	TreatData(imageOriginale.GetDataRGB(), buffer_data, w, h);

	// On passe l'image originale à OpenGL
	glBindTexture(GL_TEXTURE_2D, id_texture_original);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, buffer_data);

	// Affichage de l'image originale (NB : on retourne verticalement l'image)
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1);
	glVertex2i(0, 0);
	glTexCoord2f(1, 1);
	glVertex2i(w, 0);
	glTexCoord2f(1, 0);
	glVertex2i(w, h);
	glTexCoord2f(0, 0);
	glVertex2i(0, h);
	glEnd();
}

void TreatData(const unsigned char* input_data, unsigned char* displayed_data, int w, int h) {
	memcpy(displayed_data, input_data, w * h * 3 * sizeof(unsigned char));
}
