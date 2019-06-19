// Calibrage.cpp (2010)

int main() {
	return 0;
}

#ifdef toto

#include "../../maths/Vector2.h"
#include "CGTKwin.h"
#include "ImageFiltree.h"
#include "Socket.h"
#include "TGALoader.h"
#include "Webcam2010.h"
#include "log/Log.h"
#include <GL/glfw.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#define TAILLE_CROIX 5
#define COULEUR_CARRE_NOIR 1.0f, 0.0f, 0.0f  // carré pour les taches noires
#define COULEUR_CARRE_BLANC 0.0f, 1.0f, 0.5f // carré pour les taches blanches
#define TAILLE_CARRE_CURSEUR 15

using namespace std;

// Variables globales :
IWebcam* webcam;
ImageFiltree image_noir;   // Pour les mais noir
ImageFiltree image_blanc;  // Pour les mais blancs
CalibrageWebcam calibrage; // Les paramètres que l'on veut lire / écrire

// Marge autorisee autour d'une couleur moyenne pour les seuils de detection des taches
int ecartCouleurNoir = 20;
int ecartCouleurBlanc = 20;

bool phaseCalibrage = false;
int maisCourant = 0;

NDebug::CGTKwin gtk_win;

GLuint id_texture_original = 0;
GLuint id_texture_noir = 0;
GLuint id_texture_blanc = 0;
GLuint id_texture_masque = 0;

bool calibrationModeTCPIP;
char* calibrationDevice;
Socket* socketConfiguration;

// Prototypes :
void AfficherAide();
void InitCalibrage();
void InitGL();
void InitGTK();
void MajCalibrage();
void LoadMask(const string& mask_filename);

void Display();
GLuint CreateTexture(int w, int h, unsigned char* data = NULL);
void DisplayTexture(GLuint id, int x, int y, int w, int h);
void DisplayTexture(unsigned char* data, GLuint id, int x, int y, int w, int h);
void DisplayTexture(ImageFiltree* image_filtree, GLuint id, int x, int y, int w, int h);
void DisplayTaches(ImageFiltree* image_filtree, int x, int y, int w, int h, GLfloat r, GLfloat g, GLfloat b);
void DisplayTache(int x_tache, int y_tache, int w_tache, int h_tache, int x_barycentre, int y_barycentre, GLfloat r, GLfloat g, GLfloat b);
void DisplayMouse();
void DisplayPositionsPointees();

void GLFWCALL KeyCallback(int key, int state);
void GLFWCALL MouseCallback(int key, int state);

int clamp(int value, int min, int max) {
	if(value < min)
		value = min;
	if(value > max)
		value = max;
	return value;
}

void setFichierCalibration(char* camconf) {
	int ip_server[4];

	// teste si c un fichier
	if(strstr(camconf, ".conf") != NULL) {
		calibrationModeTCPIP = false;
		return;
	}
	calibrationModeTCPIP = true;

	// connection au server du fichier de configuration
	// Récupération de l'adresse IP et du port à utiliser
	int port;
	int nbrEntierParse = sscanf(camconf, "%d.%d.%d.%d:%d", &ip_server[0], &ip_server[1], &ip_server[2], &ip_server[3], &port);

	if(nbrEntierParse == 0) {
		ip_server[0] = 10;
		ip_server[1] = 30;
		ip_server[2] = 5;
		ip_server[3] = 42;
		nbrEntierParse = sscanf(camconf, "%d", &port);
		if(nbrEntierParse == 0)
			port = 4243;
	}

	// Connexion au serveur
	char ipStr[80];
	sprintf(ipStr, "%d.%d.%d.%d", ip_server[0], ip_server[1], ip_server[2], ip_server[3]);
	printf("WebcamTCPIPMJPG : connexion au serveur %s, port %d\n", ipStr, port);
	Socket::Init();
	socketConfiguration = new Socket();
	socketConfiguration->Connect(ipStr, port);
}

