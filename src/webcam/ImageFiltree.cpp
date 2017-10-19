// ImageFiltree.cpp

#include "ImageFiltree.h"
#include "Pos2D.h"
#include <queue>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

ImageFiltree::ImageFiltree()
        : m_width(0)
        , m_height(0)
        , m_data(NULL)
        , m_data_rgb(NULL)
        , m_color_min(COLOR_BLACK)
        , m_color_max(COLOR_WHITE)
        , m_color_percent_min(COLOR_PERCENT_BLACK)
        , m_color_percent_max(COLOR_PERCENT_WHITE)
        , m_list_taches() {}

ImageFiltree::~ImageFiltree() {
	Free();
}

void ImageFiltree::Init(int width, int height) {
	Free();
	m_width = width;
	m_height = height;
	m_data = new unsigned char[m_width * m_height];
	m_data_rgb = new unsigned char[m_width * m_height * 3];
}

// Retournement de l'image :
void ImageFiltree::HorizontalFlip() {
	for(int y = 0; y < m_height; y++) {
		for(int x = 0; x < m_width / 2; x++) {
			int cx = m_width - 1 - x;
			int i = x + y * m_width;
			int ci = cx + y * m_width;

			unsigned char r_temp = m_data[i * 3 + 0];
			unsigned char g_temp = m_data[i * 3 + 1];
			unsigned char b_temp = m_data[i * 3 + 2];

			m_data[i * 3 + 0] = m_data[ci * 3 + 0];
			m_data[i * 3 + 1] = m_data[ci * 3 + 1];
			m_data[i * 3 + 2] = m_data[ci * 3 + 2];

			m_data[ci * 3 + 0] = r_temp;
			m_data[ci * 3 + 1] = g_temp;
			m_data[ci * 3 + 2] = b_temp;
		}
	}
}

void ImageFiltree::VerticalFlip() {
	for(int y = 0; y < m_height / 2; y++) {
		for(int x = 0; x < m_width; x++) {
			int cy = m_height - 1 - y;
			int i = x + y * m_width;
			int ci = x + cy * m_width;

			unsigned char r_temp = m_data[i * 3 + 0];
			unsigned char g_temp = m_data[i * 3 + 1];
			unsigned char b_temp = m_data[i * 3 + 2];

			m_data[i * 3 + 0] = m_data[ci * 3 + 0];
			m_data[i * 3 + 1] = m_data[ci * 3 + 1];
			m_data[i * 3 + 2] = m_data[ci * 3 + 2];

			m_data[ci * 3 + 0] = r_temp;
			m_data[ci * 3 + 1] = g_temp;
			m_data[ci * 3 + 2] = b_temp;
		}
	}
}

// Remplit le buffer de pixels noirs ou blancs suivant data_original
// et les paramètres de filtrage RGB (couleurs max et min et les versions en pourcentages)
// Applique ensuite le masque.
void ImageFiltree::UpdateFromData(const unsigned char* data_original) {
	if(!m_data)
		return;

	int x = 0, y = 0;

	// Parcourt de l'image
	for(x = 0; x < m_width; x++) {
		for(y = 0; y < m_height; y++) {
			// "pixel" : couleur du pixel parcouru
			Color pixel(data_original[(x + y * m_width) * 3],
			            data_original[(x + y * m_width) * 3 + 1],
			            data_original[(x + y * m_width) * 3 + 2]);

			// "pixel_pourcentage" : pourcentage de chaque couleur dans le pixel
			ColorPercent pixel_pourcentage;
			float somme_composantes = (float)pixel.r + (float)pixel.g + (float)pixel.b;
			if(somme_composantes) {
				pixel_pourcentage.r = ((float)pixel.r) / somme_composantes;
				pixel_pourcentage.g = ((float)pixel.g) / somme_composantes;
				pixel_pourcentage.b = ((float)pixel.b) / somme_composantes;
			}

			if( // Test des valeurs en absolu
			    pixel.r > m_color_max.r || pixel.g > m_color_max.g || pixel.b > m_color_max.b ||

			    pixel.r < m_color_min.r || pixel.g < m_color_min.g || pixel.b < m_color_min.b ||

			    // Test des valeurs en relatif
			    pixel_pourcentage.r > m_color_percent_max.r ||
			    pixel_pourcentage.g > m_color_percent_max.g || pixel_pourcentage.b > m_color_percent_max.b ||

			    pixel_pourcentage.r < m_color_percent_min.r || pixel_pourcentage.g < m_color_percent_min.g ||
			    pixel_pourcentage.b < m_color_percent_min.b) {
				m_data[x + y * m_width] = 0x00;
			} else {
				m_data[x + y * m_width] = 0xFF;
			}
		}
	}
}

