// Calibration.cpp - programme permettant la calibration de la webcam pour 2009

#include "CGTKwin.h"
#include "ImageFiltree.h"
#include "Webcam.h"
#include "Webcam2009.h"
#include "log/Log.h"
#include <GL/glfw.h>
#include <stdio.h>
#include <stdlib.h>

///////////// Constantes //////////////

#define WIN_WIDTH 320 * 3
#define WIN_HEIGHT 240 * 2

#define TAILLE_CROIX 5

#define PARAMS_FILENAME_ROUGE "params_webcam_rouge.txt"
#define PARAMS_FILENAME_VERT "params_webcam_vert.txt"
#define MOUSE_POS_FILENAME "mouse_pos.txt"

#define TITLE "Calibration 2009"

///////////// Variables globales //////////////
IWebcam* webcam = NULL;
ImageFiltree image_rouge;
ImageFiltree image_vert;
NDebug::CGTKwin gtk_win;

bool super_flou_rouge = false;
bool super_flou_vert = false;

int x_mouse = 0, y_mouse = 0;
FILE* file_mouse_pos = NULL;

ImageFiltree::ParamsTaches params_rouge, params_vert;

bool running = true;

GLuint id_texture_original; // Texture de l'image vue par la webcam
GLuint id_texture_rouge;    // Texture de l'image binarisée pour le rouge
GLuint id_texture_vert;     // Texture de l'image binarisée pour le vert

GLuint id_texture_h;
GLuint id_texture_s;
GLuint id_texture_v;

///////////////////////////////////////////////

// Initialisation de la partie OpenGL
void InitGL();

// Initialisation de la fenêtre GTK
void InitGTK();

// Affichage d'une ImageFiltree
void DisplayImage(GLuint id, ImageFiltree* image_filtree, int x, int y, int w, int h);

// Affichage d'une texture
void DisplayTextureQuad(GLuint id, unsigned char* data_rgb, int x, int y, int w, int h);

// Calcul et affichage des FPS
void CalculerFPS();

// Fonction d'affichage
void Display();

// Fonction appelée lors d'un clic
void GLFWCALL OnMouseButton(int button, int action);

// Création d'une texture
GLuint CreerTexture(int w, int h);

// Affichage d'une tache
void DisplayCarre(int x_tache, int y_tache, int w_tache, int h_tache, int x_barycentre, int y_barycentre);

// RGB -> HSV
void CalculerHSV(const unsigned char* data_rgb, unsigned char* data_h, unsigned char* data_s, unsigned char* data_v, int w, int h);

///////////////////////////////////////////////

int main(int argc, char* argv[]) {
	// Création de la webcam
	webcam = NewWebcam(argc, argv);

	// Initialisation des images
	image_rouge.Init(webcam->GetWidth(), webcam->GetHeight());
	image_vert.Init(webcam->GetWidth(), webcam->GetHeight());

	// Chargement des paramètres
	ChoisirParametresImage(PARAMS_FILENAME_ROUGE, &image_rouge, &params_rouge, &super_flou_rouge);
	ChoisirParametresImage(PARAMS_FILENAME_VERT, &image_vert, &params_vert, &super_flou_vert);

	// Initialisation OpenGL :
	InitGL();

	// Initialisation de la fenêtre GTK :
	InitGTK();

	// Ouverture du fichier où l'on écrit les positions des clics de souris
	file_mouse_pos = fopen(MOUSE_POS_FILENAME, "w");
	if(!file_mouse_pos)
		logWarn("impossible d'ouvrir ", MOUSE_POS_FILENAME, " en ecriture !");

	// On demande la 1ère image
	if(!webcam->QueryImage()) {
		logError("QueryImage");
		exit(EXIT_FAILURE);
	}

	// Boucle principale :
	while(running) {
		// FPS
		CalculerFPS();

		// MàJ de la webcam
		webcam->WaitForImageSync();

		// Filtrage rouge et vert
		image_rouge.UpdateFromDataHSV(webcam->GetData());
		if(super_flou_rouge)
			image_rouge.SuperFlouterBlanc();
		else
			image_rouge.FlouterBlanc();

		image_rouge.RepererTaches(params_rouge);

		image_vert.UpdateFromDataHSV(webcam->GetData());
		if(super_flou_vert)
			image_vert.SuperFlouterBlanc();
		else
			image_vert.FlouterBlanc();
		image_vert.RepererTaches(params_vert);

		// Mise à jour de la position de la souris
		glfwGetMousePos(&x_mouse, &y_mouse);
		y_mouse = WIN_HEIGHT - y_mouse;

		// Affichage
		Display();

		// On demande à la webcam de récupérer l'image suivante
		if(!webcam->QueryImage()) {
			logError("QueryImage");
			exit(EXIT_FAILURE);
		}

		glfwSwapBuffers();

		// Condition de sortie
		running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);

		// Test d'erreurs OpenGL
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
			logError("erreur OpenGL : ", gluErrorString(error));
	}

	// Fermeture de la fenêtre GTK
	gtk_win.Terminate();

	// Fermeture de la fenêtre OpenGL / de GLFW
	glfwTerminate();

	// Fermeture de file_mouse_pos
	if(file_mouse_pos != NULL)
		fclose(file_mouse_pos);

	// Enregistrement des paramètres
	EcrireParametresImage(PARAMS_FILENAME_ROUGE, image_rouge, params_rouge, super_flou_rouge);
	EcrireParametresImage(PARAMS_FILENAME_VERT, image_vert, params_vert, super_flou_vert);

	// Suppression de la webcam
	delete webcam;

	return 0;
}