// -------------------------------------------
int main(int argc, char* argv[]) {
	int x, y;
	int fps = 0, frames;
	double t0, t;
	char titre[200] = "";
	bool running = true;

	AfficherAide();

	// Récupération des arguments et création de la webcam
	webcam = NewWebcam(argc, argv);

	if(!webcam)
		return -1;

	// cherche l'argument de calibration
	int argCalib = 0;
	while(argCalib < argc && strcmp(argv[argCalib], "CAMCONF") == 0)
		;
	if(argCalib != argc) {
		cout << "Vous devez donner le nom du fichier de calibration avec la directive CAMCONF" << endl
		     << "exemple : " << argv[0] << " CAMCONF webcam.conf" << endl
		     << "          " << argv[0] << " CAMCONF 127.0.0.1:4243" << endl
		     << "          " << argv[0] << " CAMCONF 4243" << endl;
		return -1;
	}

	// recupere le fichier ou le port TCP
	argCalib++;
	calibrationDevice = argv[argCalib];
	setFichierCalibration(calibrationDevice);

	// Initialisation de GLFW
	glfwInit();

	int webcam_w = webcam->GetWidth();
	int webcam_h = webcam->GetHeight();
	if(!glfwOpenWindow(webcam_w, webcam_h + webcam_h / 2, 0, 0, 0, 0, 0, 0, GLFW_WINDOW)) {
		printf("Erreur : impossible d'ouvrir une fenêtre !\n");
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwEnable(GLFW_STICKY_KEYS);
	glfwSwapInterval(0); // disable V-Sync
	glfwSetKeyCallback(KeyCallback);
	glfwSetMouseButtonCallback(MouseCallback);

	// Création des textures, etc :
	InitGL();

	// Création des images filtrées, etc :
	InitCalibrage();

	// Initialisation de la fenetre GTK :
	InitGTK();

	running = true;
	frames = 0;
	t0 = glfwGetTime();

	// Boucle principale
	while(running) {
		t = glfwGetTime();
		glfwGetMousePos(&x, &y);

		// Calcul des FPS
		if((t - t0) > 1.0 || frames == 0) {
			fps = (int)((double)frames / (double)(t - t0));
			sprintf(titre, "Calibrage 2010 - FPS : %d", fps);
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

		// Update : copie des paramètres de calibrage dans les images
		image_noir.SetColorMin(calibrage.rgb_noir_min);
		image_noir.SetColorMax(calibrage.rgb_noir_max);

		image_blanc.SetColorMin(calibrage.rgb_blanc_min);
		image_blanc.SetColorMax(calibrage.rgb_blanc_max);

		MajCalibrage();

		// Application du masque, binarisation, floutage et repérage des taches :
		image_noir.UpdateFromData(webcam->GetData());
		image_noir.SuperFlouterBlanc();
		image_noir.RepererTaches(calibrage.params_noir);

		image_blanc.UpdateFromData(webcam->GetData());
		image_blanc.SuperFlouterBlanc();
		image_blanc.RepererTaches(calibrage.params_blanc);

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

	gtk_win.Terminate();

	glfwTerminate();

	webcam->Close();
	socketConfiguration->Destroy();

	delete webcam;

	return 0;
}

// -------------------------------------------
void AfficherAide() {
	cout << "==============" << endl
	     << "Utilisation" << endl
	     << "==============" << endl
	     << "Entree : enregistrer les parametres" << endl
	     << "L (load) : recharger les parametres depuis un fichier" << endl
	     << "M (masque) : charger un masque" << endl
	     << "C (calibrage) : acquerir les positions des mais a la souris" << endl
	     << "Z : annuler un clic sur un mais" << endl
	     << "==============" << endl;
}

// -------------------------------------------
void InitGL() {
	int w = webcam->GetWidth();
	int h = webcam->GetHeight();

	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);

	// Creation des textures :
	id_texture_original = CreateTexture(w, h);
	id_texture_noir = CreateTexture(w, h);
	id_texture_blanc = CreateTexture(w, h);
	id_texture_masque = CreateTexture(w, h);
}

// -------------------------------------------
void InitCalibrage() {
	int w = webcam->GetWidth();
	int h = webcam->GetHeight();

	// Initialisation des "ImageFiltree"
	image_noir.Init(w, h);
	image_blanc.Init(w, h);

	// Chargement du fichier de calibrage
	if(calibrationModeTCPIP) {
		socketConfiguration->SendMsg("GET", strlen("GET") + 1);

		char bufferFichierConf[10240];
		int tailleFichierConf = socketConfiguration->ReceiveMsg(bufferFichierConf, 10240);
		bufferFichierConf[tailleFichierConf] = '\0';

		istringstream oss117(bufferFichierConf);
		calibrage.Lire(oss117, webcam->GetWidth(), webcam->GetHeight());
	} else {
		if(!calibrage.Lire(calibrationDevice, w, h))
			exit(EXIT_FAILURE);
	}

	// Copie des params RGB depuis "calibrage vers image_noir / image_blanc :
	image_noir.SetColorMin(calibrage.rgb_noir_min);
	image_noir.SetColorMax(calibrage.rgb_noir_max);

	image_blanc.SetColorMin(calibrage.rgb_blanc_min);
	image_blanc.SetColorMax(calibrage.rgb_blanc_max);

	// Initialisation du masque :
	LoadMask(calibrage.mask_filename);
}

// -------------------------------------------
void InitGTK() {
	gtk_win.Init();

	gtk_win.Attach(ecartCouleurNoir, "Ecart couleur noir", 0, 255);
	gtk_win.Attach(ecartCouleurBlanc, "Ecart couleur blanc", 0, 255);

	// - min: w, h, aire
	gtk_win.Attach(calibrage.params_blanc.width_min, "Largeur min", 1, webcam->GetWidth());
	gtk_win.Attach(calibrage.params_blanc.height_min, "Hauteur min", 1, webcam->GetHeight());

	// - max: w, h, aire
	gtk_win.Attach(calibrage.params_blanc.width_max, "Largeur max", 1, webcam->GetWidth());
	gtk_win.Attach(calibrage.params_blanc.height_max, "Hauteur max", 1, webcam->GetHeight());


	// Noir :
	// - RGB min
	gtk_win.Attach(calibrage.rgb_noir_min.r, "Noir R min", 0, 255);
	gtk_win.Attach(calibrage.rgb_noir_min.g, "Noir G min", 0, 255);
	gtk_win.Attach(calibrage.rgb_noir_min.b, "Noir B min", 0, 255);

	// - RGB max
	gtk_win.Attach(calibrage.rgb_noir_max.r, "Noir R max", 0, 255);
	gtk_win.Attach(calibrage.rgb_noir_max.g, "Noir G max", 0, 255);
	gtk_win.Attach(calibrage.rgb_noir_max.b, "Noir B max", 0, 255);

	// Blanc :
	// - RGB min
	gtk_win.Attach(calibrage.rgb_blanc_min.r, "Blanc R min", 0, 255);
	gtk_win.Attach(calibrage.rgb_blanc_min.g, "Blanc G min", 0, 255);
	gtk_win.Attach(calibrage.rgb_blanc_min.b, "Blanc B min", 0, 255);

	// - RGB max
	gtk_win.Attach(calibrage.rgb_blanc_max.r, "Blanc R max", 0, 255);
	gtk_win.Attach(calibrage.rgb_blanc_max.g, "Blanc G max", 0, 255);
	gtk_win.Attach(calibrage.rgb_blanc_max.b, "Blanc B max", 0, 255);

	gtk_win.Run();
}

// Mise a jour de tous les parametres de calibrage
void MajCalibrage() {
	calibrage.params_blanc.aire_min = calibrage.params_blanc.width_min * calibrage.params_blanc.height_min;
	calibrage.params_blanc.aire_max = calibrage.params_blanc.width_max * calibrage.params_blanc.height_max;

	calibrage.params_noir.aire_min = calibrage.params_blanc.aire_min;
	calibrage.params_noir.aire_max = calibrage.params_blanc.aire_max;

	calibrage.params_noir.width_min = calibrage.params_blanc.width_min;
	calibrage.params_noir.height_min = calibrage.params_blanc.height_min;
	calibrage.params_noir.aire_min = calibrage.params_blanc.aire_min;

	calibrage.params_noir.width_max = calibrage.params_blanc.width_max;
	calibrage.params_noir.height_max = calibrage.params_blanc.height_max;
	calibrage.params_noir.aire_max = calibrage.params_blanc.aire_max;


	int rMoyNoir = (calibrage.rgb_noir_min.r + calibrage.rgb_noir_max.r) / 2;
	int gMoyNoir = (calibrage.rgb_noir_min.g + calibrage.rgb_noir_max.g) / 2;
	int bMoyNoir = (calibrage.rgb_noir_min.b + calibrage.rgb_noir_max.b) / 2;

	int rMoyBlanc = (calibrage.rgb_blanc_min.r + calibrage.rgb_blanc_max.r) / 2;
	int gMoyBlanc = (calibrage.rgb_blanc_min.g + calibrage.rgb_blanc_max.g) / 2;
	int bMoyBlanc = (calibrage.rgb_blanc_min.b + calibrage.rgb_blanc_max.b) / 2;

	calibrage.rgb_noir_min.r = clamp(rMoyNoir - ecartCouleurNoir, 0, 255);
	calibrage.rgb_noir_min.g = clamp(gMoyNoir - ecartCouleurNoir, 0, 255);
	calibrage.rgb_noir_min.b = clamp(bMoyNoir - ecartCouleurNoir, 0, 255);

	calibrage.rgb_noir_max.r = clamp(rMoyNoir + ecartCouleurNoir, 0, 255);
	calibrage.rgb_noir_max.g = clamp(gMoyNoir + ecartCouleurNoir, 0, 255);
	calibrage.rgb_noir_max.b = clamp(bMoyNoir + ecartCouleurNoir, 0, 255);

	calibrage.rgb_blanc_min.r = clamp(rMoyBlanc - ecartCouleurBlanc, 0, 255);
	calibrage.rgb_blanc_min.g = clamp(gMoyBlanc - ecartCouleurBlanc, 0, 255);
	calibrage.rgb_blanc_min.b = clamp(bMoyBlanc - ecartCouleurBlanc, 0, 255);

	calibrage.rgb_blanc_max.r = clamp(rMoyBlanc + ecartCouleurBlanc, 0, 255);
	calibrage.rgb_blanc_max.g = clamp(gMoyBlanc + ecartCouleurBlanc, 0, 255);
	calibrage.rgb_blanc_max.b = clamp(bMoyBlanc + ecartCouleurBlanc, 0, 255);
}

// -------------------------------------------
void LoadMask(const string& mask_filename) {
	TGALoader tga;
	string filename = mask_filename;

	int w = webcam->GetWidth();
	int h = webcam->GetHeight();

	// S'il n'y a pas de nom de fichier précisé, on en demande un à l'utilisateur :
	if(filename.empty()) {
		char buffer[40];
		cout << "Nom du fichier TGA de masque(entree s'il n'y en a pas) : " << flush;
		cin.getline(buffer, sizeof(buffer));
		filename = buffer;
	}

	// Si l'utilisateur n'a pas précisé de nom, on enlève le masque
	if(filename.empty()) {
		image_noir.SetMaskData(NULL);
		image_blanc.SetMaskData(NULL);
		return;
	}

	// Chargement du fichier TGA :
	TGAErrorCode error = tga.loadFile(filename.c_str());
	if(error != TGA_OK) {
		logError("Chargement du masque \"", filename, "\" : erreur TGA : ", error);
		image_noir.SetMaskData(NULL);
		image_blanc.SetMaskData(NULL);
	}

	// Tests de conformité :
	// - taille du masque :
	if(int(tga.getWidth()) != w || int(tga.getHeight()) != h) {
		logError("Dimensions du masque differentes de la resolution webcam");
		image_noir.SetMaskData(NULL);
		image_blanc.SetMaskData(NULL);
	}

	// - BPP :
	if(tga.getBpp() != 3) {
		logError("Nombre de BPP different de 3, attention a bien supprimer le canal alpha lors de l'enregistrement du "
		         "masque !");
		image_noir.SetMaskData(NULL);
		image_blanc.SetMaskData(NULL);
	}

	// Application du masque :
	image_noir.SetMaskDataRGB(tga.getData());
	image_blanc.SetMaskDataRGB(tga.getData());

	// Chargement des données du masque dans la texture :
	glBindTexture(GL_TEXTURE_2D, id_texture_masque);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, tga.getData());

	tga.free();
}

// -------------------------------------------
void Display() {
	glColor3ub(0xFF, 0xFF, 0xFF);
	glEnable(GL_TEXTURE_2D);

	const int w = webcam->GetWidth(), h = webcam->GetHeight();

	// Récupération de la taille de la fenêtre
	int win_width, win_height;
	glfwGetWindowSize(&win_width, &win_height);
	win_height = win_height > 0 ? win_height : 1;

	// Mise à jour du viewport
	glViewport(0, 0, win_width, win_height);

	// Matrice de projection : 2D
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, win_width, 0, win_height);

	// Matrice modelview
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Color buffer
	glClearColor(0.5, 0.5, 0.5, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);

	// Affichage de l'image originale :
	DisplayTexture(webcam->GetData(), id_texture_original, 0, 0, w, h);

	// Affichage, en rose transparent, du masque par-dessus :
	glEnable(GL_BLEND);
	glColor3f(1.0f, 0.0f, 1.0f);
	DisplayTexture(id_texture_masque, 0, 0, w, h);
	glColor3f(1.0f, 1.0f, 1.0f);
	glDisable(GL_BLEND);

	// Affichage des images filtrées, en haut, à la moitié de leur taille originale :
	DisplayTexture(&image_blanc, id_texture_blanc, 0, h, w / 2, h / 2);
	DisplayTexture(&image_noir, id_texture_noir, w / 2, h, w / 2, h / 2);

	// Affichage des rectangles correspondant aux taches :
	// - par dessus l'image originale :
	DisplayTaches(&image_noir, 0, h / 2, w, h, COULEUR_CARRE_NOIR);
	DisplayTaches(&image_blanc, 0, h / 2, w, h, COULEUR_CARRE_BLANC);

	// - par dessus les images binarisées :
	DisplayTaches(&image_noir, w / 2, 0, w / 2, h / 2, COULEUR_CARRE_NOIR);
	DisplayTaches(&image_blanc, 0, 0, w / 2, h / 2, COULEUR_CARRE_BLANC);

	// Affichage du carré correspondant à la position de la souris
	DisplayMouse();

	DisplayPositionsPointees();
}

