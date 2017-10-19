// Application.cpp
// CONVENTION :
// Pour les images (ImageFiltree), le pixel (0, 0) est en HAUT à gauche
// Pour les coordonnées OpenGL (que l'on passe aux fonctions DisplayXxx()) : (0, 0) est en BAS à gauche.

#include "Application.h"
#include "GLUtils.h"
#include "Log.h"
#include "StaticAssert.h"
#include <stdlib.h>
using namespace std;

#define WIN_TITLE "Calibrage 2010"

#define COULEURS_CARRE_NOIR Color(0x00, 0x00, 0x00), Color(0xFF, 0x00, 0xFF)    // pour les taches noires
#define COULEURS_CARRE_BLANC Color(0xFF, 0xFF, 0xFF), Color(0xFF, 0x00, 0xFF)   // pour les taches blanches
#define COULEURS_CARRE_CURSEUR Color(0xFF, 0x00, 0x00), Color(0xFF, 0xFF, 0x00) // pour le curseur
#define COULEUR_POSITIONS_CLIQUEES Color(0xFF, 0x00, 0xFF)                      // pour les positions cliquées

#define COULEUR_LIGNE_SEPARATRICE Color(0xFF, 0x00, 0xFF)
#define TAILLE_CARRE_CURSEUR 15

template <class T>
T clamp(T value, T valMin, T valMax) {
	if(value < valMin)
		value = valMin;
	if(value > valMax)
		value = valMax;
	return value;
}

// -------------------------------------------
Application::Application()
        : webcam(NULL)
        , GTKwin()
        , idNoir(0)
        , idBlanc(0)
        , idImageMasquee(0)
        , imageMasqueeData(NULL)
        , camWidth(0)
        , camHeight(0)
        , phaseCalibrage(false)
        , maisCourant(0)
        , afficherTaches(true)
        , afficherMasque(true)
        , afficherPositionsCliquees(true) {
	// Ne rien faire d'autre qu'initialiser des variables ici.
	// Utiliser Init() pour le reste, on ne sait pas quand le constructeur est appelé
	// (singleton).
}

// -------------------------------------------
// Singleton getter :
Application* Application::GetInstance() {
	static Application instance;
	return &instance;
}

// -------------------------------------------
// Point d'entrée de l'application :
int Application::Run(int argc, char* argv[]) {
	AfficherAide();

	if(!Init(argc, argv)) {
		Cleanup();
		return EXIT_FAILURE;
	}

	if(!MainLoop()) {
		Cleanup();
		return EXIT_FAILURE;
	}

	Cleanup();
	return EXIT_SUCCESS;
}

// -------------------------------------------
void Application::AfficherAide() {
	cout << "Calibrage 2010 - commandes" << endl;
	cout << "F1 : afficher cette aide" << endl;
	cout << "Ctrl+P : afficher l'etat du calibrage dans la console" << endl;
	cout << "Ctrl+O : ouvrir un fichier de configuration" << endl;
	cout << "Ctrl+S : enregistrer un fichier de configuration" << endl;
	cout << "Ctrl+M : charger un masque" << endl;
	cout << "Ctrl+C : debut phase de calibrage" << endl;
	cout << "Ctrl+Z : annuler le clic sur un mais" << endl;
	cout << "Ctrl+F : flip horizontal de l'image" << endl;
	cout << "Clic gauche : mais noirs" << endl;
	cout << "Clic droit  : mais blancs" << endl;
	cout << "--------------------------" << endl;
}

// -------------------------------------------
bool Application::Init(int argc, char* argv[]) {
	// Initialisation de GLFW et du système de logging :
	glfwInit();
	Log::open(argc, argv);

	// Initialisation de la webcam, des paramètres de calibrage, etc.
	if(!InitWebcam(argc, argv))
		return false;

	// Récupération des dimensions de la webcam
	this->camWidth = webcam->GetWebcam()->GetWidth();
	this->camHeight = webcam->GetWebcam()->GetHeight();

	// Création de la fenetre GLFW, des textures, etc
	if(!InitGL())
		return false;

	// Initialisation de la fenetre GTK :
	if(!InitGTK())
		return false;

	return true;
}

// Callback wrappers :
static void GLFWCALL KeyCallbackWrapper(int key, int state) {
	Application::GetInstance()->KeyCallback(key, state);
}

