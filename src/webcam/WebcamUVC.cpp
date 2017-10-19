// WebcamUVC.cpp

#ifndef WIN32

#include "WebcamUVC.h"
#include <stdlib.h>

//////////////////////////////////// Constantes ///////////////////////////////////////

// Nombre de FPS d'une vidéo lorsque l'on utilise la possibilité d'enregistrer un .avi,
// ce que l'on ne fait pas ici.
#define FPS 15.0f

// Format utilisé ; j'ai eu des problèmes avec MJPEG comme avec YUYV, ça ne marchait pas à tous les coups,
// des fois du noir et blanc...
#define FORMAT V4L2_PIX_FMT_MJPEG
//#define FORMAT V4L2_PIX_FMT_YUYV

#define GRAB_METHOD 1 // Peut valoir 0 ou 1 ; apparemment, avec 0, c'est du "streaming"
                      // (V4L2_CAP_STREAMING) tandis qu'avec 0 c'est du "read I/O"
                      // (V4L2_CAP_READWRITE)...

#define AVI_FILE_NAME "video1.avi" // Inutilisé ^^


/////////////////////////////////// Fonctions utilitaires //////////////////////////////

// Restreindre une valeur à un sol et un plafond
static inline double clamp(double value, double min, double max) {
	if(value < min)
		value = min;
	if(value > max)
		value = max;
	return value;
}

// Conversion YUV -> RGB
static inline void
    YUVToRGB(unsigned char y, unsigned char u, unsigned char v, unsigned char* pr, unsigned char* pg, unsigned char* pb) {
	// Coeffs tirés de http://people.via.ecp.fr/~remi/ecp/tpi/rapport/yuv.html :
	*pr = (unsigned char)(clamp(double(y) - 0.0009267 * (double(u) - 128.0) + 1.4016868 * (double(v) - 128.0), 0.0, 255.0));
	*pg = (unsigned char)(clamp(double(y) - 0.3436954 * (double(u) - 128.0) - 0.7141690 * (double(v) - 128.0), 0.0, 255.0));
	*pb = (unsigned char)(clamp(double(y) + 1.7721604 * (double(u) - 128.0) + 0.0009902 * (double(v) - 128.0), 0.0, 255.0));
}

// Conversion d'une image de YUY2 vers RGB24.
// Doc du format YUY2 : http://www.fourcc.org/yuv.php#YUY2
static void YUY2ToRGB24(unsigned char* data_in, unsigned char* data_out, int width, int height) {
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

/////////////////////////////////////// Implémentation de WebcamUVC //////////////////////////////////

// Constructeur
WebcamUVC::WebcamUVC()
        : m_p_data(NULL), m_width(DEFAULT_UVC_WIDTH), m_height(DEFAULT_UVC_HEIGHT), m_reversed_image_mode(false), videoIn(NULL) {}

WebcamUVC::~WebcamUVC() {
	Close();
}

bool WebcamUVC::Init(const char* device) {
	Close();

	videoIn = (struct vdIn*)calloc(1, sizeof(struct vdIn));

	if(init_videoIn(videoIn, device, DEFAULT_UVC_WIDTH, DEFAULT_UVC_HEIGHT, FPS, FORMAT, GRAB_METHOD, AVI_FILE_NAME) < 0)
		return false;

	initLut();

	m_p_data = new unsigned char[DEFAULT_UVC_WIDTH * DEFAULT_UVC_HEIGHT * 3];

	for(int x = 0; x < m_width; x++)
		for(int y = 0; y < 3 * m_height; y++)
			m_p_data[x + y * m_width] = 0;

	return true;
}

void WebcamUVC::Close() {
	if(videoIn != NULL) {
		close_v4l2(videoIn);
		free(videoIn);
		freeLut();
		delete[] m_p_data;
		m_p_data = NULL;
		videoIn = NULL;
	}
}

bool WebcamUVC::WaitForImageSync() // Fonction qui attend qu'une image soit arrivée depuis la webcam
{
	return true;
}

bool WebcamUVC::QueryImage() // Fonction qui demande à la webcam de capturer une nouvelle image
{
	if(uvcGrab(videoIn) < 0) {
		fprintf(stderr, "Erreur : uvcGrab()\n");
		return false;
	}

	YUY2ToRGB24(videoIn->framebuffer, m_p_data, DEFAULT_UVC_WIDTH, DEFAULT_UVC_HEIGHT);

	if(m_reversed_image_mode)
		ReverseData();

	return true;
}

// Fonction qui retourne verticalement l'image pointée par m_p_data
void WebcamUVC::ReverseData() {
	for(int y = 0; y < m_height / 2; y++) {
		for(int x = 0; x < m_width; x++) {
			unsigned char r_temp = 0, g_temp = 0, b_temp = 0;
			int y_comp = m_height - y - 1; // "comp" pour "complémentaire"

			r_temp = m_p_data[(x + y * m_width) * 3 + 0];
			g_temp = m_p_data[(x + y * m_width) * 3 + 1];
			b_temp = m_p_data[(x + y * m_width) * 3 + 2];

			m_p_data[(x + y * m_width) * 3 + 0] = m_p_data[(x + y_comp * m_width) * 3 + 0];
			m_p_data[(x + y * m_width) * 3 + 1] = m_p_data[(x + y_comp * m_width) * 3 + 1];
			m_p_data[(x + y * m_width) * 3 + 2] = m_p_data[(x + y_comp * m_width) * 3 + 2];

			m_p_data[(x + y_comp * m_width) * 3 + 0] = r_temp;
			m_p_data[(x + y_comp * m_width) * 3 + 1] = g_temp;
			m_p_data[(x + y_comp * m_width) * 3 + 2] = b_temp;
		}
	}
}

#endif // !defined WIN32