// Pareil mais avec les paramètres en HSV
void ImageFiltree::UpdateFromDataHSV(const unsigned char* data_original) {
	if(!m_data)
		return;

	int x = 0, y = 0;

	// Parcourt de l'image
	for(x = 0; x < m_width; x++) {
		for(y = 0; y < m_height; y++) {
			// "pixel" : couleur du pixel parcouru
			ColorHSV pixel = Color(data_original[(x + y * m_width) * 3],
			                       data_original[(x + y * m_width) * 3 + 1],
			                       data_original[(x + y * m_width) * 3 + 2]);

			if( // Test des valeurs en absolu
			    !HInInterval(pixel.h, m_color_hsv_min.h, m_color_hsv_max.h) || pixel.s > m_color_hsv_max.s ||
			    pixel.v > m_color_hsv_max.v ||

			    pixel.s < m_color_hsv_min.s || pixel.v < m_color_hsv_min.v) {
				m_data[x + y * m_width] = 0x00;
			} else {
				m_data[x + y * m_width] = 0xFF;
			}
		}
	}
}

// Calcul d'un ET logique entre un masque et les données.
void ImageFiltree::ApplyMask(const unsigned char* mask_data) {
	if(mask_data == NULL)
		return;

	int size = m_width * m_height;
	for(int i = 0; i < size; i++)
		m_data[i] &= mask_data[i];
}

// Version RGB : n'utilise que le rouge du masque
void ImageFiltree::ApplyMaskRGB(const unsigned char* mask_data) {
	if(mask_data == NULL)
		return;

	int size = m_width * m_height;
	for(int i = 0; i < size; i++)
		m_data[i] &= mask_data[i * 3];
}

// ---------------------------------------------------------------------
void ImageFiltree::Free() {
	if(m_data != NULL) {
		m_width = 0;
		m_height = 0;
		delete[] m_data;
		delete[] m_data_rgb;
		m_data = NULL;
	}
}

// Fonction qui parcourt l'image filtrée (noire et blanche) et qui dès qu'elle tombe sur un pixel blanc, appelle
// RemplirTache().
// On renseigne ainsi m_list_taches. On ne garde que les taches dont les tailles sont comprises dans l'intervalle
// précisé.
// NB : les largeurs et hauteurs données en paramètres sont celles du rectangle entourant la tache.
void ImageFiltree::RepererTaches(int width_min, int height_min, int aire_min, int width_max, int height_max, int aire_max) {
	int x, y;
	Tache tache;
	m_list_taches.clear();

	// Parcourt de l'image
	for(x = 0; x < m_width; x++) {
		for(y = 0; y < m_height; y++) {
			// Si c'est un pixel blanc : RemplirTache()
			if(m_data[x + y * m_width] == 0xFF) {
				tache = RemplirTache(x, y);

				// Si la tache satisfait aux critères de taille et d'aire, on la rajoute à m_list_taches
				if(tache.w >= width_min && tache.h >= height_min && tache.aire >= aire_min && tache.w <= width_max &&
				   tache.h <= height_max && tache.aire <= aire_max) {
					m_list_taches.push_back(tache);
					/*printf("Tache ACCEPTEE : w, h, aire, barycentre : %d, %d, %d, (%d, %d)\n", tache.w, tache.h,
					   tache.aire,
					   tache.x_barycentre, tache.y_barycentre);*/
				}
			}
		}
	}
}

// Floutage pour une image en noir et blanc
void ImageFiltree::FlouterBlanc() {
	unsigned char* new_data = new unsigned char[m_width * m_height];

	for(int x = 0; x < m_width; x++) {
		for(int y = 0; y < m_height; y++) {
			if(x >= 1 && x < m_width - 1 && y >= 1 && y < m_height - 1) {
				if(m_data[x - 1 + (y - 1) * m_width] != 0x00 || // Bas
				   m_data[x + (y - 1) * m_width] != 0x00 ||
				   m_data[x + 1 + (y - 1) * m_width] != 0x00 ||

				   m_data[x - 1 + y * m_width] != 0x00 || // Milieu
				   m_data[x + y * m_width] != 0x00 ||
				   m_data[x + 1 + y * m_width] != 0x00 ||

				   m_data[x - 1 + (y + 1) * m_width] != 0x00 || // Haut
				   m_data[x + (y + 1) * m_width] != 0x00 ||
				   m_data[x + 1 + (y + 1) * m_width] != 0x00) {
					new_data[x + y * m_width] = 0xFF;
				} else {
					new_data[x + y * m_width] = 0x00;
				}
			} else
				new_data[x + y * m_width] = 0x00; // Pixels aux bords
		}
	}

	delete[] m_data;
	m_data = new_data;
}