// Initialisation de la partie OpenGL
void InitGL() {
	int win_width = 0, win_height = 0;

	// Initialisation de GLFW
	glfwInit();

	// Ouverture de la fenêtre
	if(!glfwOpenWindow(WIN_WIDTH, WIN_HEIGHT, 0, 0, 0, 0, 0, 0, GLFW_WINDOW)) {
		logError("impossible d'ouvrir une fenêtre !");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwEnable(GLFW_STICKY_KEYS);
	glEnable(GL_TEXTURE_2D);
	glfwSwapInterval(0); // disable V-Sync

	// Callback de la souris
	glfwSetMouseButtonCallback(OnMouseButton);

	// Création des textures
	id_texture_original = CreerTexture(webcam->GetWidth(), webcam->GetHeight());
	id_texture_rouge = CreerTexture(webcam->GetWidth(), webcam->GetHeight());
	id_texture_vert = CreerTexture(webcam->GetWidth(), webcam->GetHeight());

	id_texture_h = CreerTexture(webcam->GetWidth(), webcam->GetHeight());
	id_texture_s = CreerTexture(webcam->GetWidth(), webcam->GetHeight());
	id_texture_v = CreerTexture(webcam->GetWidth(), webcam->GetHeight());

	// Récupération de la taille de la fenêtre et mise à jour du viewport
	glfwGetWindowSize(&win_width, &win_height);
	win_height = win_height > 0 ? win_height : 1;

	glViewport(0, 0, win_width, win_height);

	// Matrice de projection : 2D
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, win_width, 0, win_height);
}

// Initialisation de la fenêtre GTK :
void InitGTK() {
	gtk_win.Init();
	image_rouge.AddToGTK("Rouge", &gtk_win);
	gtk_win.Attach(params_rouge.width_min, "Rouge w min", 1, 320);
	gtk_win.Attach(params_rouge.height_min, "Rouge h min", 1, 240);
	gtk_win.Attach(params_rouge.aire_min, "Rouge aire min", 1, 320 * 240);
	gtk_win.Attach(params_rouge.width_max, "Rouge w max", 1, 320);
	gtk_win.Attach(params_rouge.height_max, "Rouge h max", 1, 240);
	gtk_win.Attach(params_rouge.aire_max, "Rouge aire max", 1, 320 * 240);
	gtk_win.Attach(super_flou_rouge, "Rouge super flou");

	image_vert.AddToGTK("Vert", &gtk_win);
	gtk_win.Attach(params_vert.width_min, "Vert w min", 1, 320);
	gtk_win.Attach(params_vert.height_min, "Vert h min", 1, 240);
	gtk_win.Attach(params_vert.aire_min, "Vert aire min", 1, 320 * 240);
	gtk_win.Attach(params_vert.width_max, "Vert w max", 1, 320);
	gtk_win.Attach(params_vert.height_max, "Vert h max", 1, 240);
	gtk_win.Attach(params_vert.aire_max, "Vert aire max", 1, 320 * 240);
	gtk_win.Attach(super_flou_vert, "Vert super flou");

	gtk_win.Attach(x_mouse, "X souris", -10, WIN_WIDTH + 10);
	gtk_win.Attach(y_mouse, "Y souris", -10, WIN_HEIGHT + 10);

	gtk_win.Run();
}

