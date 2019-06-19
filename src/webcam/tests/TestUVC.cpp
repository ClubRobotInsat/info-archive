// TestUVC.cpp - test_uvc : programme qui prend une photo avec la webcam, en utilisant des bouts
// de code source issus de l'exécutable "luvcview", qui provient du projet de driver
// webcam universel "uvcvideo" (qui a l'air bien orienté Logitech quand même...).

#include "color.h"
#include "v4l2uvc.h"
#include <stdio.h>
#include <stdlib.h>

#define WIDTH 640
#define HEIGHT 480
#define DEVICE "/dev/video1"
#define FPS \
	30.0f // Je crois que c'est le nombre de FPS d'une vidéo lorsque l'on utilise
	      // la possibilité d'enregistrer un .avi...

//#define FORMAT V4L2_PIX_FMT_MJPEG
#define FORMAT V4L2_PIX_FMT_YUYV // J'ai eu des problèmes avec MJPEG, ça ne marchait pas à tous les coups...

#define GRAB_METHOD \
	1 // Peut valoir 0 ou 1 ; apparemment, avec 0, c'est du "streaming"
	  // (V4L2_CAP_STREAMING) tandis qu'avec 0 c'est du "read I/O"
	  // (V4L2_CAP_READWRITE)...

#define AVI_FILE_NAME "video1.avi" // Inutilisé ^^

void SavePPM(const char* filename, unsigned char* data, int width, int height);

void YUY2ToRGB24(unsigned char* data_in, unsigned char* data_out, int width, int height);

int main(int argc, char* argv[]) {
	struct vdIn* videoIn = (struct vdIn*)calloc(1, sizeof(struct vdIn));
	unsigned char rgb_buffer[WIDTH * HEIGHT * 3];

	// Initialisation
	if(init_videoIn(videoIn, DEVICE, WIDTH, HEIGHT, FPS, FORMAT, GRAB_METHOD, AVI_FILE_NAME) < 0)
		return EXIT_FAILURE;
	initLut();

	if(uvcGrab(videoIn) < 0) {
		printf("Error grabbing\n");
		return EXIT_FAILURE;
	}

	YUY2ToRGB24(videoIn->framebuffer, rgb_buffer, WIDTH, HEIGHT);

	SavePPM("photo.ppm", rgb_buffer, WIDTH, HEIGHT);

	close_v4l2(videoIn);
	free(videoIn);
	freeLut();
	printf("Cleanup done. Exiting ...\n");

	return 0;
}

// Restreindre une valeur à un sol et un plafond
double clamp(double value, double min, double max) {
	if(value < min)
		value = min;
	if(value > max)
		value = max;
	return value;
}

void YUVToRGB(unsigned char y, unsigned char u, unsigned char v, unsigned char* pr, unsigned char* pg, unsigned char* pb) {
	// Coeffs tirés de http://people.via.ecp.fr/~remi/ecp/tpi/rapport/yuv.html :
	*pr = (unsigned char)(clamp(double(y) - 0.0009267 * (double(u) - 128.0) + 1.4016868 * (double(v) - 128.0), 0.0, 255.0));
	*pg = (unsigned char)(clamp(double(y) - 0.3436954 * (double(u) - 128.0) - 0.7141690 * (double(v) - 128.0), 0.0, 255.0));
	*pb = (unsigned char)(clamp(double(y) + 1.7721604 * (double(u) - 128.0) + 0.0009902 * (double(v) - 128.0), 0.0, 255.0));
}

// Doc du format YUY2 : http://www.fourcc.org/yuv.php#YUY2
void YUY2ToRGB24(unsigned char* data_in, unsigned char* data_out, int width, int height) {
	int size = width * height;
	unsigned char y = 0, u = 0, v = 0;
	unsigned char r = 0, g = 0, b = 0;
	int j = 0;

	// On parcourt 4 valeurs par 4 valeurs, ce qui correspond à 2 pixels par 2 pixels
	for(int i = 0; i < size * 2; i += 4) {
		// Pixel 1 :
		y = data_in[i];
		u = data_in[i + 1];
		v = data_in[i + 3];
		YUVToRGB(y, u, v, &r, &g, &b);

		data_out[j++] = r;
		data_out[j++] = g;
		data_out[j++] = b;

		// Pixel 2 :
		y = data_in[i + 2];
		YUVToRGB(y, u, v, &r, &g, &b);

		data_out[j++] = r;
		data_out[j++] = g;
		data_out[j++] = b;
	}
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
	for(int y = 0; y < height; y++) {
		size_t dummy = fwrite(&data[y * width * 3], 1, width * 3, f);
	}

	// Fermeture du fichier
	fclose(f);
}
