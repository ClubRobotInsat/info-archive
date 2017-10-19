// ImageFiltreeMasquee.cpp

#include "ImageFiltreeMasquee.h"
#include "maths/Vector2.h"
#include <stdio.h>
#include <string.h>

ImageFiltreeMasquee::ImageFiltreeMasquee()
        : m_width_target_min(0), m_width_target_max(0), m_height_target(0), m_dist_webcam(0) {}

ImageFiltreeMasquee::~ImageFiltreeMasquee() {
#ifdef IMAGE_FILTREE_MASQUEE_DEBUG
	delete[] m_masque_data;
#endif
}

// Init :
void ImageFiltreeMasquee::Init(int width, int height) {
	ImageFiltree::Init(width, height);
#ifdef IMAGE_FILTREE_MASQUEE_DEBUG
	m_masque_data = new unsigned char[m_width * m_height * 3];
#endif
}

// Mise à jour de m_image_to_target
void ImageFiltreeMasquee::UpdateInternal() {
	// Calcul de m_image_to_target
	/*	m_image_to_target[0] = (float)(m_width_target_min) / (float)(m_width);
	    m_image_to_target[1] = 0.0f;
	    m_image_to_target[2] = (float)(m_width_target_max) / (float)(m_width * m_height);
	    m_image_to_target[3] = (float)(m_height_target) / (float)(m_height);

	    printf("DEBUG : m_image_to_target == \n");
	    printf("%.3f %.3f\n", m_image_to_target[0], m_image_to_target[2]);
	    printf("%.3f %.3f\n", m_image_to_target[1], m_image_to_target[3]);
	*/
}

// Fonction qui calcule sur le terrain la position du pixel indiqué par pixel_x et pixel_y.
// Utilisée par les fonctions plus bas.
// pos_x, pos_y : mm : position de la webcam sur le terrain
// theta : mrad : angle fait par le robot par rapport à l'axe des x
// pixel_x, pixel_y : position dans l'image du pixel à tester
void ImageFiltreeMasquee::ProjeterPixel(int pos_x, int pos_y, int theta, int pixel_x, int pixel_y, int* ptarget_x, int* ptarget_y) {
	// Casts en float :
	float w = (float)m_width;
	float h = (float)m_height;
	float W_min = (float)(m_width_target_min);
	float W_max = (float)(m_width_target_max);
	float H = (float)(m_height_target);
	float dist_webcam = (float)m_dist_webcam;
	float theta_rad_rotation = (((float)theta) / 1000.0f) - M_PI / 2.0f;
	float fpos_x = (float)pos_x;
	float fpos_y = (float)pos_y;

	// On décale l'origine du repère dans lequel est exprimé le pixel pour qu'il s'agisse
	// du point au centre en bas de l'image.
	float fx = (float)(pixel_x - m_width / 2);
	float fy = (float)(pixel_y);

	//	printf("DEBUG : depart : fx == %f, fy == %f\n", fx, fy);

	// On applique la transformation qui permet de passer du rectangle de la caméra au parallélogramme
	// du champ de vision de la caméra sur la table.

	// - interpolation linéaire sur y :
	fy = fy * H / h;

	// - calcul de la largeur du parallélogramme lorsque l'on se place à la hauteur fy :
	float W_fy = W_min + (W_max - W_min) * (fy / H);

	// - calcul de la coordonnée x de la même façon que pour y :
	fx = fx * W_fy / w;

	//	printf("W_fy : %f\n", W_fy);

	//	printf("DEBUG : arrivee : fx == %f, fy == %f\n", fx, fy);

	// printf("DEBUG : target == (%f, %f)\n", target.x, target.y);

	// Jusqu'ici, on a considéré que le parallélogramme "target" se trouvait à l'origine du repère.
	// Maintenant, on bouge selon la distance entre le parallélogramme et la position du point de vue,
	// la position du robot sur le terrain et l'angle du robot.

	// - on avance d'autant que la projection de la tache sur la table est loin de la webcam
	fy += dist_webcam;

	// - on tourne de l'angle du robot : pour cela, on multiplie par la matrice :
	// ( cos(theta), -sin(theta) )
	// ( sin(theta), cos(theta)  )

	float cos_angle = cos(theta_rad_rotation);
	float sin_angle = sin(theta_rad_rotation);
	float old_x = fx;

	fx = cos_angle * old_x - sin_angle * fy;
	fy = sin_angle * old_x + cos_angle * fy;

	// - on va à la position du robot
	fx += fpos_x;
	fy += fpos_y;

	/*	target.y += (float)m_dist_webcam;

	    // printf("DEBUG : [apres target.y += dist_webcam] : target == (%f, %f)\n", target.x, target.y);

	    // - on tourne de l'angle du robot
	    target.rotate((((double)theta) / 1000.0) - M_PI/2.0);

	    // printf("DEBUG : [apres rotate] : target == (%f, %f)\n", target.x, target.y);

	    // - on va à la position du robot
	    target.x += (float)pos_x;
	    target.y += (float)pos_y;
	*/
	// printf("DEBUG : [apres target += (pos_x, pos_y) : target == (%f, %f)\n", target.x, target.y);

	// Résultat :
	*ptarget_x = (int)fx;
	*ptarget_y = (int)fy;
}