// Floutage pour une image en noir et blanc avec une plus grande aire ^^
void ImageFiltree::SuperFlouterBlanc() {
	unsigned char* new_data = new unsigned char[m_width * m_height];

	for(int x = 0; x < m_width; x++) {
		for(int y = 0; y < m_height; y++) {
			if(x >= 2 && x < m_width - 2 && y >= 2 && y < m_height - 2) {
				if(m_data[x - 2 + (y - 2) * m_width] != 0x00 || // Super-Bas
				   m_data[x - 1 + (y - 2) * m_width] != 0x00 ||
				   m_data[x + (y - 2) * m_width] != 0x00 || m_data[x + 1 + (y - 2) * m_width] != 0x00 ||
				   m_data[x + 2 + (y - 2) * m_width] != 0x00 ||

				   m_data[x - 2 + (y - 1) * m_width] != 0x00 || // Bas
				   m_data[x - 1 + (y - 1) * m_width] != 0x00 ||
				   m_data[x + (y - 1) * m_width] != 0x00 || m_data[x + 1 + (y - 1) * m_width] != 0x00 ||
				   m_data[x + 2 + (y - 1) * m_width] != 0x00 ||

				   m_data[x - 2 + y * m_width] != 0x00 || // Milieu
				   m_data[x - 1 + y * m_width] != 0x00 ||
				   m_data[x + y * m_width] != 0x00 || m_data[x + 1 + y * m_width] != 0x00 || m_data[x + 2 + y * m_width] != 0x00 ||

				   m_data[x - 2 + (y + 1) * m_width] != 0x00 || // Haut
				   m_data[x - 1 + (y + 1) * m_width] != 0x00 ||
				   m_data[x + (y + 1) * m_width] != 0x00 || m_data[x + 1 + (y + 1) * m_width] != 0x00 ||
				   m_data[x + 2 + (y + 1) * m_width] != 0x00 ||

				   m_data[x - 2 + (y + 2) * m_width] != 0x00 || // Super-Haut
				   m_data[x - 1 + (y + 2) * m_width] != 0x00 ||
				   m_data[x + (y + 2) * m_width] != 0x00 || m_data[x + 1 + (y + 2) * m_width] != 0x00 ||
				   m_data[x + 2 + (y + 2) * m_width] != 0x00) {
					new_data[x + y * m_width] = 0xFF;
				} else {
					new_data[x + y * m_width] = 0x00;
				}
			} else
				new_data[x + y * m_width] = 0x00; // Pixels aux bords
		}
	}

	delete[] m_data;
	m_data = new_data;
}

