// ImageFiltreeMasquee.h
// Classe qui ajout à ImageFiltree la possibilité de définir des zones qui doivent
// et d'autres qui ne doivent pas être testées (i.e. un masque).
// Pour l'instant, le masque ne peut être constitué que de rectangles.
// TODO : précalculer cos et sin et peut-être ne plus dépendre de Vector2.h

#ifndef IMAGE_FILTREE_MASQUEE_H
#define IMAGE_FILTREE_MASQUEE_H

#include "ImageFiltree.h"
#include <list>
using namespace std;

// Activation ou non du calcul du masque de debug
#define IMAGE_FILTREE_MASQUEE_DEBUG

class ImageFiltreeMasquee : public ImageFiltree {
public:
	// Structure représentant un rectangle sur le terrain
	struct Rectangle {
		// NB : on considère que l'utilisateur ne va pas rentrer n'importe quoi comme valeurs pour
		// x_min, y_min, x_max et y_max...
		int x_min, y_min; // Position du point bas-gauche du rectangle
		int x_max, y_max; // Position du point haut-droite du rectangle

		bool ok; // Booléen, qui indique si le rectangle est une zone à garder dans le traitement
		         // (true) ou une zone à ne pas tester (false)
		bool PointDansRectangle(int px, int py) {
			return px >= x_min && px <= x_max && py >= y_min && py <= y_max;
		}
	};

private:
	// Liste des rectangles qui définissent les zones à traiter et celles à ne pas traiter
	// lors d'une recherche de taches.
	list<Rectangle> m_rectangles;

	// Petite et grande largeur du parallélogramme visé par la cam, en mm
	int m_width_target_min;
	int m_width_target_max;
	int m_height_target;
	int m_dist_webcam;

// Pour le debug : correspond à une image qui permet de voir les zones interdites / ok
#ifdef IMAGE_FILTREE_MASQUEE_DEBUG
	unsigned char* m_masque_data;
#endif

	// Mise à jour de m_image_to_target
	void UpdateInternal();

public:
	ImageFiltreeMasquee();
	virtual ~ImageFiltreeMasquee();

	// Init :
	virtual void Init(int width, int height);

	// Getters / setters pour les dimensions du parallélogramme visé par la cam
	// + sa distance par rapport à la webcam
	void SetTarget(int width_target_min, int width_target_max, int height_target, int dist_webcam) {
		m_width_target_min = width_target_min;
		m_width_target_max = width_target_max;
		m_height_target = height_target;
		m_dist_webcam = dist_webcam;

		UpdateInternal();
	}

	int GetWidthTargetMin() {
		return m_width_target_min;
	}
	int GetWidthTargetMax() {
		return m_width_target_min;
	}
	int GetHeightTarget() {
		return m_height_target;
	}
	int GetDistWebcam() {
		return m_dist_webcam;
	}

	// Fonction qui calcule sur le terrain la position du pixel indiqué par pixel_x et pixel_y.
	// Utilisée par les fonctions plus bas.
	// pos_x, pos_y : mm : position de la webcam sur le terrain
	// theta : mrad : angle fait par le robot par rapport à l'axe des x
	// pixel_x, pixel_y : position dans l'image du pixel à tester
	void ProjeterPixel(int pos_x, int pos_y, int theta, int pixel_x, int pixel_y, int* ptarget_x, int* ptarget_y);

	// Fonction qui détermine si un point est masqué ou pas, en fonction des rectangles.
	// Utilisée par les fonctions plus bas.
	// pos_x, pos_y : mm : position de la webcam sur le terrain
	// theta : mrad : angle fait par le robot par rapport à l'axe des x
	// pixel_x, pixel_y : position dans l'image du pixel à tester
	bool EstPointMasque(int pos_x, int pos_y, int theta, int pixel_x, int pixel_y);

	// Fonction qui supprime de m_list_taches les taches qui sont masquées
	void SupprimerTachesMasquees(int pos_x, int pos_y, int theta);

// Pour le debug :
#ifdef IMAGE_FILTREE_MASQUEE_DEBUG
	unsigned char* CalculerMasque(int pos_x, int pos_y, int theta);
#endif

	// Lecture et modifications de la liste des rectangles
	void AddRectangle(Rectangle rect) {
		m_rectangles.push_back(rect);
	}
	void RemoveRectangles() {
		m_rectangles.clear();
	}
	const list<Rectangle>& GetRectangles() {
		return m_rectangles;
	}
};

#endif // IMAGE_FILTREE_MASQUEE_H
