// CalibrageWebcam.cpp

#include "CalibrageWebcam.h"
#include "Socket.h"
#include "log/Log.h"
#include <fstream>
using namespace std;

#define TAILLE_BUFFER_FICHIER 102400

template <class T>
T clamp(T val, T valMin, T valMax) {
	if(val < valMin)
		return valMin;
	if(val > valMax)
		return valMax;
	return val;
}

CalibrageWebcam::CalibrageWebcam() {
	// Valeurs par défaut :
	couleurNoir = COLOR_BLACK;
	couleurBlanc = COLOR_WHITE;

	ecartCouleurNoir = ecartCouleurBlanc = 20;

	widthMin = heightMin = 1;
	widthMax = heightMax = 1000;

	flipHorizontal = false;

	maskFilename = "";
	maskWidth = maskHeight = 0;
	maskDataRGB = NULL;
}

CalibrageWebcam::CalibrageWebcam(const CalibrageWebcam& ref) {
	*this = ref;
}

CalibrageWebcam::~CalibrageWebcam() {
	delete[] maskDataRGB;
}

bool CalibrageWebcam::Ecrire(const char* filename) const {
	bool modeTCPIP = false;

	// teste si c'est une adresse IP et port TCP
	int port, ipServeur[4];
	int nbrEntierParse = sscanf(filename, "%d.%d.%d.%d:%d", &ipServeur[0], &ipServeur[1], &ipServeur[2], &ipServeur[3], &port);
	if(nbrEntierParse == 5) {
		modeTCPIP = true;
	} else {
		// teste si c'est un port TCP
		ipServeur[0] = 10;
		ipServeur[1] = 30;
		ipServeur[2] = 5;
		ipServeur[3] = 42;
		nbrEntierParse = sscanf(filename, "%d", &port);
		if(nbrEntierParse == 1) {
			modeTCPIP = true;
		}
	}

	// ouvre le flux suivant le mode
	ostream* os;
	if(modeTCPIP) {
		os = new ostringstream();
	} else {
		os = new ofstream(filename);
		if(!((ofstream*)os)->is_open()) {
			logError("impossible d'ouvrir le fichier \"", filename, "\" en ecriture !");
			return false;
		}
	}

	cout << "Ecriture dans \"" << filename << "\" :" << endl;
	Print();

	// On écrit dans le flux créé auparavant :
	Ecrire(*os);

	// Selon que l'on utilise TCP/IP ou pas, soit on envoie par réseau,
	// soit on écrit dans un fichier en local.
	bool retour = true;

	if(modeTCPIP) {
		ostringstream* oss = (ostringstream*)os;
		char ipStr[80];
		sprintf(ipStr, "%d.%d.%d.%d", ipServeur[0], ipServeur[1], ipServeur[2], ipServeur[3]);
		logInfo("connexion au serveur de configuration ", ipStr, ":", port);

		Socket::Init();
		Socket socketConfiguration;
		socketConfiguration.Create(SockProtocol::TCP);
		if(!socketConfiguration.Connect(ipStr, port)) {
			logError("connexion au serveur de configuration");
			retour = false;
		} else {
			logInfo("connexion au serveur de configuration OK");
			socketConfiguration.SendMsg("SEND", strlen("SEND") + 1);
			socketConfiguration.SendMsg(oss->str().c_str(), oss->str().size() + 1);
			retour = true;
		}

		socketConfiguration.Destroy();
		Socket::CleanUp();
	} else {
		((ofstream*)os)->close();
		retour = true;
	}

	delete os;

	return retour;
}

// ---------------------------------------------------------------------
// Ecriture dans un fichier :
void CalibrageWebcam::Ecrire(ostream& os) const {
	os << int(couleurNoir.r) << " " << int(couleurNoir.g) << " " << int(couleurNoir.b) << endl;

	os << int(couleurBlanc.r) << " " << int(couleurBlanc.g) << " " << int(couleurBlanc.b) << endl;

	os << ecartCouleurNoir << endl;
	os << ecartCouleurBlanc << endl;

	os << widthMin << " " << heightMin << endl;

	os << widthMax << " " << heightMax << endl;

	for(int i = 0; i < NB_MAIS_LATERAUX; i++)
		os << maisLateraux[i].x << " " << maisLateraux[i].y << endl;

	for(int i = 0; i < NB_MAIS_CENTRAUX; i++)
		os << maisCentraux[i].x << " " << maisCentraux[i].y << endl;

	os << int(flipHorizontal) << endl;

	os << "mask:" << maskFilename << endl; // On rajoute "mask:" pour éviter les pb si maskFilename==""
}

