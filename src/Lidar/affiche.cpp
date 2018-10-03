/******************************************************************************/
/* info/robot/Lidar/affiche.cpp                                    2015-05-25 */
/* Club Robot INSA Toulouse                                      Félix Poisot */
/******************************************************************************/
#include "affiche.h"
#include <GLFW/include/GL/glfw.h>
#include <log/Log.h>
/******************************************************************************/

// On suppose que c'est la seule utilisation d'OpenGL par le programme.

const double ECHELLE = 180; // pixels par mètre
const Vec2 ORIG = {50, 400};


Affiche::Affiche(Vec2 tailleTable) : _textureTable("table.tga") {
	glfwInit();

	if(!glfwOpenWindow(640, 480, 0, 0, 0, 0, 16, 0, GLFW_WINDOW)) {
		glfwTerminate();
		throw std::runtime_error("Impossible d'ouvrir une fenêtre");
	}

	glfwSwapInterval(0);

	// État que l'on considère comme l'état par défaut :
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);      // tri des faces : peut être activé en appuyant sur F. Améliore les performances
	                              // graphiques, mais donne un effet bizarre sur les modèles non prévus pour.
	glDisable(GL_LIGHTING);       // Lumière : peut être désactivé en appuyant sur L
	glDisable(GL_POLYGON_SMOOTH); // Antialiasing : peut être activé en appuyant sur A
	glDisable(GL_LINE_SMOOTH);    // Antialiasing : peut être activé en appuyant sur Z
	glEnable(GL_BLEND);           // Transparence : peut etre désactivé en appuyant sur B
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.0, 0.0, 0.0, 0.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	int w, h;
	glfwGetWindowSize(&w, &h);
	gluOrtho2D(0.0, w, h, 0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	_size = tailleTable;

	_t0 = TimePoint::now();
	_fpsT = TimePoint::now();
	_frames = 0;
}

Affiche::~Affiche() {
	try {
		glfwCloseWindow();
		glfwTerminate();
	} catch(...) {
		logError("Problème dans ~Affiche()");
	}
}

bool Affiche::isClosed() {
	return glfwGetKey(GLFW_KEY_ESC) || !glfwGetWindowParam(GLFW_OPENED);
}

void Affiche::begin() {
	// mesure / affichage des FPS
	auto t = TimePoint::now();
	if((t - _fpsT) > 1.0_s) {
		int fps = (int)((double)_frames / (double)((t - _fpsT).toS()));
		char buffer[256] = "";
		sprintf(buffer, "Affichage Lidar - FPS : %d", fps);
		glfwSetWindowTitle(buffer);
		_fpsT = t;
		_frames = 0;
	}

	// dessin à faire à chaque fois
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float ox = ORIG.x;
	float oy = ORIG.y;
	float stepx = _size.x * ECHELLE;
	float stepy = _size.y * ECHELLE;
	// table
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE0, _textureTable.getID());
	glBegin(GL_QUADS);
	glColor3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0, 0);
	glVertex2d(ox, oy);
	glColor3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0, 1);
	glVertex2d(ox, oy - stepy);
	glColor3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(1, 1);
	glVertex2d(ox + stepx, oy - stepy);
	glColor3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(1, 0);
	glVertex2d(ox + stepx, oy);
	glEnd();
	glBindTexture(GL_TEXTURE0, 0);
	glDisable(GL_TEXTURE_2D);
}

void Affiche::end() {
	++_frames;
	glfwSwapBuffers();
	_t0 = TimePoint::now();
}


void Affiche::trameLidar(const TrameLidar& mesure, Vec2 orig, Angle orient, Vec4 couleur) {
	float ox = ORIG.x + orig.x * ECHELLE;
	float oy = ORIG.y - orig.y * ECHELLE;

	Angle prevAngle = mesure.begin + orient;
	float prevAmpl = mesure.points[0].toM() * ECHELLE;

	glBegin(GL_TRIANGLES);
	for(size_t i = 1; i < mesure.points.size(); ++i) {
		Angle angle = i * mesure.angularResolution + mesure.begin + orient;
		float amplitude = mesure.points[i].toM() * ECHELLE;

		glColor4f(couleur.r, couleur.g, couleur.b, couleur.a);
		glVertex2d(ox, oy);
		glColor4f(couleur.r, couleur.g, couleur.b, couleur.a);
		glVertex2d(amplitude * cos(angle) + ox, amplitude * -sin(angle) + oy);
		glColor4f(couleur.r, couleur.g, couleur.b, couleur.a);
		glVertex2d(prevAmpl * cos(prevAngle) + ox, prevAmpl * -sin(prevAngle) + oy);

		prevAngle = angle;
		prevAmpl = amplitude;
	}
	glEnd();
}

void Affiche::grille(const OccupGrid& occ, Vec3 color) {
	float s = 3 / (float)ECHELLE; // demi-taille du point

	glBegin(GL_QUADS);

	float ox = ORIG.x;
	float oy = ORIG.y;

	auto r = occ.resolution();
	for(int y = 0; y < r.y; ++y)
		for(int x = 0; x < r.x; ++x) {
			if(!occ(x, y))
				continue;

			float mx = (x / (float)r.x) * _size.x;
			float my = (y / (float)r.y) * _size.y;

			glColor3f(color.r, color.g, color.b);
			glVertex2d(ox + (mx - s) * ECHELLE, oy - (my - s) * ECHELLE);
			glColor3f(color.r, color.g, color.b);
			glVertex2d(ox + (mx - s) * ECHELLE, oy - (my + s) * ECHELLE);
			glColor3f(color.r, color.g, color.b);
			glVertex2d(ox + (mx + s) * ECHELLE, oy - (my + s) * ECHELLE);
			glColor3f(color.r, color.g, color.b);
			glVertex2d(ox + (mx + s) * ECHELLE, oy - (my - s) * ECHELLE);
		}

	glEnd();
}

void Affiche::candidats(std::vector<Vec2> pts, Vec3 color) {
	float s = 5 / (float)ECHELLE; // demi-taille du point

	float ox = ORIG.x;
	float oy = ORIG.y;

	glBegin(GL_QUADS);

	for(auto v : pts) {
		auto x = v.x;
		auto y = v.y;

		glColor3f(color.r, color.g, color.b);
		glVertex2d(ox + (x - s) * ECHELLE, oy - (y - s) * ECHELLE);
		glColor3f(color.r, color.g, color.b);
		glVertex2d(ox + (x - s) * ECHELLE, oy - (y + s) * ECHELLE);
		glColor3f(color.r, color.g, color.b);
		glVertex2d(ox + (x + s) * ECHELLE, oy - (y + s) * ECHELLE);
		glColor3f(color.r, color.g, color.b);
		glVertex2d(ox + (x + s) * ECHELLE, oy - (y - s) * ECHELLE);
	}

	glEnd();
}