// Fonction qui détermine si un point est masqué ou pas, en fonction des rectangles.
// Utilisée par les fonctions plus bas.
// pos_x, pos_y : mm : position de la webcam sur le terrain
// theta : mrad : angle fait par le robot par rapport à l'axe des x
// pixel_x, pixel_y : position dans l'image du pixel à tester
bool ImageFiltreeMasquee::EstPointMasque(int pos_x, int pos_y, int theta, int pixel_x, int pixel_y) {
	// Calcul de la position du pixel dans le repère du terrain :
	int target_x = 0;
	int target_y = 0;
	ProjeterPixel(pos_x, pos_y, theta, pixel_x, pixel_y, &target_x, &target_y);

	// Parcours des rectangles pour voir si le point est masqué ou pas :
	bool ok = false; // Indique si le point considéré est masqué ou pas.

	// Un point n'est considéré OK que s'il est dans un rectangle OK ET qu'il n'est pas dans un rectangle KO
	// Sinon, par défaut, il est considéré comme KO.

	for(list<Rectangle>::iterator it = m_rectangles.begin(); it != m_rectangles.end(); it++) {
		Rectangle& rect = (*it);

		// Si le point est dans un rectangle KO, on arrête là
		if(rect.ok == false && rect.PointDansRectangle(target_x, target_y)) {
			ok = false;
			break;
		}

		// Si le point est dans un rectangle OK, on le marque, mais c'est pas encore gagné !
		if(rect.ok == true && rect.PointDansRectangle(target_x, target_y))
			ok = true;
	}

	return !ok;
}

// Fonction qui supprime de m_list_taches les taches qui sont masquées
void ImageFiltreeMasquee::SupprimerTachesMasquees(int pos_x, int pos_y, int theta) {
	// Parcours des taches
	list<Tache>::iterator it = m_list_taches.begin();

	while(it != m_list_taches.end()) {
		Tache& tache = (*it);

		// Si la tache est masquée, on la retire de la liste
		if(EstPointMasque(pos_x, pos_y, theta, tache.x, tache.y))
			it = m_list_taches.erase(it); // erase() renvoie l'itérateur vers l'élément suivant dans la liste
		// Sinon on la garde
		else
			it++;
	}
}

// Pour le debug :
#ifdef IMAGE_FILTREE_MASQUEE_DEBUG
unsigned char* ImageFiltreeMasquee::CalculerMasque(int pos_x, int pos_y, int theta) {
	for(int x = 0; x < m_width; x++) {
		for(int y = 0; y < m_height; y++) {
			if(EstPointMasque(pos_x, pos_y, theta, x, y)) {
				m_masque_data[(x + y * m_width) * 3 + 0] = 0x00;
				m_masque_data[(x + y * m_width) * 3 + 1] = 0x00;
				m_masque_data[(x + y * m_width) * 3 + 2] = 0x00;
			} else {
				m_masque_data[(x + y * m_width) * 3 + 0] = 0xFF;
				m_masque_data[(x + y * m_width) * 3 + 1] = 0xFF;
				m_masque_data[(x + y * m_width) * 3 + 2] = 0xFF;
			}
		}
	}

	return m_masque_data;
}
#endif