static void GLFWCALL MouseCallbackWrapper(int key, int state) {
	Application::GetInstance()->MouseCallback(key, state);
}

// Initialisation de la webcam, des paramètres de calibrage, etc.
bool Application::InitWebcam(int argc, char* argv[]) {
	this->webcam = new Webcam2010();

	if(!this->webcam->Init(argc, argv, false))
		return false;

	// Copie des paramètres de calibrage lus par la Webcam2010.
	this->calibrage = *(this->webcam->GetCalibrage());
	return true;
}

// Initialisation de GLFW, des paramètres OpenGL, etc
bool Application::InitGL() {
	// Création de la fenetre GLFW :
	if(!glfwOpenWindow(camWidth, camHeight + camHeight / 2, 0, 0, 0, 0, 0, 0, GLFW_WINDOW)) {
		logError("impossible d'ouvrir une fenetre");
		return false;
	}

	glfwSwapInterval(0); // disable V-Sync
	glfwSetKeyCallback(KeyCallbackWrapper);
	glfwSetMouseButtonCallback(MouseCallbackWrapper);

	// Création des textures :
	idOriginal = CreateTextureRGB(camWidth, camHeight);
	idNoir = CreateTextureRGB(camWidth, camHeight);
	idBlanc = CreateTextureRGB(camWidth, camHeight);
	idImageMasquee = CreateTextureRGB(camWidth, camHeight);

	// Création du buffer pour l'image masquée :
	imageMasqueeData = new unsigned char[camWidth * camHeight * 3];

	// Etat que l'on considère "par défaut" d'OpenGL :
	glColor3ub(0xFF, 0xFF, 0xFF);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);

	return true;
}

// Initialisation de la fenetre GTK :
bool Application::InitGTK() {
	GTKwin.Init();

	GTKwin.Attach(calibrage.ecartCouleurNoir, "Ecart couleur noir", 0, 255);
	GTKwin.Attach(calibrage.ecartCouleurBlanc, "Ecart couleur blanc", 0, 255);

	GTKwin.Attach(calibrage.widthMin, "Largeur min", 1, camWidth);
	GTKwin.Attach(calibrage.heightMin, "Hauteur min", 1, camHeight);

	GTKwin.Attach(calibrage.widthMax, "Largeur max", 1, camWidth);
	GTKwin.Attach(calibrage.heightMax, "Hauteur max", 1, camHeight);

	GTKwin.Attach(calibrage.couleurNoir.r, "Noir R", 0x00, 0xFF);
	GTKwin.Attach(calibrage.couleurNoir.g, "Noir G", 0x00, 0xFF);
	GTKwin.Attach(calibrage.couleurNoir.b, "Noir B", 0x00, 0xFF);

	GTKwin.Attach(calibrage.couleurBlanc.r, "Blanc R", 0x00, 0xFF);
	GTKwin.Attach(calibrage.couleurBlanc.g, "Blanc G", 0x00, 0xFF);
	GTKwin.Attach(calibrage.couleurBlanc.b, "Blanc B", 0x00, 0xFF);

	GTKwin.Attach(afficherMasque, "Afficher masque");
	GTKwin.Attach(afficherTaches, "Afficher taches");
	GTKwin.Attach(afficherPositionsCliquees, "Afficher positions");

	GTKwin.Run();
	return true;
}

// -------------------------------------------
void Application::Cleanup() {
	// Image masquée :
	if(imageMasqueeData != NULL) {
		delete[] imageMasqueeData;
		imageMasqueeData = NULL;
	}

	// Fenetre GTK :
	GTKwin.Terminate();

	// Suppression des textures :
	DeleteTexture(&idOriginal);
	DeleteTexture(&idNoir);
	DeleteTexture(&idBlanc);
	DeleteTexture(&idImageMasquee);

	// Suppression de la webcam :
	delete webcam;
	webcam = NULL;

	// Système de logging, GLFW
	Log::close();
	glfwTerminate();
}

// -------------------------------------------
// Boucle principale :
bool Application::MainLoop() {
	bool running = true;

	while(running) {
		// Mise à jour du titre de la fenetre :
		CountFPS();

		// Récupération d'une image, binarisation, superfloutage, repérage des taches.
		if(!webcam->AcquisitionEtTraitement())
			return false;

		Update();

		Display();

		glfwSwapBuffers();

		// Test d'erreurs OpenGL
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
			logError("Erreur OpenGL : ", gluErrorString(error));

		// Condition de sortie
		running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
	}

	return true;
}