// Création d'une texture
GLuint CreerTexture(int w, int h) {
	GLuint id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	return id;
}

///////////////////////////////////////////////

// Affichage d'une ImageFiltree
void DisplayImage(GLuint id, ImageFiltree* image_filtree, int x, int y, int w, int h) {
	glBindTexture(GL_TEXTURE_2D, id);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, image_filtree->GetDataRGB());
	glPushMatrix();
	glTranslatef(x, y, 0);

	// Affichage de l'image
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex2i(0, 0);
	glTexCoord2f(1, 0);
	glVertex2i(w, 0);
	glTexCoord2f(1, 1);
	glVertex2i(w, h);
	glTexCoord2f(0, 1);
	glVertex2i(0, h);
	glEnd();

	// Affichage des taches
	const list<ImageFiltree::Tache>& taches = image_filtree->GetListeTaches();
	for(list<ImageFiltree::Tache>::const_iterator it = taches.begin(); it != taches.end(); it++) {
		DisplayCarre((*it).x, (*it).y, (*it).w, (*it).h, (*it).x_barycentre, (*it).y_barycentre);
	}

	glPopMatrix();

	// Affichage des taches dans la zone non translatée
	for(list<ImageFiltree::Tache>::const_iterator it = taches.begin(); it != taches.end(); it++) {
		DisplayCarre((*it).x, (*it).y, (*it).w, (*it).h, (*it).x_barycentre, (*it).y_barycentre);
	}
}

///////////////////////////////////////////////

void CalculerFPS() {
	static bool first_time = true;
	static int nb_frames = 0;
	static double t0 = 0.0;

	if(first_time) {
		t0 = glfwGetTime();
		first_time = false;
	}

	nb_frames++;

	if(glfwGetTime() - t0 >= 1.0) {
		char s[255] = "";
		t0 = glfwGetTime();

		sprintf(s, "FPS : %d - %s", nb_frames, TITLE);
		glfwSetWindowTitle(s);

		nb_frames = 0;
	}
}

///////////////////////////////////////////////

// Fonction d'affichage
void Display() {
	int w = webcam->GetWidth();
	int h = webcam->GetHeight();

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);

	// Image de ce que voit la webcam :
	glBindTexture(GL_TEXTURE_2D, id_texture_original);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, webcam->GetData());
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex2i(0, 0);
	glTexCoord2f(1, 0);
	glVertex2i(w, 0);
	glTexCoord2f(1, 1);
	glVertex2i(w, h);
	glTexCoord2f(0, 1);
	glVertex2i(0, h);
	glEnd();

	// Images filtrées : rouge et vert
	DisplayImage(id_texture_rouge, &image_rouge, w, 0, w, h);
	DisplayImage(id_texture_vert, &image_vert, 2 * w, 0, w, h);

	// Récupération de l'image en HSV
	unsigned char* data_h = new unsigned char[webcam->GetWidth() * webcam->GetHeight() * 3];
	unsigned char* data_s = new unsigned char[webcam->GetWidth() * webcam->GetHeight() * 3];
	unsigned char* data_v = new unsigned char[webcam->GetWidth() * webcam->GetHeight() * 3];
	CalculerHSV(webcam->GetData(), data_h, data_s, data_v, webcam->GetWidth(), webcam->GetHeight());

	// Affichage de H, S et V au-dessus
	DisplayTextureQuad(id_texture_h, data_h, w * 0, h, w, h);
	DisplayTextureQuad(id_texture_s, data_s, w * 1, h, w, h);
	DisplayTextureQuad(id_texture_v, data_v, w * 2, h, w, h);

	delete[] data_h;
	delete[] data_s;
	delete[] data_v;
}

///////////////////////////////////////////////