// -------------------------------------------
GLuint CreateTexture(int w, int h, unsigned char* data) {
	GLuint id = 0;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	return id;
}

// -------------------------------------------
// Fonctions d'affichage
void DisplayTexture(GLuint id, int x, int y, int w, int h) {
	// NB : on retourne verticalement l'image
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

void DisplayTexture(unsigned char* data, GLuint id, int x, int y, int w, int h) {
	glBindTexture(GL_TEXTURE_2D, id);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, data);

	DisplayTexture(id, x, y, w, h);
}

void DisplayTexture(ImageFiltree* image_filtree, GLuint id, int x, int y, int w, int h) {
	glBindTexture(GL_TEXTURE_2D, id);
	glTexSubImage2D(GL_TEXTURE_2D,
	                0,
	                0,
	                0,
	                image_filtree->GetWidth(),
	                image_filtree->GetHeight(),
	                GL_RGB,
	                GL_UNSIGNED_BYTE,
	                image_filtree->GetDataRGB());

	DisplayTexture(id, x, y, w, h);
}

void DisplayTaches(ImageFiltree* image_filtree, int x, int y, int w, int h, GLfloat r, GLfloat g, GLfloat b) {
	const int width = webcam->GetWidth(), height = webcam->GetHeight();

	// Affichage des taches
	const list<ImageFiltree::Tache>& taches = image_filtree->GetListeTaches();
	for(list<ImageFiltree::Tache>::const_iterator it = taches.begin(); it != taches.end(); it++) {
		DisplayTache((*it).x * w / width + x,
		             (*it).y * h / height + y,
		             (*it).w * w / width,
		             (*it).h * h / height,
		             (*it).x_barycentre * w / width + x,
		             (*it).y_barycentre * h / height + y,
		             r,
		             g,
		             b);
	}
}