bool CalibrageWebcam::Lire(const char* filename, int camWidth, int camHeight) {
	bool modeTCPIP = false;

	// teste si c'est une adresse IP et port TCP
	int port, ipServeur[4];
	int nbrEntierParse = sscanf(filename, "%d.%d.%d.%d:%d", &ipServeur[0], &ipServeur[1], &ipServeur[2], &ipServeur[3], &port);
	if(nbrEntierParse == 5) {
		modeTCPIP = true;
	} else {
		// teste si c'est un port TCP
		ipServeur[0] = 10;
		ipServeur[1] = 30;
		ipServeur[2] = 5;
		ipServeur[3] = 42;
		nbrEntierParse = sscanf(filename, "%d", &port);
		if(nbrEntierParse == 1) {
			modeTCPIP = true;
		}
	}

	bool retour;

	// ouvre le flux suivant le mode
	if(modeTCPIP) {
		char bufferFichier[TAILLE_BUFFER_FICHIER];

		// connection au serveur de conf
		char ipStr[80];
		sprintf(ipStr, "%d.%d.%d.%d", ipServeur[0], ipServeur[1], ipServeur[2], ipServeur[3]);
		printf("connexion au serveur de configuration %s, port %d\n", ipStr, port);
		Socket::Init();
		Socket socketConfiguration;
		socketConfiguration.Create(SockProtocol::TCP);
		if(!socketConfiguration.Connect(ipStr, port)) {
			cerr << "erreur de connection au server de configuration" << endl;
			exit(-1);
		}
		cout << "connexion au serveur de configuration ok" << endl;

		// reception du fichier de conf
		socketConfiguration.SendMsg("GET", strlen("GET") + 1);
		cout << "attente reception fichier de conf ..." << endl;
		int tailleFichierConf = socketConfiguration.ReceiveMsg(bufferFichier, TAILLE_BUFFER_FICHIER);
		cout << "reception du fichier de conf de " << tailleFichierConf << " octets" << endl;
		istringstream is(bufferFichier);

		// lire le flux
		cout << "lecture du flux" << endl;
		retour = Lire(is, camWidth, camHeight);
		cout << "fin lecture du flux" << endl;

		// reception du masque
		socketConfiguration.SendMsg("GETMASK", strlen("GETMASK") + 1);
		socketConfiguration.SendMsg(maskFilename.c_str(), maskFilename.size() + 1);
		cout << "attente reception du masque ..." << endl;
		int tailleFichierMask = socketConfiguration.ReceiveMsg(bufferFichier, TAILLE_BUFFER_FICHIER);
		cout << "reception du fichier de conf de " << tailleFichierMask << " octets" << endl;

		if(!LoadMaskFromData((unsigned char*)bufferFichier, camWidth, camHeight))
			logWarn("impossible de charger le masque \"", maskFilename, "\"");

		// fermeture de la connection
		socketConfiguration.Destroy();
	} else {
		ifstream is(filename);
		if(!is.is_open()) {
			logError("impossible d'ouvrir le fichier \"", filename, "\" en lecture !");
			return false;
		}

		// lire le flux
		cout << "lecture du flux" << endl;
		retour = Lire(is, camWidth, camHeight);
		cout << "fin lecture du flux" << endl;

		is.close();

		if(!LoadMask(maskFilename, camWidth, camHeight))
			logWarn("impossible de charger le masque \"", maskFilename, "\"");
	}

	cout << "Lecture depuis \"" << filename << "\" :" << endl;
	Print();

	return retour;
}

// ---------------------------------------------------------------------
// Lecture depuis un fichier :
bool CalibrageWebcam::Lire(istream& is, int camWidth, int camHeight) {
	int i = 0;

	is >> i;
	couleurNoir.r = (unsigned char)i;
	is >> i;
	couleurNoir.g = (unsigned char)i;
	is >> i;
	couleurNoir.b = (unsigned char)i;

	is >> i;
	couleurBlanc.r = (unsigned char)i;
	is >> i;
	couleurBlanc.g = (unsigned char)i;
	is >> i;
	couleurBlanc.b = (unsigned char)i;

	is >> ecartCouleurNoir;
	is >> ecartCouleurBlanc;

	is >> widthMin;
	is >> heightMin;

	is >> widthMax;
	is >> heightMax;

	for(int i = 0; i < NB_MAIS_LATERAUX; i++) {
		is >> maisLateraux[i].x;
		is >> maisLateraux[i].y;
	}

	for(int i = 0; i < NB_MAIS_CENTRAUX; i++) {
		is >> maisCentraux[i].x;
		is >> maisCentraux[i].y;
	}

	is >> i;
	flipHorizontal = bool(i);

	is >> maskFilename;
	maskFilename = maskFilename.substr(5);

	return true;
}