// Fonction d'affichage d'une tache
void DisplayCarre(int x_tache, int y_tache, int w_tache, int h_tache, int x_barycentre, int y_barycentre) {
	glDisable(GL_TEXTURE_2D);
	glColor3f(0.0f, 1.0f, 0.5f);

	// Affichage du carré
	glBegin(GL_LINE_LOOP);
	glVertex2i(x_tache, y_tache + h_tache);
	glVertex2i(x_tache + w_tache, y_tache + h_tache);
	glVertex2i(x_tache + w_tache, y_tache);
	glVertex2i(x_tache, y_tache);
	glEnd();

	// Affichage du barycentre :
	glBegin(GL_LINES);
	glVertex2i(x_barycentre - TAILLE_CROIX, y_barycentre);
	glVertex2i(x_barycentre + TAILLE_CROIX, y_barycentre);
	glVertex2i(x_barycentre, y_barycentre + TAILLE_CROIX);
	glVertex2i(x_barycentre, y_barycentre - TAILLE_CROIX);
	glEnd();

	glColor3f(1.0f, 1.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
}

///////////////////////////////////////////////

// Affichage d'une texture
void DisplayTextureQuad(GLuint id, unsigned char* data_rgb, int x, int y, int w, int h) {
	glBindTexture(GL_TEXTURE_2D, id);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, data_rgb);
	glPushMatrix();
	glTranslatef(x, y, 0);

	// Affichage de l'image
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex2i(0, 0);
	glTexCoord2f(1, 0);
	glVertex2i(w, 0);
	glTexCoord2f(1, 1);
	glVertex2i(w, h);
	glTexCoord2f(0, 1);
	glVertex2i(0, h);
	glEnd();

	glPopMatrix();
}

///////////////////////////////////////////////

// Fonction appelée lors d'un clic
void GLFWCALL OnMouseButton(int button, int action) {
	if(action == GLFW_RELEASE && file_mouse_pos != NULL) {
		logInfo("clic : ", x_mouse, ", ", y_mouse);
		fprintf(file_mouse_pos, "%d, %d\n", x_mouse, y_mouse);
	}
}

///////////////////////////////////////////////

// Restreindre une valeur à un sol et un plafond
/*static double clamp(double value, double min, double max)
{
    if(value < min)
        value = min;
    if(value > max)
        value = max;
    return value;
}

static void YUVToRGB(unsigned char y, unsigned char u, unsigned char v,
                                unsigned char* pr, unsigned char* pg, unsigned char* pb)
{
    // Coeffs tirés de la page Wikipedia "YUV" - donne un effet assez stylé mais pas du tout voulu ^^
    // *pr = (unsigned char)(float(y) - 0.00004f * float(u) + 1.140f  * float(v));
    // *pg = (unsigned char)(float(y) - 0.395f   * float(u) - 0.581f  * float(v));
    // *pb = (unsigned char)(float(y) + 2.032f   * float(u) - 0.0005f * float(v));

    // Coeffs tirés de http://people.via.ecp.fr/~remi/ecp/tpi/rapport/yuv.html :
    *pr = (unsigned char)(clamp(double(y) - 0.0009267*(double(u)-128.0) + 1.4016868*(double(v)-128.0), 0.0, 255.0));
    *pg = (unsigned char)(clamp(double(y) - 0.3436954*(double(u)-128.0) - 0.7141690*(double(v)-128.0), 0.0, 255.0));
    *pb = (unsigned char)(clamp(double(y) + 1.7721604*(double(u)-128.0) + 0.0009902*(double(v)-128.0), 0.0, 255.0));
}
*/

// RGB -> HSV
void CalculerHSV(const unsigned char* data_rgb, unsigned char* data_h, unsigned char* data_s, unsigned char* data_v, int w, int h) {
	for(int x = 0; x < w; x++) {
		for(int y = 0; y < h; y++) {
			int i = (x + y * w) * 3;
			unsigned char r = data_rgb[i + 0];
			unsigned char g = data_rgb[i + 1];
			unsigned char b = data_rgb[i + 2];

			ColorHSV hsv(Color(r, g, b));

			data_h[i + 0] = data_h[i + 1] = data_h[i + 2] = hsv.h;
			data_s[i + 0] = data_s[i + 1] = data_s[i + 2] = hsv.s;
			data_v[i + 0] = data_v[i + 1] = data_v[i + 2] = hsv.v;
		}
	}
}