void Application::CountFPS() {
	static bool firstTime = true;
	static int nbFrames = 0;
	static double t0 = 0.0;

	if(firstTime) {
		t0 = glfwGetTime();
		firstTime = false;
	}

	nbFrames++;

	if(glfwGetTime() - t0 >= 1.0) {
		char s[255] = "";
		t0 = glfwGetTime();

		sprintf(s, "FPS : %d - %s", nbFrames, WIN_TITLE);
		glfwSetWindowTitle(s);

		nbFrames = 0;
	}
}

// -------------------------------------------
void Application::KeyCallback(int key, int state) {
	if(state == GLFW_PRESS)
		return;

	if(key == GLFW_KEY_F1) {
		AfficherAide();
		return;
	}

	bool ctrlPressed = (glfwGetKey(GLFW_KEY_LCTRL) == GLFW_PRESS) || (glfwGetKey(GLFW_KEY_RCTRL) == GLFW_PRESS);

	if(ctrlPressed) {
		// Ctrl+P : affichage de l'état du calibrage dans la console
		if(key == 'P')
			calibrage.Print();
		else if(key == 'S') {
			string filename;
			cout << "[SAVE] : nom du fichier de configuration [" << webcam->getConfigFilename() << "] : " << flush;
			getline(cin, filename);

			if(filename == "")
				filename = webcam->getConfigFilename();

			if(!calibrage.Ecrire(filename.c_str()))
				cout << "[SAVE] : erreur : impossible d'enregistrer \"" << filename << "\"" << endl;
			else
				cout << "[SAVE] : configuration enregistree dans \"" << filename << "\"" << endl;
		}
		// Ctrl+O : chargement du fichier de configuration
		else if(key == 'O') {
			string filename;
			cout << "[OPEN] : nom du fichier de configuration [" << webcam->getConfigFilename() << "] : " << flush;
			getline(cin, filename);

			if(filename == "")
				filename = webcam->getConfigFilename();

			if(!calibrage.Lire(filename.c_str(), camWidth, camHeight))
				cout << "[OPEN] : impossible de lire le fichier de configuration \"" << filename << endl;
			else
				cout << "[OPEN] : configuration \"" << filename << "\" chargee" << endl;
		}
		// Ctrl+M : chargement d'un masque
		else if(key == 'M') {
			string filename;
			cout << "[OPEN MASK] : nom du fichier TGA : " << flush;
			getline(cin, filename);

			if(filename == "")
				cout << "[OPEN MASK] : annule" << endl;
			else {
				// NB : le flipping est fait par LoadMask() si besoin.
				if(!calibrage.LoadMask(filename, camWidth, camHeight))
					cout << "[OPEN MASK] : erreur : impossible de charger le masque \"" << filename << "\"" << endl;
				else
					cout << "[OPEN MASK] : masque \"" << filename << "\" charge" << endl;
			}
		}
		// Ctrl+C : passer en mode calibrage
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
				     << "Ctrl+Z permet d'annuler le dernier clic" << endl;
				maisCourant = 0;
			}
			phaseCalibrage = !phaseCalibrage;
		}
		// Ctrl+Z
		else if(key == 'Z') {
			if(maisCourant && phaseCalibrage)
				maisCourant--;
		}
		// Ctrl+F : flipping horizontal
		else if(key == 'F') {
			calibrage.FlipHorizontal();

			cout << "[FLIP HORIZONTAL] : IsFlipHorizontal() == " << (calibrage.IsFlipHorizontal() ? "true" : "false") << endl;
		}
	}
}

