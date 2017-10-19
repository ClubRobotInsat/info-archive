// CalibrageWebcam.h

#ifndef CALIBRAGE_WEBCAM_H
#define CALIBRAGE_WEBCAM_H

#include "../../maths/Vector2.h"
#include "Color.h"
#include "TGALoader.h"
#include <string.h>
#include <string>

// Nombre de mais
#define NB_MAIS_LATERAUX 6
#define NB_MAIS_CENTRAUX 4
#define NB_MAIS_TOTAL (NB_MAIS_LATERAUX + NB_MAIS_CENTRAUX)

// Structure représentant le résultat du programme de calibrage de la webcam.
// Elle peut etre sérialisée dans un fichier de configuration.
struct CalibrageWebcam {
public:
	CalibrageWebcam();
	CalibrageWebcam(const CalibrageWebcam& ref);
	~CalibrageWebcam();

	// Ecriture / lecture depuis un fichier :
	bool Ecrire(const char* filename) const;
	bool Lire(const char* filename, int camWidth, int camHeight);

	// Ecriture / lecture depuis un stream quelconque :
	void Ecrire(std::ostream& os) const;
	bool Lire(std::istream& is, int camWidth, int camHeight);

	// Chargement du masque précisé dans maskFilename
	bool LoadMask(const std::string& maskFilename, int camWidth, int camHeight);
	bool LoadMaskFromData(unsigned char* data, int camWidth, int camHeight);
	bool LoadMask(TGALoader& tga, int camWidth, int camHeight);

	// Affichage dans la console
	void Print() const;

	// Calcul des couleurs maximale et minimale pour le noir et pour le blanc
	void GetCouleurNoirMinMax(Color* noirMin, Color* noirMax) const;
	void GetCouleurBlancMinMax(Color* blancMin, Color* blancMax) const;

	// Opérateur d'affectation :
	CalibrageWebcam& operator=(const CalibrageWebcam& ref);

public:
	Color couleurNoir;
	Color couleurBlanc;

	int ecartCouleurNoir, ecartCouleurBlanc;

	int widthMin, heightMin;
	int widthMax, heightMax;

	// Positions des taches (en pixels)
	Vector2i maisLateraux[NB_MAIS_LATERAUX];
	Vector2i maisCentraux[NB_MAIS_CENTRAUX];

private:
	// Doit-on faire un flip horizontal de l'image ?
	bool flipHorizontal;

	// Le masque.
	// Convention : comme pour le reste (avec ImageFiltree...),
	// dans les données du masque, l'origine est en haut à gauche.
	// Lors du chargement du fichier TGA, on fait un flip vertical pour suivre cette convention.
	std::string maskFilename;
	int maskWidth, maskHeight;
	unsigned char* maskDataRGB; // NULL s'il n'y a pas de masque

public:
	// Getters / setters pour les variables privées :
	void FlipHorizontal();             // Lorsque l'on change l'état de flipHorizontal, on
	void SetFlipHorizontal(bool flip); // met à jour le masque.

	bool IsFlipHorizontal() const {
		return flipHorizontal;
	}

	const std::string GetMaskFilename() const {
		return maskFilename;
	}

	int GetMaskWidth() const {
		return maskWidth;
	}
	int GetMaskHeight() const {
		return maskHeight;
	}

	const unsigned char* GetMaskDataRGB() const {
		return maskDataRGB;
	}
	bool HasMask() const {
		return maskDataRGB != NULL;
	}

private:
	// Helper : flipping horizontal des données du masque, s'il y en a un :
	void FlipMaskHorizontal();
};

#endif // CALIBRAGE_WEBCAM_H
