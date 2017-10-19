// DebugMasques.cpp

#include "ImageFiltreeMasquee.h"

#ifndef IMAGE_FILTREE_MASQUEE_DEBUG
int main() {
	return 0;
}
#else

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_LARGEUR 3000
#define TABLE_HAUTEUR 2100

void SavePPM(const char* filename, unsigned char* data, int width, int height);
void ReverseImage(unsigned char* data, int width, int height);

int main(int argc, char* argv[]) {
	/*	int pos_x = 0;
	    int pos_y = 0;
	    int theta = (int)(M_PI*1000.0f / 2.0f);
	    int dist_webcam = 200;
	    int w_min_target = 340;
	    int w_max_target = 800;
	    int h_target = 660;
	*/
	int pos_x = 0;
	int pos_y = 0;
	int theta = (int)(M_PI * 1000.0f / 2.0f);
	int dist_webcam = 100;
	int w_min_target = 640;
	int w_max_target = 2 * 640;
	// int w_min_target = 640;
	// int w_max_target = 100000;
	int h_target = 480;
	int res_x = 0, res_y = 0;

	ImageFiltreeMasquee::Rectangle rect;

	ImageFiltreeMasquee image;
	image.Init(640, 480);
	image.SetTarget(w_min_target, w_max_target, h_target, dist_webcam);

	image.ProjeterPixel(pos_x, pos_y, theta, 50, 50, &res_x, &res_y);
	printf("Image de %d, %d : %d, %d\n", 50, 50, res_x, res_y);

	image.ProjeterPixel(pos_x, pos_y, theta, 150, 50, &res_x, &res_y);
	printf("Image de %d, %d : %d, %d\n", 150, 50, res_x, res_y);

	image.ProjeterPixel(pos_x, pos_y, theta, 150, 150, &res_x, &res_y);
	printf("Image de %d, %d : %d, %d\n", 150, 150, res_x, res_y);

	image.ProjeterPixel(pos_x, pos_y, theta, 50, 150, &res_x, &res_y);
	printf("Image de %d, %d : %d, %d\n", 50, 150, res_x, res_y);

	// Ajout d'un rectangle en (50, 50), de taille 100x100 :
	/*	rect.x_min = 50;
	    rect.x_max = 150;
	    rect.y_min = 50;
	    rect.y_max = 150;
	    rect.ok = true;
	    image.AddRectangle(rect);

	    rect.x_min = 0;
	    rect.x_max = 50;
	    rect.y_min = 0;
	    rect.y_max = 50;
	    rect.ok = true;
	    image.AddRectangle(rect);
	*/
	for(int y = 0; y < 10; y++) {
		for(int x = -5 + y % 2; x < 5; x += 2) {
			rect.x_min = x * 50;
			rect.x_max = (x + 1) * 50;
			rect.y_min = y * 50;
			rect.y_max = (y + 1) * 50;
			rect.ok = true;
			image.AddRectangle(rect);
		}
	}

	// Ajout de la table :
	/*	rect.x_min = -TABLE_LARGEUR/2;
	    rect.x_max = TABLE_LARGEUR/2;
	    rect.y_min = 0;
	    rect.y_max = TABLE_HAUTEUR;
	    image.AddRectangle(rect);
	*/

	system("rm -f masques/*.ppm");
	// for(pos_x=0 ; pos_x < 100 ; pos_x += 10)
	// for(pos_y=0 ; pos_y < 100 ; pos_y += 10)
	for(theta = (int)(M_PI * 1000.0f / 2.0f); theta > 0; theta -= 200) {
		static int c = 'a';
		char file_name[100] = "";
		sprintf(file_name, "masques/masque_%c.ppm", c++);

		unsigned char* data = image.CalculerMasque(pos_x, pos_y, theta);
		ReverseImage(data, image.GetWidth(), image.GetHeight());
		SavePPM(file_name, data, image.GetWidth(), image.GetHeight());
	}

	/*unsigned char* data = image.CalculerMasque(pos_x, pos_y, theta);
	ReverseImage(data, image.GetWidth(), image.GetHeight());
	SavePPM("masque.ppm", data, image.GetWidth(), image.GetHeight());
	system("display masque.ppm");*/

	return 0;
}

void SavePPM(const char* filename, unsigned char* data, int width, int height) {
	// Ouverture du fichier
	FILE* f = fopen(filename, "wb");
	if(!f) {
		fprintf(stderr, "Erreur : impossible de creer %s\n", filename);
		return;
	}

	// Ecriture du header
	fprintf(f, "P6\n%d %d\n255\n", width, height);

	// Ecriture des datas
	for(int y = 0; y < height; y++)
		fwrite(&data[y * width * 3], 1, width * 3, f);

	// Fermeture du fichier
	fclose(f);
}

void ReverseImage(unsigned char* data, int width, int height) {
	unsigned char temp_r = 0x00, temp_g = 0x00, temp_b = 0x00;

	for(int y = 0; y < height / 2; y++) {
		for(int x = 0; x < width; x++) {
			int cy = height - 1 - y;

			temp_r = data[(x + y * width) * 3 + 0];
			temp_g = data[(x + y * width) * 3 + 1];
			temp_b = data[(x + y * width) * 3 + 2];

			data[(x + y * width) * 3 + 0] = data[(x + cy * width) * 3 + 0];
			data[(x + y * width) * 3 + 1] = data[(x + cy * width) * 3 + 0];
			data[(x + y * width) * 3 + 2] = data[(x + cy * width) * 3 + 0];

			data[(x + cy * width) * 3 + 0] = temp_r;
			data[(x + cy * width) * 3 + 1] = temp_g;
			data[(x + cy * width) * 3 + 2] = temp_b;
		}
	}
}

#endif // defined IMAGE_FILTREE_MASQUEE_DEBUG