// -------------------------------------------
void Application::MouseCallback(int key, int state) {
	int x, y, tx, ty, offset;
	int r, g, b;
	r = g = b = 0;
	int count = 0;
	int cote = (TAILLE_CARRE_CURSEUR - 1) / 2;

	// Recuperation de la taille de la fenetre
	int winWidth = 0, winHeight = 0;
	glfwGetWindowSize(&winWidth, &winHeight);

	// Clic detecte
	if(state == GLFW_PRESS) {
		glfwGetMousePos(&x, &y);
		y -= (winHeight - camHeight);
		cout << "Position clic : (" << x << ", " << y << ")" << endl;

		// Phase calibrage : on note les positions des clics pour les mais, d'abord latéraux, ensuite centraux.
		if(phaseCalibrage) {
			Vector2<int> centreTache = PlusProcheTache(x, y);

			if(maisCourant < NB_MAIS_LATERAUX) {
				calibrage.maisLateraux[maisCourant] = centreTache;
				cout << "Mais lateral " << maisCourant << " <- (" << centreTache.x << ", " << centreTache.y << ")" << endl;
			} else {
				int numMaisCentral = maisCourant - NB_MAIS_LATERAUX;
				calibrage.maisCentraux[numMaisCentral] = centreTache;
				cout << "Mais central " << numMaisCentral << " <- (" << centreTache.x << ", " << centreTache.y << ")" << endl;
			}

			maisCourant++;
			if(maisCourant >= NB_MAIS_TOTAL) {
				cout << "Fin de la phase de calibrage" << endl;
				cout << "----------------------------" << endl;
				phaseCalibrage = false;
			}
		}
		// Mode normal : calcul de la couleur moyenne a l'emplacement du curseur, dans un carre de 5x5 pixels
		// TODO : mettre ça dans une fonction à part, dédiée à ce calcul.
		else {
			const unsigned char* data = webcam->GetWebcam()->GetData();
			const unsigned char* maskData = calibrage.GetMaskDataRGB();
			for(int i = -cote; i <= cote; i++) {
				for(int j = -cote; j <= cote; j++) {
					tx = x + i;
					ty = y + j;

					offset = 3 * (tx + ty * camWidth);

					if(tx >= 0 && tx < camWidth && ty >= 0 && ty < camHeight && (maskData == NULL || maskData[offset + 0] != 0x00)) {
						r += data[offset + 0];
						g += data[offset + 1];
						b += data[offset + 2];
						count++;
					}
				}
			}

			if(count != 0) {
				r = clamp(r / count, 0, 255);
				g = clamp(g / count, 0, 255);
				b = clamp(b / count, 0, 255);

				cout << "Couleur moyenne : (" << r << ", " << g << ", " << b << "), " << count << " pixels utilises" << endl;

				// clic droit => couleur moyenne pour les taches blanches
				if(glfwGetMouseButton(GLFW_MOUSE_BUTTON_RIGHT)) {
					calibrage.couleurBlanc.r = r;
					calibrage.couleurBlanc.g = g;
					calibrage.couleurBlanc.b = b;
				}
				// clic gauche => couleur moyenne pour les taches noires
				else if(glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT)) {
					calibrage.couleurNoir.r = r;
					calibrage.couleurNoir.g = g;
					calibrage.couleurNoir.b = b;
				}
			}
		}
	}
}

// -------------------------------------------
void Application::Update() {
	// Mise à jour des paramètres de calibrage de la webcam
	webcam->SetCalibrage(calibrage);

	// Mise à jour de l'image masquée :
	UpdateImageMasquee();
}

// -------------------------------------------
void Application::UpdateImageMasquee() {
	if(!calibrage.HasMask())
		return;

	int size = camWidth * camHeight;
	const unsigned char* maskData = calibrage.GetMaskDataRGB();
	const unsigned char* webcamData = webcam->GetWebcam()->GetData();

	for(int i = 0; i < size; i++) {
		// Si le pixel du masque est noir, on met la couleur du masque, sinon on met l'image originale.
		if(maskData[i * 3] == 0x00) {
			imageMasqueeData[i * 3 + 0] = maskData[i * 3 + 0];
			imageMasqueeData[i * 3 + 1] = maskData[i * 3 + 1];
			imageMasqueeData[i * 3 + 2] = maskData[i * 3 + 2];
		} else {
			imageMasqueeData[i * 3 + 0] = webcamData[i * 3 + 0];
			imageMasqueeData[i * 3 + 1] = webcamData[i * 3 + 1];
			imageMasqueeData[i * 3 + 2] = webcamData[i * 3 + 2];
		}
	}
}