// Fonction d'affichage d'une tache
void DisplayTache(int x_tache, int y_tache, int w_tache, int h_tache, int x_barycentre, int y_barycentre, GLfloat r, GLfloat g, GLfloat b) {
	int win_width = 0, win_height = 0;
	glfwGetWindowSize(&win_width, &win_height);

	glDisable(GL_TEXTURE_2D);
	glColor3f(r, g, b);

	// Affichage du carré
	glBegin(GL_LINE_LOOP);
	glVertex2i(x_tache, win_height - (y_tache + h_tache));
	glVertex2i(x_tache + w_tache, win_height - (y_tache + h_tache));
	glVertex2i(x_tache + w_tache, win_height - y_tache);
	glVertex2i(x_tache, win_height - y_tache);
	glEnd();

	// Affichage du barycentre :
	glBegin(GL_LINES);
	glVertex2i(x_barycentre - TAILLE_CROIX, win_height - y_barycentre);
	glVertex2i(x_barycentre + TAILLE_CROIX, win_height - y_barycentre);
	glVertex2i(x_barycentre, win_height - (y_barycentre + TAILLE_CROIX));
	glVertex2i(x_barycentre, win_height - (y_barycentre - TAILLE_CROIX));
	glEnd();

	glColor3f(1.0f, 1.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
}


// -------------------------------------------
void GLFWCALL KeyCallback(int key, int state) {
	if(state == GLFW_PRESS)
		return;

	if(key == GLFW_KEY_ENTER) {
		if(calibrationModeTCPIP) {
			ostringstream oss117;
			calibrage.Ecrire(oss117);
			socketConfiguration->SendMsg(oss117.str().c_str(), oss117.str().size());
		} else {
			if(!calibrage.Ecrire(calibrationDevice))
				logError("impossible d'ecrire \"", calibrationDevice, "\"");
		}

		cout << "Enregistre en tant que \"" << calibrationDevice << "\"" << endl;
	} else if(key == 'L')
		InitCalibrage();
	else if(key == 'C') {
		if(phaseCalibrage)
			cout << "FIN phase calibrage" << endl;
		else {
			cout << "DEBUT phase calibrage" << endl
			     << "=====================" << endl
			     << "Cliquer sur les mais dans l'ordre defini dans Webcam2010.cpp" << endl
			     << "(Mais lateraux puis mais centraux)" << endl
			     << "Si un mais n'est pas visible, cliquer juste devant le robot sur la table" << endl
			     << "pour etre sur que le point ne se situera pas dans une tache lors de la detection" << endl
			     << "Appuyer sur Z permet d'annuler le dernier clic" << endl;
			maisCourant = 0;
		}
		phaseCalibrage = !phaseCalibrage;

	} else if(key == 'M') {
		cout << "Chargement d'un masque" << endl;
		LoadMask("");
	} else if(key == 'Z') {
		if(maisCourant && phaseCalibrage)
			maisCourant--;
	}
}

// Un clic gauche permet de selectionner la couleur moyenne pour le reperage des taches blanches
// Le clic droit fait la meme chose pour les taches noires
void GLFWCALL MouseCallback(int key, int state) {
	// return;
	int x, y, tx, ty, offset;
	long int r, g, b;
	r = g = b = 0;
	int count = 0;
	int cote = (TAILLE_CARRE_CURSEUR - 1) / 2;
	int win_width = 0, win_height = 0;
	glfwGetWindowSize(&win_width, &win_height);

	// Clic detecte => calcul de la couleur moyenne a l'emplacement du curseur, dans un carre de 5x5 pixels
	if(state == GLFW_PRESS) {
		glfwGetMousePos(&x, &y);
		y -= (win_height - webcam->GetHeight());
		cout << x << " " << y << endl;

		if(phaseCalibrage) {
			if(maisCourant < NB_MAIS_LATERAUX) {
				calibrage.mais_lateraux[maisCourant] = Vector2<int>(x, y);
			} else {
				calibrage.mais_centraux[maisCourant - NB_MAIS_LATERAUX] = Vector2<int>(x, y);
			}

			maisCourant++;
			if(maisCourant >= NB_MAIS_LATERAUX + NB_MAIS_CENTRAUX)
				phaseCalibrage = false;
		} // if(phaseCalibrage)
		else {
			unsigned char* data = webcam->GetData();
			for(int i = -cote; i <= cote; i++)
				for(int j = -cote; j <= cote; j++) {
					tx = x + i;
					ty = y + j;
					if(tx > 0 && tx < webcam->GetWidth() && ty > 0 && ty < webcam->GetHeight()) {
						offset = 3 * (tx + ty * webcam->GetWidth());
						r += data[offset + 0];
						g += data[offset + 1];
						b += data[offset + 2];
						count++;
					}
				}
			if(count) {
				cout << "count " << count << " r : " << r / count << " g : " << g / count << " b : " << b / count << endl;
				// clic droit => couleur moyenne pour les taches noires
				if(glfwGetMouseButton(GLFW_MOUSE_BUTTON_RIGHT)) {
					calibrage.rgb_noir_min.r = clamp(r / count - ecartCouleurNoir, 0, 255);
					calibrage.rgb_noir_min.g = clamp(g / count - ecartCouleurNoir, 0, 255);
					calibrage.rgb_noir_min.b = clamp(b / count - ecartCouleurNoir, 0, 255);

					calibrage.rgb_noir_max.r = clamp(r / count + ecartCouleurNoir, 0, 255);
					calibrage.rgb_noir_max.g = clamp(g / count + ecartCouleurNoir, 0, 255);
					calibrage.rgb_noir_max.b = clamp(b / count + ecartCouleurNoir, 0, 255);
				} // clic gauche => couleur moyenne pour les taches blanches
				else if(glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT)) {
					calibrage.rgb_blanc_min.r = clamp(r / count - ecartCouleurBlanc, 0, 255);
					calibrage.rgb_blanc_min.g = clamp(g / count - ecartCouleurBlanc, 0, 255);
					calibrage.rgb_blanc_min.b = clamp(b / count - ecartCouleurBlanc, 0, 255);

					calibrage.rgb_blanc_max.r = clamp(r / count + ecartCouleurBlanc, 0, 255);
					calibrage.rgb_blanc_max.g = clamp(g / count + ecartCouleurBlanc, 0, 255);
					calibrage.rgb_blanc_max.b = clamp(b / count + ecartCouleurBlanc, 0, 255);
				}
			}
		} // else
	}     // if(state == GLFW_PRESS)
}

