// GLUtils.cpp
// CONVENTION :
// Pour les images (ImageFiltree), le pixel (0, 0) est en HAUT à gauche
// Pour les coordonnées OpenGL (que l'on passe aux fonctions DisplayXxx()) : (0, 0) est en BAS à gauche.

#include "GLUtils.h"

#define TAILLE_CROIX 5
//#define TAILLE_CARRE_CURSEUR 15

// Création et mise à jour de textures :
GLuint CreateTextureRGB(int w, int h, const unsigned char* data) {
	GLuint id = 0;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	return id;
}

void UpdateTextureRGB(GLuint id, int w, int h, const unsigned char* data) {
	glBindTexture(GL_TEXTURE_2D, id);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, data);
}

void UpdateTextureRGBInvert(GLuint id, int w, int h, const unsigned char* data) {
	unsigned char* p = const_cast<unsigned char*>(data);
	int size = w * h * 3;

	// Inverser les données :
	for(int i = 0; i < size; i++)
		p[i] = (unsigned char)(255 - int(p[i]));

	UpdateTextureRGB(id, w, h, p);

	// Réinverser les données, comme si on n'avait rien fait :
	for(int i = 0; i < size; i++)
		p[i] = (unsigned char)(255 - int(p[i]));
}

void DeleteTexture(GLuint* pid) {
	if(*pid != 0) {
		glDeleteTextures(1, pid);
		*pid = 0;
	}
}

// Préparation du contexte OpenGL pour pouvoir afficher en 2D, en coordonnées de pixels
void Setup2DDisplay(const Color& backgroundColor) {
	// Récupération de la taille de la fenêtre
	int winWidth = 0, winHeight = 0;
	glfwGetWindowSize(&winWidth, &winHeight);

	// Mise à jour du viewport
	glViewport(0, 0, winWidth, winHeight);

	// Matrice de projection : 2D
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, winWidth, 0, winHeight);

	// Matrice modelview
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Color buffer
	GLdouble r = GLfloat(backgroundColor.r) / 255.0f;
	GLdouble g = GLfloat(backgroundColor.g) / 255.0f;
	GLdouble b = GLfloat(backgroundColor.b) / 255.0f;

	glClearColor(r, g, b, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
}

// Affichage :
// - d'une texture dont les données sont déjà à jour :
void DisplayTexture(GLuint id, int x, int y, int w, int h) {
	// IMPORTANT : on inverse verticalement l'image !
	glBindTexture(GL_TEXTURE_2D, id);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1);
	glVertex2i(x + 0, y + 0);
	glTexCoord2f(1, 1);
	glVertex2i(x + w, y + 0);
	glTexCoord2f(1, 0);
	glVertex2i(x + w, y + h);
	glTexCoord2f(0, 0);
	glVertex2i(x + 0, y + h);
	glEnd();
}

// - de données brutes :
void DisplayData(const unsigned char* data, int dataWidth, int dataHeight, GLuint id, int x, int y, int w, int h) {
	UpdateTextureRGB(id, dataWidth, dataHeight, data);
	DisplayTexture(id, x, y, w, h);
}

// - d'une ImageFiltree :
void DisplayImage(ImageFiltree* image, GLuint id, int x, int y, int w, int h) {
	UpdateTextureRGB(id, image->GetWidth(), image->GetHeight(), image->GetDataRGB());
	DisplayTexture(id, x, y, w, h);
}

// - des rectangles représentant les taches d'une ImageFiltree :
void DisplayTaches(const ImageFiltree* image, int x, int y, int w, int h, const Color& color1, const Color& color2) {
	const list<ImageFiltree::Tache>& taches = image->GetListeTaches();
	list<ImageFiltree::Tache>::const_iterator itEnd = taches.end();

	for(list<ImageFiltree::Tache>::const_iterator it = taches.begin(); it != itEnd; it++) {
		DisplayTache(image, (*it), x, y, w, h, color1, color2);
	}
}

// - d'un rectangle :
// Convention : origine en bas à gauche.
void DisplayRect(int x, int y, int w, int h, const Color& color) {
	glDisable(GL_TEXTURE_2D);
	glColor3ub(color.r, color.g, color.b);

	// Affichage du rectangle
	glBegin(GL_LINE_LOOP);
	glVertex2i(x, y);
	glVertex2i(x + w, y);
	glVertex2i(x + w, y + h);
	glVertex2i(x, y + h);
	glEnd();

	glColor3ub(0xFF, 0xFF, 0xFF);
	glEnable(GL_TEXTURE_2D);
}

// - d'un rectangle en pointillés :
// Convention : origine en bas à gauche.
void DisplayRectStipple(int x, int y, int w, int h, const Color& color1, const Color& color2) {
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LINE_STIPPLE);

	for(int i = 0; i < 2; i++) {
		if(i == 0) {
			glColor3ub(color1.r, color1.g, color1.b);
			glLineStipple(1, 0x00FF);
		} else {
			glColor3ub(color2.r, color2.g, color2.b);
			glLineStipple(1, 0xFF00);
		}

		// Affichage du rectangle
		glBegin(GL_LINE_LOOP);
		glVertex2i(x, y);
		glVertex2i(x + w, y);
		glVertex2i(x + w, y + h);
		glVertex2i(x, y + h);
		glEnd();
	}

	glColor3ub(0xFF, 0xFF, 0xFF);
	glDisable(GL_LINE_STIPPLE);
	glEnable(GL_TEXTURE_2D);
}

