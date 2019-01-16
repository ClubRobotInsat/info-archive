//
//  display.cpp
//  Club Robot
//
//  Created by Rémi on 10/03/2015.
//

#include "../../third_parties/GLFW/include/GL/glfw.h"
#include "filtre.h"
#include <Units/TimePoint.h>
#include <log/Log.h>

std::unique_ptr<Lidar::Lidar> source1, source2;
std::unique_ptr<Lidar::Filtre> filtre1, filtre2;
bool running = true;

int savedFrames = 0;
int savedCount = 10;

typedef struct {
	double r; // percent
	double g; // percent
	double b; // percent
} rgb;

typedef struct {
	double h; // angle in degrees
	double s; // percent
	double v; // percent
} hsv;

rgb hsv2rgb(hsv in) {
	double hh, p, q, t, ff;
	long i;
	rgb out;

	if(in.s <= 0.0) { // < is bogus, just shuts up warnings
		out.r = in.v;
		out.g = in.v;
		out.b = in.v;
		return out;
	}
	hh = in.h;
	if(hh >= 360.0)
		hh = 0.0;
	hh /= 60.0;
	i = (long)hh;
	ff = hh - i;
	p = in.v * (1.0 - in.s);
	q = in.v * (1.0 - (in.s * ff));
	t = in.v * (1.0 - (in.s * (1.0 - ff)));

	switch(i) {
		case 0:
			out.r = in.v;
			out.g = t;
			out.b = p;
			break;
		case 1:
			out.r = q;
			out.g = in.v;
			out.b = p;
			break;
		case 2:
			out.r = p;
			out.g = in.v;
			out.b = t;
			break;

		case 3:
			out.r = p;
			out.g = q;
			out.b = in.v;
			break;
		case 4:
			out.r = t;
			out.g = p;
			out.b = in.v;
			break;
		case 5:
		default:
			out.r = in.v;
			out.g = p;
			out.b = q;
			break;
	}
	return out;
}


void display_frame(const Lidar::FrameLidar& mesure, float ox, float oy) {

	Angle prevAngle = mesure.begin + 180_deg;
	float prevAmpl = mesure.points[0].toM() * 500;

	glBegin(GL_TRIANGLES);
	for(size_t i = 1; i < mesure.points.size(); ++i) {
		Angle angle = i * mesure.angular_resolution + mesure.begin + 180_deg;
		float amplitude = mesure.points[i].toM() * 500;
		hsv couleur;
		couleur.h = mesure.points[i] / 4_m * 240;
		couleur.s = 1;
		couleur.v = 1;

		auto couleur2 = hsv2rgb(couleur);

		glColor3f(1, 0, 0);
		glVertex2d(ox, oy);
		glColor3f(couleur2.r, couleur2.g, couleur2.b);
		glVertex2d(amplitude * cos(angle) + ox, amplitude * sin(angle) + oy);
		glColor3f(couleur2.r, couleur2.g, couleur2.b);
		glVertex2d(prevAmpl * cos(prevAngle) + ox, prevAmpl * sin(prevAngle) + oy);

		prevAngle = angle;
		prevAmpl = amplitude;
	}
	glEnd();
}

void display() {


	// if(savedFrames < savedCount)
	//	mesure.save_to_file("/tmp" + std::to_string(savedFrames++));

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	int w, h;
	glfwGetWindowSize(&w, &h);
	gluOrtho2D(0.0, w, h, 0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(source1) {
		auto mesure = source1->get_frame();
		// display_frame(mesure, 300, 300);
		display_frame(filtre1->get_frame(mesure), 375, 500);
	}
	if(source2) {
		auto mesure = source2->get_frame();
		// display_frame(mesure, 300, 600);
		display_frame(filtre2->get_frame(mesure), 375, 300);
	}
}

void keyboard(int touche, int etat) {}

int main(int argc, char** argv) {
	glfwInit();
	Log::open(argc, argv, false);

	if(!glfwOpenWindow(1024, 720, 0, 0, 0, 0, 16, 0, GLFW_WINDOW)) {
		logError("impossible d'ouvrir une fenetre !");
		glfwTerminate();
		exit(-1);
	}

	glfwSetKeyCallback(&keyboard);
	glfwSwapInterval(0);

	// État que l'on considère comme l'état par défaut :
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);      // tri des faces : peut être activé en appuyant sur F. Améliore les performances
	                              // graphiques, mais donne un effet bizarre sur les modèles non prévus pour.
	glEnable(GL_TEXTURE_2D);      // texture : peut être désactivé en appuyant sur T
	glDisable(GL_LIGHTING);       // Lumière : peut être désactivé en appuyant sur L
	glDisable(GL_POLYGON_SMOOTH); // Antialiasing : peut être activé en appuyant sur A
	glDisable(GL_LINE_SMOOTH);    // Antialiasing : peut être activé en appuyant sur Z
	glEnable(GL_BLEND);           // Transparence : peut etre désactivé en appuyant sur B
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.0, 0.0, 0.0, 0.0);

	source1 = Lidar::Lidar::open_lidar(Lidar::Type::Sick);
	filtre1 = std::make_unique<Lidar::Filtre>();

	source2 = Lidar::Lidar::open_lidar(Lidar::Type::Hokuyo);
	filtre2 = std::make_unique<Lidar::Filtre>();

	TimePoint t0 = TimePoint::now(), fpsT = TimePoint::now();
	int frames = 0;
	while(running) {
		auto t = TimePoint::now();

		if((t - fpsT) > 1.0_s) {
			int fps = (int)((double)frames / (double)((t - fpsT).toS()));
			char buffer[256] = "";
			sprintf(buffer, "Affichage Lidar - FPS : %d", fps);
			glfwSetWindowTitle(buffer);
			fpsT = t;
			frames = 0;
		}

		// Affichage
		display();
		++frames;

		// Limitation des FPS
		Duration frameDuration = t - t0;
		if(frameDuration < 0.02_s)
			sleep(0.02_s - frameDuration);

		glfwSwapBuffers();

		running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
		t0 = TimePoint::now();
	}

	glfwCloseWindow();
	glfwTerminate();
	return 0;
}