void DisplayMouse() {
	int win_width = 0, win_height = 0;
	glfwGetWindowSize(&win_width, &win_height);

	if(phaseCalibrage)
		return;
	int cote = (TAILLE_CARRE_CURSEUR - 1) / 2;
	int x, y;
	// int h = webcam->GetHeight();
	glfwGetMousePos(&x, &y);

	glDisable(GL_TEXTURE_2D);
	glColor3f(1.0f, 0.0f, 0.5f);

	// Affichage du carré
	glBegin(GL_LINE_LOOP);
	glVertex2i(x - cote, win_height - (y + cote));
	glVertex2i(x + cote, win_height - (y + cote));
	glVertex2i(x + cote, win_height - (y - cote));
	glVertex2i(x - cote, win_height - (y - cote));
	glEnd();

	glColor3f(1.0f, 1.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
}

void DisplayPositionsPointees() {
	int win_width = 0, win_height = 0;
	glfwGetWindowSize(&win_width, &win_height);

	int x, y, cote = 3;


	if(maisCourant > NB_MAIS_LATERAUX + NB_MAIS_CENTRAUX) {
		maisCourant = 0;
		return;
	}

	glDisable(GL_TEXTURE_2D);
	glColor3f(0.0f, 0.0f, 1.0f);

	for(int i = 0; i < maisCourant; i++) {
		// Affichage du carré
		if(i < NB_MAIS_LATERAUX) {
			x = calibrage.mais_lateraux[i].x;
			y = calibrage.mais_lateraux[i].y;
		} else {
			x = calibrage.mais_centraux[i - NB_MAIS_LATERAUX].x;
			y = calibrage.mais_centraux[i - NB_MAIS_LATERAUX].y;
		}
		y += (win_height - webcam->GetHeight());

		glBegin(GL_QUADS);
		glVertex2i(x - cote, win_height - (y + cote));
		glVertex2i(x + cote, win_height - (y + cote));
		glVertex2i(x + cote, win_height - (y - cote));
		glVertex2i(x - cote, win_height - (y - cote));
		glEnd();
	}

	glColor3f(1.0f, 1.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
}

#endif
