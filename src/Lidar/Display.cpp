/******************************************************************************/
/* info/robot/Lidar/affiche.cpp                                    2015-05-25 */
/* Club Robot INSA Toulouse                                      Félix Poisot */
/******************************************************************************/
#include "Display.h"
#include "../../third_parties/GLFW/include/GL/glfw.h"
#include <log/Log.h>
/******************************************************************************/

// On suppose que c'est la seule utilisation d'OpenGL par le programme.

const double ECHELLE = 180; // pixels par mètre
const Vector2f ORIG = {50, 400};


Display::Display(Vector2m table_size) : _table_size(table_size), _frames(0) /*_textureTable("table.tga")*/ {
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

	_t0 = TimePoint::now();
	_fpsT = TimePoint::now();
}

Display::~Display() {
	try {
		glfwCloseWindow();
		glfwTerminate();
	} catch(...) {
		logError("Problème dans Display()");
	}
}

bool Display::isClosed() {
	return glfwGetKey(GLFW_KEY_ESC) || !glfwGetWindowParam(GLFW_OPENED);
}

void Display::begin() {
	// mesure / affichage des FPS
	auto t = TimePoint::now();
	if((t - _fpsT) > 1.0_s) {
		int fps = static_cast<int>(static_cast<double>(_frames) / static_cast<double>((t - _fpsT).toS()));
		char buffer[256] = "";
		sprintf(buffer, "Affichage Lidar - FPS : %d", fps);
		glfwSetWindowTitle(buffer);
		_fpsT = t;
		_frames = 0;
	}

	// dessin à faire à chaque fois
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	double ox = ORIG.x;
	double oy = ORIG.y;
	double stepx = _table_size.x.toM() * ECHELLE;
	double stepy = _table_size.y.toM() * ECHELLE;
	// table
	glEnable(GL_TEXTURE_2D);
	// glBindTexture(GL_TEXTURE0, _textureTable.getID());
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

void Display::end() {
	++_frames;
	glfwSwapBuffers();
	_t0 = TimePoint::now();
}


void Display::frame_lidar(const FrameLidar& mesure, const repere::Coordinates& lidar_coords, Vec4 couleur) {
	double ox = ORIG.x + lidar_coords.getX().toM() * ECHELLE;
	double oy = ORIG.y - lidar_coords.getY().toM() * ECHELLE;

	Angle prevAngle = mesure.begin + lidar_coords.getAngle();
	double prevAmpl = mesure.points[0].toM() * ECHELLE;

	glBegin(GL_TRIANGLES);
	for(size_t i = 1; i < mesure.points.size(); ++i) {
		Angle angle = i * mesure.angularResolution + mesure.begin + lidar_coords.getAngle();
		double amplitude = mesure.points[i].toM() * ECHELLE;

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

void Display::grid(const OccupGrid& occ, Vec3 color) {
	double s = 3 / ECHELLE; // demi-taille du point

	glBegin(GL_QUADS);

	double ox = ORIG.x;
	double oy = ORIG.y;

	auto r = occ.resolution();
	for(int y = 0; y < r.y; ++y)
		for(int x = 0; x < r.x; ++x) {
			if(!occ(x, y)) {
				continue;
			}

			double mx = (x / static_cast<double>(r.x)) * _table_size.x.toM();
			double my = (y / static_cast<double>(r.y)) * _table_size.y.toM();

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

void Display::candidates(const std::vector<repere::Position>& pts, Vec3 color) {
	double s = 5 / ECHELLE; // demi-taille du point

	double ox = ORIG.x;
	double oy = ORIG.y;

	glBegin(GL_QUADS);

	for(auto v : pts) {
		auto x = v.getX().toM();
		auto y = v.getY().toM();

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