// -------------------------------------------
void Application::Display() {
	Setup2DDisplay(Color(0x7F, 0x7F, 0x7F));

	// Affichage de l'image masquée :
	if(calibrage.HasMask() && afficherMasque) {
		DisplayData(imageMasqueeData, camWidth, camHeight, idImageMasquee, 0, 0, camWidth, camHeight);
	}
	// Affichage de l'image originale :
	else {
		// Affichage de l'image originale en bas :
		DisplayData(webcam->GetWebcam()->GetData(), camWidth, camHeight, idOriginal, 0, 0, camWidth, camHeight);
	}

	// Affichage des images filtrées en haut, en divisant par 2 leur taille :
	// - à gauche : noir :
	DisplayImage(webcam->GetImageNoir(), idNoir, 0, camHeight, camWidth / 2, camHeight / 2);
	// - à droite : blanc :
	DisplayImage(webcam->GetImageBlanc(), idBlanc, camWidth / 2, camHeight, camWidth / 2, camHeight / 2);

	// Affichage des taches :
	if(afficherTaches) {
		// Affichage des taches par dessus l'image originale :
		DisplayTaches(webcam->GetImageNoir(), 0, 0, camWidth, camHeight, COULEURS_CARRE_NOIR);
		DisplayTaches(webcam->GetImageBlanc(), 0, 0, camWidth, camHeight, COULEURS_CARRE_BLANC);

		// Affichage des taches sur les images filtrées, en haut :
		// - à gauche : noir :
		DisplayTaches(webcam->GetImageNoir(), 0, camHeight, camWidth / 2, camHeight / 2, COULEURS_CARRE_NOIR);

		// - à droite : blanc :
		DisplayTaches(webcam->GetImageBlanc(), camWidth / 2, camHeight, camWidth / 2, camHeight / 2, COULEURS_CARRE_BLANC);
	}

	// Affichage des positions cliquees avec la souris
	if(afficherPositionsCliquees) {
		if(phaseCalibrage)
			DisplayPositionsCliquees(calibrage, maisCourant, camHeight, COULEUR_POSITIONS_CLIQUEES);
		else
			DisplayPositionsCliquees(calibrage, NB_MAIS_TOTAL, camHeight, COULEUR_POSITIONS_CLIQUEES);
	}

	// Affichage de la position de la souris :
	DisplayMousePos(COULEURS_CARRE_CURSEUR, TAILLE_CARRE_CURSEUR);

	// Lignes separatrices
	// - verticale
	DisplayRect(camWidth / 2, camHeight, 1, camHeight / 2, COULEUR_LIGNE_SEPARATRICE);
	// - horizontale
	DisplayRect(0, camHeight, camWidth, 1, COULEUR_LIGNE_SEPARATRICE);
}

// -------------------------------------------
Vector2<int> Application::PlusProcheTache(int x, int y) {
	float plusPetiteDistance = -1.0, distance;

	// Parcours des taches noires
	const list<ImageFiltree::Tache>& tachesNoir = webcam->GetImageNoir()->GetListeTaches();
	list<ImageFiltree::Tache>::const_iterator itEndNoir = tachesNoir.end(), itBest = tachesNoir.end();

	for(list<ImageFiltree::Tache>::const_iterator it = tachesNoir.begin(); it != itEndNoir; it++) {
		const ImageFiltree::Tache& tache = (*it);
		if((x >= tache.x) && (x <= tache.x + tache.w) && (y >= tache.y) && (y <= tache.y + tache.h)) {
			distance = Vector2<int>((*it).x_barycentre - x, (*it).y_barycentre - y).norm();
			if(distance < plusPetiteDistance || plusPetiteDistance < 0.0) {
				plusPetiteDistance = distance;
				itBest = it;
			}
		}
	}

	// Parcours des taches blanches
	const list<ImageFiltree::Tache>& tachesBlanc = webcam->GetImageBlanc()->GetListeTaches();
	list<ImageFiltree::Tache>::const_iterator itEndBlanc = tachesBlanc.end();

	for(list<ImageFiltree::Tache>::const_iterator it = tachesBlanc.begin(); it != itEndBlanc; it++) {
		const ImageFiltree::Tache& tache = (*it);
		if((x >= tache.x) && (x <= tache.x + tache.w) && (y >= tache.y) && (y <= tache.y + tache.h)) {
			distance = Vector2<int>((*it).x_barycentre - x, (*it).y_barycentre - y).norm();
			if(distance < plusPetiteDistance || plusPetiteDistance < 0.0) {
				plusPetiteDistance = distance;
				itBest = it;
			}
		}
	}

	// Si aucune tache n'a ete trouvee, on retourne les coordonnees du point clique
	if(itBest == itEndNoir)
		return Vector2<int>(x, y);

	// Sinon on retourne les coordonnees du barycentre de la tache la plus proche
	return Vector2<int>((*itBest).x_barycentre, (*itBest).y_barycentre);
}
