// ImageFiltree.h
// Classe représentant une image en noir et (blanc ou gris), i.e. une image filtrée (dite aussi "binarisée").
// L'image est en noir et blanc tant que l'on n'a pas appelé RepererTaches(), et devient noire et grise
// après.

#ifndef IMAGE_FILTREE_H
#define IMAGE_FILTREE_H

#include "CGTKwin.h"
#include "Color.h"
#include "ColorHSV.h"
#include "ColorPercent.h"
#include <list>
using namespace std;

class ImageFiltree {
public:
	struct Tache // Représente une tache blanche sur l'image filtrée
	{
		int x, y, w, h;                 // Coordonnées du rectangle entourant la tache
		int x_barycentre, y_barycentre; // Coordonnées du barycentre
		int aire;
		Tache() : x(0), y(0), w(0), h(0), x_barycentre(0), y_barycentre(0), aire(0) {}
		Tache(const Tache& ref) {
			*this = ref;
		}

		Tache& operator=(const Tache& ref) {
			this->x = ref.x;
			this->y = ref.y;
			this->w = ref.w;
			this->h = ref.h;
			this->x_barycentre = ref.x_barycentre;
			this->y_barycentre = ref.y_barycentre;
			this->aire = ref.aire;

			return *this;
		}
	};

	struct ParamsTaches {
		int width_min, height_min, aire_min;
		int width_max, height_max, aire_max;
	};

protected:
	int m_width, m_height;
	unsigned char* m_data;     // L'image, en noir et gris (1 octet pour 1 pixel)
	unsigned char* m_data_rgb; // L'image en noir et gris (3 octets pour 1 pixel).
	                           // Attention : n'est mise à jour que lors de l'appel à GetDataRGB()
	                           // et n'est présente que pour le debug

	Color m_color_min;
	Color m_color_max;
	ColorPercent m_color_percent_min;
	ColorPercent m_color_percent_max;
	ColorHSV m_color_hsv_min;
	ColorHSV m_color_hsv_max;

	list<Tache> m_list_taches;

private:
	// Fonction helper utilisée par RepererTaches(), qui fait un parcourt en largeur d'abord sur
	// la tache dont l'un des pixels est précisé en argument.
	Tache RemplirTache(int x, int y);

	void GriserPixel(int x, int y) {
		m_data[x + y * m_width] = 0x7F;
	}

	bool PixelEstBlanc(int x, int y) {
		return m_data[x + y * m_width] == 0xFF;
	}

	// Helper qui teste si la valeur de teinte h se trouve entre les 2 valeurs indiquées
	// (avec h_min < h_max). On tient compte du côté "circulaire" de la teinte (Cf GIMP ^^)
	static bool HInInterval(float h, float h_min, float h_max) {
		float delta_h = h_max - h_min;

		if(delta_h > 180.f)
			return (h_max <= h && h <= 360.f) || (0 <= h && h <= h_min);
		else
			return (h_min <= h && h <= h_max);
	}

public:
	ImageFiltree();
	virtual ~ImageFiltree();

	virtual void Init(int width, int height); // Initialisation : crée le buffer

	// Retournement de l'image :
	void HorizontalFlip();
	void VerticalFlip();

	// Remplit le buffer de pixels noirs ou blancs suivant data_original
	// et les paramètres de filtrage RGB (couleurs max et min et les versions en pourcentages)
	void UpdateFromData(const unsigned char* data_original);

	// Pareil mais avec les paramètres en HSV
	void UpdateFromDataHSV(const unsigned char* data_original);

	// Calcul d'un ET logique entre un masque et les données.
	void ApplyMask(const unsigned char* mask_data);
	void ApplyMaskRGB(const unsigned char* mask_data); // version RGB : n'utilise que le rouge du masque

	void Free(); // Libération de la mémoire

	// Renseigne correctement m_list_taches.
	// NB : les largeurs et hauteurs données en paramètres sont celles du rectangle entourant la tache.
	void RepererTaches(int width_min, int height_min, int aire_min, int width_max, int height_max, int aire_max);

	void RepererTaches(const ParamsTaches& params) {
		RepererTaches(params.width_min, params.height_min, params.aire_min, params.width_max, params.height_max, params.aire_max);
	}

	// Floutage pour une image en noir et blanc
	void FlouterBlanc();

	// Floutage pour une image en noir et blanc avec une plus grande aire ^^
	void SuperFlouterBlanc();

	// Gets/Sets :

	// Taille
	int GetWidth() const {
		return m_width;
	}
	int GetHeight() const {
		return m_height;
	}

	// Getter du buffer
	unsigned char* GetData() const {
		return m_data;
	}

	// Pour le debug : getter qui renvoie une image en RGB représentant le buffer
	unsigned char* GetDataRGB();

	// Paramètres de filtrage :
	void SetColorMin(const Color& color_min) {
		m_color_min = color_min;
	}
	const Color& GetColorMin() const {
		return m_color_min;
	}

	void SetColorMax(const Color& color_max) {
		m_color_max = color_max;
	}
	const Color& GetColorMax() const {
		return m_color_max;
	}

	void SetColorPercentMin(const ColorPercent& color_percent_min) {
		m_color_percent_min = color_percent_min;
	}
	const ColorPercent& GetColorPercentMin() const {
		return m_color_percent_min;
	}

	void SetColorPercentMax(const ColorPercent& color_percent_max) {
		m_color_percent_max = color_percent_max;
	}
	const ColorPercent& GetColorPercentMax() const {
		return m_color_percent_max;
	}

	void SetColorHSVMin(const ColorHSV& color_hsv_min) {
		m_color_hsv_min = color_hsv_min;
	}
	const ColorHSV& GetColorHSVMin() const {
		return m_color_hsv_min;
	}

	void SetColorHSVMax(const ColorHSV& color_hsv_max) {
		m_color_hsv_max = color_hsv_max;
	}
	const ColorHSV& GetColorHSVMax() const {
		return m_color_hsv_max;
	}

	// Liste des taches
	const list<Tache>& GetListeTaches() const {
		return m_list_taches;
	}

	// Affichage des paramètres de l'ImageFiltree
	void PrintDebugInfo() const;

	// Pour la calibration : ajout à une fenêtre GTK
	void AddToGTK(const std::string& txt, NDebug::CGTKwin* win) {
		win->Attach(m_color_hsv_min.h, txt + " : H min", 0.0f, 360.0f);
		win->Attach(m_color_hsv_min.s, txt + " : S min", 0.0f, 100.0f);
		win->Attach(m_color_hsv_min.v, txt + " : V min", 0.0f, 100.0f);

		win->Attach(m_color_hsv_max.h, txt + " : H max", 0.0f, 360.0f);
		win->Attach(m_color_hsv_max.s, txt + " : S max", 0.0f, 100.0f);
		win->Attach(m_color_hsv_max.v, txt + " : V max", 0.0f, 100.0f);
	}
};

#endif // IMAGE_FILTREE_H