// - d'un rectangle rempli :
// Convention : origine en bas à gauche.
void DisplayFilledRect(int x, int y, int w, int h, const Color& color) {
	glDisable(GL_TEXTURE_2D);
	glColor3ub(color.r, color.g, color.b);

	// Affichage du rectangle
	glBegin(GL_QUADS);
	glVertex2i(x, y);
	glVertex2i(x + w, y);
	glVertex2i(x + w, y + h);
	glVertex2i(x, y + h);
	glEnd();

	glColor3ub(0xFF, 0xFF, 0xFF);
	glEnable(GL_TEXTURE_2D);
}

// - d'une croix :
// Convention : origine en bas à gauche
void DisplayCroix(int x, int y, const Color& color) {
	glDisable(GL_TEXTURE_2D);
	glColor3ub(color.r, color.g, color.b);

	glBegin(GL_LINES);
	glVertex2i(x - TAILLE_CROIX, y);
	glVertex2i(x + TAILLE_CROIX, y);

	glVertex2i(x, y - TAILLE_CROIX);
	glVertex2i(x, y + TAILLE_CROIX);
	glEnd();

	glColor3ub(0xFF, 0xFF, 0xFF);
	glEnable(GL_TEXTURE_2D);
}

// - pour une tache donnée, d'un rectangle avec une croix pour le barycentre.
// (x, y, w, h) définissent la position sur l'écran de l'image à laquelle appartient la tache, avec l'origine
// Convention : position de l'image (x, y, w, h) : (0, 0) en bas à gauche.
//              position de la tache par rapport à l'image : (0, 0) en haut à gauche
// Cf la partie "CONVENTION" en haut de ce fichier.
void DisplayTache(const ImageFiltree* image, const ImageFiltree::Tache& tache, int x, int y, int w, int h, const Color& color1, const Color& color2) {
	int imgWidth = image->GetWidth();
	int imgHeight = image->GetHeight();

	float xFactor = float(w) / float(imgWidth);
	float yFactor = float(h) / float(imgHeight);

	// Coordonnées de la tache par rapport à l'image redimensionnée selon (xFactor, yFactor),
	// dans le repère où l'origine est en bas à gauche :
	int xTache = int(xFactor * float(tache.x));
	int yTache = int(yFactor * float(imgHeight - 1 - (tache.y + tache.h - 1)));

	int xTacheBary = int(xFactor * float(tache.x_barycentre));
	int yTacheBary = int(yFactor * float(imgHeight - 1 - tache.y_barycentre));

	int wTache = int(xFactor * float(tache.w));
	int hTache = int(yFactor * float(tache.h));

	// Affichage du carré
	DisplayRectStipple(x + xTache, y + yTache, wTache, hTache, color1, color2);

	// Affichage du barycentre :
	DisplayCroix(x + xTacheBary, y + yTacheBary, color2);
}

// - de la position de la souris :
void DisplayMousePos(const Color& color1, const Color& color2, int size) {
	int xMouse = 0, yMouse = 0;
	glfwGetMousePos(&xMouse, &yMouse);

	int winWidth = 0, winHeight = 0;
	glfwGetWindowSize(&winWidth, &winHeight);

	yMouse = winHeight - 1 - yMouse;

	DisplayRectStipple(xMouse - size / 2, yMouse - size / 2, size, size, color1, color2);
}

// - des positions cliquées :
void DisplayPositionsCliquees(const CalibrageWebcam& calibrage, int maisCourant, int camHeight, const Color& color) {
	int winWidth = 0, winHeight = 0;
	glfwGetWindowSize(&winWidth, &winHeight);

	int x, y, cote = 3;


	if(maisCourant > NB_MAIS_TOTAL) {
		maisCourant = 0;
		return;
	}

	glDisable(GL_TEXTURE_2D);
	glColor3ub(color.r, color.g, color.b);

	for(int i = 0; i < maisCourant; i++) {
		// Affichage du carré
		if(i < NB_MAIS_LATERAUX) {
			x = calibrage.maisLateraux[i].x;
			y = calibrage.maisLateraux[i].y;
		} else {
			x = calibrage.maisCentraux[i - NB_MAIS_LATERAUX].x;
			y = calibrage.maisCentraux[i - NB_MAIS_LATERAUX].y;
		}
		y += (winHeight - camHeight);

		glBegin(GL_QUADS);
		glVertex2i(x - cote, winHeight - (y + cote));
		glVertex2i(x + cote, winHeight - (y + cote));
		glVertex2i(x + cote, winHeight - (y - cote));
		glVertex2i(x - cote, winHeight - (y - cote));
		glEnd();
	}

	glColor3ub(0xFF, 0xFF, 0xFF);
	glEnable(GL_TEXTURE_2D);
}
