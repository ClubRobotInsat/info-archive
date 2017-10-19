// DisplayWebcam.cpp

#include "Clock.h"
#include "Webcam.h"
#include <GL/glfw.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

using namespace std;

IWebcam* webcam;

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

	// Boucle principale
	while(running) {
		t = glfwGetTime();
		glfwGetMousePos(&x, &y);

		// Calcul des FPS
		if((t - t0) > 1.0 || frames == 0) {
			fps = (int)((double)frames / (double)(t - t0));
			sprintf(titre, "Display webcam - FPS : %d", fps);
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
	TreatData(webcam->GetData(), buffer_data, w, h);

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