// Chargement du masque :
bool CalibrageWebcam::LoadMask(const string& maskFilename, int camWidth, int camHeight) {
	this->maskFilename = maskFilename;

	// Chargement du fichier.
	TGALoader tga;
	if(tga.loadFile(maskFilename.c_str()) != TGA_OK)
		return false;

	return LoadMask(tga, camWidth, camHeight);
}

// Chargement du masque :
bool CalibrageWebcam::LoadMaskFromData(unsigned char* data, int camWidth, int camHeight) {
	this->maskFilename = maskFilename;

	// Chargement du fichier.
	TGALoader tga;
	if(tga.loadFromData(data) != TGA_OK)
		return false;

	return LoadMask(tga, camWidth, camHeight);
}

// Chargement du masque :
bool CalibrageWebcam::LoadMask(TGALoader& tga, int camWidth, int camHeight) {
	// Suppression du masque précédent, s'il y en a un :
	delete[] maskDataRGB;
	maskDataRGB = NULL;
	maskWidth = maskHeight = 0;

	// Tests de conformité :
	// - taille du masque :
	if(int(tga.getWidth()) != camWidth || int(tga.getHeight()) != camHeight) {
		logError("dimensions du masque differentes de la resolution webcam ((", tga.getWidth(), "x", tga.getHeight(), ") vs (", camWidth, "x", camHeight, "))");
		return false;
	}

	// - octets par pixel :
	if(tga.getBpp() != 3) {
		logError("nombre de BPP different de 3, attention a bien supprimer le canal alpha lors de l'enregistrement du "
		         "masque");
		return false;
	}

	// Copie du masque.
	// On retourne verticalement le masque car la convention suivie par TGALoader est l'inverse
	// de celle suivie par la libWebcam, i.e. :
	// TGALoader : origine en bas à gauche
	// libWebcam : origine en haut à gauche
	maskWidth = tga.getWidth();
	maskHeight = tga.getHeight();

	maskDataRGB = new unsigned char[maskWidth * maskHeight * 3];

	const unsigned char* tgaData = tga.getData();
	for(int x = 0; x < maskWidth; x++) {
		for(int y = 0; y < maskHeight; y++) {
			int cy = maskHeight - 1 - y;
			int i = (x + y * maskWidth) * 3;
			int ci = (x + cy * maskWidth) * 3;

			maskDataRGB[i + 0] = tgaData[ci + 0];
			maskDataRGB[i + 1] = tgaData[ci + 1];
			maskDataRGB[i + 2] = tgaData[ci + 2];
		}
	}
	return true;
}

// ---------------------------------------------------------------------
// Affichage dans la console :
void CalibrageWebcam::Print() const {
	cout << "---------------------" << endl;
	cout << "Calibrage :" << endl;
	cout << "ecart noir : " << ecartCouleurNoir << endl;
	cout << "ecart blanc : " << ecartCouleurBlanc << endl;
	cout << "taille min : " << widthMin << "x" << heightMin << endl;
	cout << "taille max : " << widthMax << "x" << heightMax << endl;

	cout << "positions des mais lateraux : " << endl;
	for(int i = 0; i < NB_MAIS_LATERAUX; i++)
		cout << maisLateraux[i] << endl;

	cout << "positions des mais centraux : " << endl;
	for(int i = 0; i < NB_MAIS_CENTRAUX; i++)
		cout << maisCentraux[i] << endl;

	cout << "flip horizontal : " << (flipHorizontal ? "true" : "false") << endl;
	cout << "masque : nom du fichier : \"" << maskFilename << "\"" << endl;
	cout << "masque : dimensions : " << maskWidth << "x" << maskHeight << endl;
	cout << "masque : charge : " << (maskDataRGB == NULL ? "non" : "oui") << endl;

	cout << "---------------------" << endl;
}

