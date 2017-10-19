// Application.h

#ifndef APPLICATION_H
#define APPLICATION_H

#include "TGALoader.h"
#include "Webcam2010.h"
#include <GL/glfw.h>

class Application {
private:
	Webcam2010* webcam;
	CalibrageWebcam calibrage; // La fenetre GTK modifie ce calibrage-là plutot que celui
	                           // qui appartient à la Webcam2010.

	NDebug::CGTKwin GTKwin;

	GLuint idOriginal; // IDs des textures OpenGL
	GLuint idNoir;
	GLuint idBlanc;
	GLuint idImageMasquee;

	unsigned char* imageMasqueeData;

	int camWidth, camHeight; // Renseignées à la création de la webcam, on n'y touche plus après.

	bool phaseCalibrage;
	int maisCourant;

	bool afficherTaches;
	bool afficherMasque;
	bool afficherPositionsCliquees;

private:
	void AfficherAide();

	bool Init(int argc, char* argv[]);
	bool InitWebcam(int argc, char* argv[]);
	bool InitGL();
	bool InitGTK();

	void Cleanup();

	bool MainLoop();
	void CountFPS();
	void Update();
	void UpdateImageMasquee();
	void Display();
	// Retourne les coordonnees du barycentre de la tache la plus proche, si le point est dans une ou plusieurs taches,
	// (x, y) sinon.
	Vector2<int> PlusProcheTache(int x, int y);

public:
	void KeyCallback(int key, int state);
	void MouseCallback(int key, int state);

private:
	Application();

public:
	virtual ~Application() {} // On ne doit rien faire ici car on ne sait pas quand la destruction a lieu
	                          // (singleton). Utiliser Cleanup() a la place.
	static Application* GetInstance();

	int Run(int argc, char* argv[]);
};

#endif // APPLICATION_H
