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
enum t_couleurs { BLEU = 0, ROUGE = 1, UNDEFINED = 2 };
char* couleurToString(enum t_couleurs c) {
	if(c == 0)
		return const_cast<char*>("BLEU");
	else if(c == 1)
		return const_cast<char*>("ROUGE");
	else
		return const_cast<char*>("UNDEFINED");
}
IWebcam* webcam;
ImageFiltree imageOriginale;
vector<ImageFiltree::Tache> tachesHaut;
vector<ImageFiltree::Tache> tachesBas;
enum t_couleurs couleurTachesHaut[NB_COULEURS_HAUT];
enum t_couleurs couleurTachesBas[NB_COULEURS_BAS];
enum t_couleurs couleurTachesHautBleu[NB_COULEURS_HAUT];
enum t_couleurs couleurTachesBasBleu[NB_COULEURS_BAS];
const ColorHSV lightRed(0, 50, 45);
const ColorHSV darkRed(10, 100, 100);
const ColorHSV lightBlue(210, 15, 15);
const ColorHSV darkBlue(250, 70, 60);
bool isBlue;
int startXHBlue;
int startXBBlue;
int nbTachesHaut;
int nbTachesBas;
int nbTachesHautBleu;
int nbTachesBasBleu;

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
	isBlue = true;
	startXBBlue = 0;
	startXHBlue = 0;
	for(int i = 0; i < NB_COULEURS_HAUT; i++)
		couleurTachesHaut[i] = UNDEFINED;
	for(int i = 0; i < NB_COULEURS_BAS; i++)
		couleurTachesBas[i] = UNDEFINED;
	for(int i = 0; i < NB_COULEURS_HAUT; i++)
		couleurTachesHautBleu[i] = UNDEFINED;
	for(int i = 0; i < NB_COULEURS_BAS; i++)
		couleurTachesBasBleu[i] = UNDEFINED;
	nbTachesBasBleu = 0;
	nbTachesHautBleu = 0;
	nbTachesBas = 0;
	nbTachesHaut = 0;

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
	paramsTaches.width_min = 40; // 30
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
	string msg;
	string msgContraire;
	if(isBlue) {
		imageOriginale.SetColorHSVMin(lightBlue);
		imageOriginale.SetColorHSVMax(darkBlue);
		printf("========= CONFIG BLEUE =========\n");
		msg = "BLEU";
		msgContraire = "ROUGE";
	} else {
		imageOriginale.SetColorHSVMin(lightRed);
		imageOriginale.SetColorHSVMax(darkRed);
		printf("========= CONFIG ROUGE =========\n");
		msg = "ROUGE";
		msgContraire = "BLEU";
	}
	imageOriginale.UpdateFromDataHSV(webcam->GetData());
	imageOriginale.SuperFlouterBlanc();
	imageOriginale.RepererTaches(paramsTaches);

	printf("-----------------------\n");
	const list<ImageFiltree::Tache>& listTaches = imageOriginale.GetListeTaches();
	vector<ImageFiltree::Tache> taches(listTaches.begin(), listTaches.end());
	for(vector<ImageFiltree::Tache>::const_iterator it = taches.begin(); it != taches.end(); it++) {
		const ImageFiltree::Tache& tache = (*it);
		// Contraintes de "fenêtrage" : TODO -> réduire l'image plutôt
		if(tache.x > 100 && tache.y > 150 && tache.x < 550 && tache.y < 350) {
			printf("Tache détectée : x=%d, y=%d, hauteur=%d, largeur=%d\n", tache.x, tache.y, tache.h, tache.w);
			if(tachesBas.empty() || (!tachesBas.empty() && abs(tachesBas.back().y - tache.y) < 30)) {
				tachesBas.push_back(tache);
			} else if((!tachesHaut.empty() && abs(tachesHaut.back().y - tache.y) < 20) ||
			          (!tachesBas.empty() && tachesBas.back().y - 40 > tache.y)) {
				tachesHaut.push_back(tache);
			} else if((!tachesBas.empty() && (tachesBas.back().y < tache.y - 40))) {
				while(tachesBas.back().y < tache.y - 40) {
					tachesHaut.push_back(tachesBas.back());
					tachesBas.pop_back();
					printf("Coucou");
				}
				tachesBas.push_back(tache);
			} else {
				tachesHaut.push_back(tache);
			}
		}
	}

	printf("--> got %d taches\n", tachesHaut.size() + tachesBas.size());
	// Taches du HAUT
	printf("DEBUG : taches du haut \n");
	for(int i = 0; i < tachesHaut.size(); i++)
		printf("+ x=%d, y=%d\n", tachesHaut[i].x, tachesHaut[i].y);

	int index = 0;
	if(isBlue)
		startXHBlue = tachesHaut[index].x;
	for(int i = 0; i < NB_COULEURS_HAUT; i++) {
		// Ecartement théorique entre deux "bougies" du haut : 100px
		if(abs(tachesHaut[index].x - (startXHBlue + i * 100)) < 5 ||
		   (tachesHaut[index].x > (startXHBlue + (i - 1) * 100) && tachesHaut[index].x < (startXHBlue + i * 100))) {
			couleurTachesHaut[i] = isBlue ? BLEU : ROUGE;
			index++;
		} else if(index < tachesHaut.size()) {
			couleurTachesHaut[i] = isBlue ? ROUGE : BLEU;
		}
	}
	nbTachesHaut = tachesHaut.size();

	// Taches du BAS
	printf("DEBUG : taches du bas \n");
	for(int i = 0; i < tachesBas.size(); i++)
		printf("+ x=%d, y=%d\n", tachesBas[i].x, tachesBas[i].y);

	index = 0;
	int deltaBas = 70;
	if(isBlue)
		startXBBlue = tachesBas[index].x;
	for(int i = 0; i < NB_COULEURS_BAS; i++) {
		// Ecartement théorique entre deux bougies du bas : 70px (offset de 20)
		if(abs(tachesBas[index].x - (startXBBlue + i * deltaBas)) < 15 ||
		   (tachesHaut[index].x > (startXBBlue + (i - 1) * deltaBas) && tachesBas[index].x < (startXBBlue + i * deltaBas))) {
			couleurTachesBas[i] = isBlue ? BLEU : ROUGE;
			index++;
		} else if(index < tachesBas.size()) {
			couleurTachesBas[i] = isBlue ? ROUGE : BLEU;
		}
	}
	nbTachesBas = tachesBas.size();

	if(isBlue) {
		// Copie des données dans des tableaux pour les garder plus tard
		for(int i = 0; i < NB_COULEURS_HAUT; i++)
			couleurTachesHautBleu[i] = couleurTachesHaut[i];
		for(int i = 0; i < NB_COULEURS_BAS; i++)
			couleurTachesBasBleu[i] = couleurTachesBas[i];
		// Copie du nb de Taches en haut et en bas
		nbTachesBasBleu = nbTachesBas;
		nbTachesHautBleu = nbTachesHaut;
	} else {
		// Traitement des données en fonction de ce qu'on a détecté pour le BLEU avant et pour le ROUGE maintenant
		printf("****** TACHES HAUT ******\n");
		// Couleurs du HAUT
		for(int i = 0; i < NB_COULEURS_HAUT; i++) {
			if(couleurTachesHautBleu[i] == couleurTachesHaut[i]) {
				printf("EGAL: %s\n", couleurToString(couleurTachesHaut[i]));
			} else if((int)couleurTachesHautBleu[i] != (int)couleurTachesHaut[i] && nbTachesHautBleu > nbTachesHaut) {
				printf("DECISION: %s (BLEU maitre)\n", couleurToString(couleurTachesHautBleu[i]));
			} else if((int)couleurTachesHautBleu[i] != (int)couleurTachesHaut[i] && nbTachesHautBleu < nbTachesHaut) {
				printf("DECISION: %s (ROUGE maitre)\n", couleurToString(couleurTachesHaut[i]));
			} else if(((int)couleurTachesHautBleu[i] == BLEU && (int)couleurTachesHaut[i] == ROUGE) ||
			          (int)(couleurTachesHautBleu[i] == ROUGE && (int)couleurTachesHaut[i] == BLEU)) {
				printf("PROBLEME: %s config bleue et %s config rouge\n",
				       couleurToString(couleurTachesHautBleu[i]),
				       couleurToString(couleurTachesHaut[i]));
			} else {
				printf("ERREUR: %s config bleue et %s config rouge\n",
				       couleurToString(couleurTachesHautBleu[i]),
				       couleurToString(couleurTachesHaut[i]));
			}
			// TODO : si 5 taches en BLEU et 4 taches en ROUGE, le maitre est BLEU (prend les décisions)
		}

		printf("****** TACHES BAS ******\n");
		// Couleurs du BAS
		for(int i = 0; i < NB_COULEURS_BAS; i++) {
			if(couleurTachesBasBleu[i] == couleurTachesBas[i]) {
				printf("EGAL: %s\n", couleurToString(couleurTachesBas[i]));
			} else if((int)couleurTachesBasBleu[i] != (int)couleurTachesBas[i] && nbTachesBasBleu > nbTachesBas) {
				printf("DECISION: %s (BLEU maitre)\n", couleurToString(couleurTachesBasBleu[i]));
			} else if((int)couleurTachesBasBleu[i] != (int)couleurTachesBas[i] && nbTachesBasBleu < nbTachesBas) {
				printf("DECISION: %s (ROUGE maitre)\n", couleurToString(couleurTachesBas[i]));
			} else if(((int)couleurTachesBasBleu[i] == BLEU && (int)couleurTachesBas[i] == ROUGE) ||
			          (int)(couleurTachesBasBleu[i] == ROUGE && (int)couleurTachesBas[i] == BLEU)) {
				printf("PROBLEME: %s config bleue et %s config rouge\n",
				       couleurToString(couleurTachesBasBleu[i]),
				       couleurToString(couleurTachesHaut[i]));
			} else {
				printf("ERREUR: %s config bleue et %s config rouge\n",
				       couleurToString(couleurTachesBasBleu[i]),
				       couleurToString(couleurTachesHaut[i]));
			}
			// TODO : si 5 taches en BLEU et 4 taches en ROUGE, le maitre est BLEU (prend les décisions)
		}
	}

	tachesHaut.clear();
	tachesBas.clear();
	isBlue = !isBlue;

	/*
	if (isBlue)
	    nbTachesDetectees = taches.size();
	else {
	    if (nbTachesDetectees == taches.size()) {
	        int oldX = 0;
	        int index = 0;
	        printf("****** TACHES HAUT ******\n");
	        for(list<ImageFiltree::Tache>::const_iterator it = tachesHaut.begin() ; it != tachesHaut.end() ; it++) {
	                const ImageFiltree::Tache& tache = (*it);
	                //printf("Tache haut : x=%d, y=%d\n", tache.x, tache.y);
	                if (!oldX) {
	                    oldX = tache.x;
	                    if (isBlue)
	                        couleurTachesHaut[index] = isBlue?BLEU:ROUGE;
	                    else {
	                        if (couleurTachesHaut[index] == isBlue?BLEU:ROUGE)
	                            printf("ROUGE : JE SUIS OK AVEC CETTE VALEUR \n");
	                        else
	                            printf("ROUGE : je suis pas OK avec cette valeur! \n");
	                    }
	                    printf("%s ", msg);
	                }
	                else {
	                    index = 1;
	                    while (!(tache.x - oldX <= 75*index) && !(index > NB_COULEURS_HAUT)) {
	                        printf("%s ", msgContraire);
	                        if (isBlue)
	                            couleurTachesHaut[index] = isBlue?ROUGE:BLEU;
	                        else {
	                            if (couleurTachesHaut[index] == isBlue?ROUGE:BLEU)
	                                printf("ROUGE : JE SUIS OK AVEC CETTE VALEUR \n");
	                            else
	                                printf("ROUGE : je suis pas OK avec cette valeur! \n");
	                        }
	                        index++;
	                    }
	                    printf("%s ", msg);
	                    if (isBlue)
	                        couleurTachesHaut[index] = isBlue?BLEU:ROUGE;
	                    else {
	                        if (couleurTachesHaut[index] == isBlue?BLEU:ROUGE)
	                            printf("ROUGE : JE SUIS OK AVEC CETTE VALEUR \n");
	                        else
	                            printf("ROUGE : je suis pas OK avec cette valeur! \n");
	                    }
	                }
	                oldX = tache.x;
	                index++;
	        }
	        for (int i=index; i<NB_COULEURS_HAUT; i++) couleurTachesHaut[i] = isBlue?ROUGE:BLEU;
	        printf("\n");

	        oldX = 0;
	        index = 0;
	        printf("****** TACHES BAS ******\n");
	        for(list<ImageFiltree::Tache>::const_iterator it = tachesBas.begin() ; it != tachesBas.end() ; it++) {
	            const ImageFiltree::Tache& tache = (*it);
	            //printf("Tache bas : x=%d, y=%d\n", tache.x, tache.y);
	            if (!oldX) {
	                oldX = tache.x;
	                if (isBlue)
	                    couleurTachesBas[index] = isBlue?BLEU:ROUGE;
	                else {
	                    if (couleurTachesHaut[index] == isBlue?BLEU:ROUGE)
	                        printf("ROUGE : JE SUIS OK AVEC CETTE VALEUR \n");
	                    else
	                        printf("ROUGE : je suis pas OK avec cette valeur! \n");
	                }
	                printf("%s ", msg);
	            }
	            else {
	                index = 1;
	                while (!(tache.x - oldX <= 100*index) && !(index > NB_COULEURS_BAS)) {
	                    if (isBlue)
	                        couleurTachesBas[index] = isBlue?ROUGE:BLEU;
	                    else {
	                        if (couleurTachesHaut[index] == isBlue?ROUGE:BLEU)
	                            printf("ROUGE : JE SUIS OK AVEC CETTE VALEUR \n");
	                        else
	                            printf("ROUGE : je suis pas OK avec cette valeur! \n");
	                    }
	                    printf("%s ", msgContraire);
	                    index++;
	                }
	                if (isBlue)
	                    couleurTachesHaut[index] = isBlue?BLEU:ROUGE;
	                else {
	                    if (couleurTachesHaut[index] == isBlue?BLEU:ROUGE)
	                        printf("ROUGE : JE SUIS OK AVEC CETTE VALEUR \n");
	                    else
	                        printf("ROUGE : je suis pas OK avec cette valeur! \n");
	                }
	                printf("%s ", msg);
	            }
	            oldX = tache.x;
	            index++;
	        }
	        for (int i=index; i<NB_COULEURS_BAS; i++) couleurTachesBas[i] = isBlue?ROUGE:BLEU;
	        printf("\n");
	    }
	}

	// On reverse le sens de calcul des images
	isBlue = !isBlue;
	// On flushe les listes
	for (int i=0; i<tachesBas.size(); i++) 	tachesBas.pop_back();
	for (int i=0; i<tachesHaut.size(); i++) tachesHaut.pop_back();
	*/


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