// ---------------------------------------------------------------------
void CalibrageWebcam::GetCouleurNoirMinMax(Color* noirMin, Color* noirMax) const {
	int semiEcart = ecartCouleurNoir / 2;
	noirMin->r = (unsigned char)clamp(int(couleurNoir.r) - semiEcart, 0, 255);
	noirMin->g = (unsigned char)clamp(int(couleurNoir.g) - semiEcart, 0, 255);
	noirMin->b = (unsigned char)clamp(int(couleurNoir.b) - semiEcart, 0, 255);

	noirMax->r = (unsigned char)clamp(int(couleurNoir.r) + semiEcart, 0, 255);
	noirMax->g = (unsigned char)clamp(int(couleurNoir.g) + semiEcart, 0, 255);
	noirMax->b = (unsigned char)clamp(int(couleurNoir.b) + semiEcart, 0, 255);
}

void CalibrageWebcam::GetCouleurBlancMinMax(Color* blancMin, Color* blancMax) const {
	int semiEcart = ecartCouleurBlanc / 2;
	blancMin->r = (unsigned char)clamp(int(couleurBlanc.r) - semiEcart, 0, 255);
	blancMin->g = (unsigned char)clamp(int(couleurBlanc.g) - semiEcart, 0, 255);
	blancMin->b = (unsigned char)clamp(int(couleurBlanc.b) - semiEcart, 0, 255);

	blancMax->r = (unsigned char)clamp(int(couleurBlanc.r) + semiEcart, 0, 255);
	blancMax->g = (unsigned char)clamp(int(couleurBlanc.g) + semiEcart, 0, 255);
	blancMax->b = (unsigned char)clamp(int(couleurBlanc.b) + semiEcart, 0, 255);
}

CalibrageWebcam& CalibrageWebcam::operator=(const CalibrageWebcam& ref) {
	this->couleurNoir = ref.couleurNoir;
	this->couleurBlanc = ref.couleurBlanc;

	this->ecartCouleurNoir = ref.ecartCouleurNoir;
	this->ecartCouleurBlanc = ref.ecartCouleurBlanc;

	this->widthMin = ref.widthMin;
	this->heightMin = ref.heightMin;

	this->widthMax = ref.widthMax;
	this->heightMax = ref.heightMax;

	for(int i = 0; i < NB_MAIS_LATERAUX; i++)
		this->maisLateraux[i] = ref.maisLateraux[i];

	for(int i = 0; i < NB_MAIS_CENTRAUX; i++)
		this->maisCentraux[i] = ref.maisCentraux[i];

	this->flipHorizontal = ref.flipHorizontal;

	// Masque :
	this->maskFilename = ref.maskFilename;

	if(ref.maskDataRGB == NULL) {
		this->maskDataRGB = NULL;
		this->maskWidth = this->maskHeight = 0;
	} else {
		int size = ref.maskWidth * ref.maskHeight * 3;

		if(this->maskWidth != ref.maskWidth || this->maskHeight != ref.maskHeight) {
			delete[] this->maskDataRGB;
			this->maskDataRGB = new unsigned char[size];
		}

		this->maskWidth = ref.maskWidth;
		this->maskHeight = ref.maskHeight;

		for(int i = 0; i < size; i++)
			this->maskDataRGB[i] = ref.maskDataRGB[i];
	}

	return *this;
}

// ---------------------------------------------------------------------
// Flipping : on n'oublie pas de mettre à jour le masque.
void CalibrageWebcam::FlipHorizontal() {
	this->flipHorizontal = !this->flipHorizontal;
}

void CalibrageWebcam::SetFlipHorizontal(bool flip) {
	this->flipHorizontal = flip;
}

// ---------------------------------------------------------------------
// Helper :flipping horizontal des données du masque, s'il y en a un :
void CalibrageWebcam::FlipMaskHorizontal() {
	if(maskDataRGB == NULL)
		return;

	for(int x = 0; x < maskWidth / 2; x++) {
		for(int y = 0; y < maskHeight; y++) {
			int cx = maskWidth - 1 - x;
			int i = (x + y * maskWidth) * 3;
			int ci = (cx + y * maskWidth) * 3;

			unsigned char rTemp = maskDataRGB[i + 0];
			unsigned char gTemp = maskDataRGB[i + 1];
			unsigned char bTemp = maskDataRGB[i + 2];

			maskDataRGB[i + 0] = maskDataRGB[ci + 0];
			maskDataRGB[i + 1] = maskDataRGB[ci + 1];
			maskDataRGB[i + 2] = maskDataRGB[ci + 2];

			maskDataRGB[ci + 0] = rTemp;
			maskDataRGB[ci + 1] = gTemp;
			maskDataRGB[ci + 2] = bTemp;
		}
	}
}