// Fonction qui effectue un parcourt en largeur d'abord dans la tache à laquelle le pixel en (x, y) appartient.
// Elle remplit au fur et à mesure la tache de gris.
ImageFiltree::Tache ImageFiltree::RemplirTache(int x, int y) {
	Tache tache;
	tache.aire = 0;

	// Coordonnées du rectangle entourant la tache
	int x_max = x;
	int y_max = y;
	int x_min = x;
	int y_min = y;

	queue<Pos2D> queue_pixels_non_examines; // File FIFO indiquant quels sont les pixels qui n'ont pas encore été
	                                        // examinés

	long somme_x = 0, somme_y = 0; // Somme des x et des y de tous les pixels de la tache (pour le calcul du barycentre)

	// On enfile d'abord le pixel donné en paramètre et on le marque comme déjà examiné (pixel gris)
	queue_pixels_non_examines.push(Pos2D(x, y));
	GriserPixel(x, y);

	// Tant que la file n'est pas vide (i.e. tant qu'il reste des pixels à examiner)
	while(queue_pixels_non_examines.size() > 0) {
		// On examine le 1er de la file et on l'enlève de la file
		Pos2D pixel = queue_pixels_non_examines.front();
		queue_pixels_non_examines.pop();

		// On marque le pixel examiné
		GriserPixel(pixel.x, pixel.y);

		// On enfile les voisins du pixel examiné qui ne sont pas marqués et qui appartiennent à la tache (ni gris ni
		// noirs, donc blancs),
		// et on les marque
		// Haut
		if(pixel.y > 0 && PixelEstBlanc(pixel.x, pixel.y - 1)) {
			queue_pixels_non_examines.push(Pos2D(pixel.x, pixel.y - 1));
			GriserPixel(pixel.x, pixel.y - 1);
		}
		// Bas
		if(pixel.y < m_height - 1 && PixelEstBlanc(pixel.x, pixel.y + 1)) {
			queue_pixels_non_examines.push(Pos2D(pixel.x, pixel.y + 1));
			GriserPixel(pixel.x, pixel.y + 1);
		}
		// Gauche
		if(pixel.x > 0 && PixelEstBlanc(pixel.x - 1, pixel.y)) {
			queue_pixels_non_examines.push(Pos2D(pixel.x - 1, pixel.y));
			GriserPixel(pixel.x - 1, pixel.y);
		}
		// Droite
		if(pixel.x < m_width - 1 && PixelEstBlanc(pixel.x + 1, pixel.y)) {
			queue_pixels_non_examines.push(Pos2D(pixel.x + 1, pixel.y));
			GriserPixel(pixel.x + 1, pixel.y);
		}

		// On incrémente l'aire et on met à jour si besoin est les coordonnées du rectangle entourant la tache
		tache.aire++;
		if(pixel.x < x_min)
			x_min = pixel.x;
		if(pixel.x > x_max)
			x_max = pixel.x;
		if(pixel.y < y_min)
			y_min = pixel.y;
		if(pixel.y > y_max)
			y_max = pixel.y;

		// On incrémente la somme des x et des y de tous les pixels (pour le calcul du barycentre)
		somme_x += pixel.x;
		somme_y += pixel.y;
	}

	tache.w = x_max - x_min;
	tache.h = y_max - y_min;
	tache.x = x_min;
	tache.y = y_min;

	// Calcul du barycentre de la tache
	tache.x_barycentre = somme_x / tache.aire;
	tache.y_barycentre = somme_y / tache.aire;

	return tache;
}

// Pour le debug : getter qui renvoie une image en RGB représentant le buffer
unsigned char* ImageFiltree::GetDataRGB() {
	for(int i = 0; i < m_width * m_height; i++)
		if(m_data[i] != 0x00)
			m_data_rgb[i * 3 + 0] = m_data_rgb[i * 3 + 1] = m_data_rgb[i * 3 + 2] = m_data[i];
		else {
			m_data_rgb[i * 3 + 0] = 0x00;
			m_data_rgb[i * 3 + 1] = 0xFF;
			m_data_rgb[i * 3 + 2] = 0x00;
		}
	return m_data_rgb;
}

// Affichage des paramètres de l'ImageFiltree
void ImageFiltree::PrintDebugInfo() const {
	printf("=============== DEBUT ImageFiltree::PrintDebugInfo() ===============\n");
	printf("w, h : %d, %d\n", m_width, m_height);
	printf("color min : %d, %d, %d\n", m_color_min.r, m_color_min.g, m_color_min.b);
	printf("color max : %d, %d, %d\n", m_color_max.r, m_color_max.g, m_color_max.b);
	printf("color percent min : %f, %f, %f\n", m_color_percent_min.r, m_color_percent_min.g, m_color_percent_min.b);
	printf("color percent max : %f, %f, %f\n", m_color_percent_max.r, m_color_percent_max.g, m_color_percent_max.b);
	printf("color hsv min : %f, %f, %f\n", m_color_hsv_min.h, m_color_hsv_min.s, m_color_hsv_min.v);
	printf("color hsv max : %f, %f, %f\n", m_color_hsv_max.h, m_color_hsv_max.s, m_color_hsv_max.v);
	printf("nb taches : %d\n", m_list_taches.size());
	printf("liste : ");
	for(list<Tache>::const_iterator it = m_list_taches.begin(); it != m_list_taches.end(); it++)
		printf("(%d, %d) ", (*it).x_barycentre, (*it).y_barycentre);
	printf("=============== FIN   ImageFiltree::PrintDebugInfo() ===============\n");
}

// Pour la calibration : ajout à une fenêtre GTK
void AddToGTK() {}
