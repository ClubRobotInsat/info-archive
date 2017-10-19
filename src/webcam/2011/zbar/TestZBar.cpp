// DisplayWebcam.cpp

#include "Clock.h"
#include "ImageFiltree.h"
#include "Webcam.h"
#include <GL/glfw.h>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <zbar.h>

using namespace std;
using namespace zbar;
// ---------------------------------------------------------------------

// Constantes qui définissent quelle partie de l'image originale on garde
#define MARGIN_Y_RELATIVE 0.3f
#define START_Y_RELATIVE MARGIN_Y_RELATIVE
#define END_Y_RELATIVE (1.0f - MARGIN_Y_RELATIVE)

// ---------------------------------------------------------------------
IWebcam* webcam = NULL;

GLuint id_texture_original = 0;
unsigned char* buffer_data = NULL;
unsigned char* zbar_data = NULL;
// ---------------------------------------------------------------------

void Display();
void TreatData(const unsigned char* input_data, unsigned char* displayed_data, int w, int h);
// ---------------------------------------------------------------------

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

	// Allocation de mémoire :
	// - buffer_data :
	size_t size = webcam->GetWidth() * webcam->GetHeight() * 3;
	buffer_data = new unsigned char[size];
	memset(buffer_data, 127, size);

	// - zbar_data :
	zbar_data = new unsigned char[webcam->GetWidth() * webcam->GetHeight()];

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
	delete[] zbar_data;

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

// ---------------------------------------------------------------------
void TreatData(const unsigned char* input_data, unsigned char* displayed_data, int w, int h) {
	static const int START_Y = std::max(0, int(float(h) * START_Y_RELATIVE));
	static const int END_Y = std::min(h, int(float(h) * END_Y_RELATIVE));

	// Recopie de la partie gardée vers le buffer visualisé
	// et conversion en N&B
	for(int y = START_Y; y < END_Y; y++) {
		for(int x = 0; x < w; x++) {
			int i = (x + y * w) * 3;
			unsigned char luminance = (unsigned char)(0.299 * double(input_data[i + 0]) + 0.587 * double(input_data[i + 0]) +
			                                          0.114 * double(input_data[i + 0]));

			displayed_data[i + 0] = luminance;
			displayed_data[i + 1] = luminance;
			displayed_data[i + 2] = luminance;
		}
	}

	// ---------------------------------------------------------------------
	// Ici commence la reconnaissance avec ZBar

	// On copie la sous-image à traiter dans un buffer au format Y800 (grayscale, 8 bits) :
	int zbar_w = w;
	int zbar_h = END_Y - START_Y;
	int zbar_size = zbar_w * zbar_h;

	for(int i = 0; i < zbar_size; i++)
		zbar_data[i] = displayed_data[(START_Y * w + i) * 3];

	// Création du scanner
	ImageScanner scanner;
	scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);

	// On wrappe les données dans un objet Image
	Image image(zbar_w, zbar_h, "Y800", zbar_data, zbar_size);

	// Scan !
	int nb_results = scanner.scan(image);

	// Affichage des résultats :
	if(nb_results != 0)
		cout << "nb_results == " << nb_results << endl;
	for(Image::SymbolIterator symbol = image.symbol_begin(); symbol != image.symbol_end(); ++symbol) {
		cout << "Symbol: type name: \"" << symbol->get_type_name() << "\" - data: \"" << symbol->get_data() << "\"" << endl;
	}

	// Cleanup
	image.set_data(NULL, 0);
}
