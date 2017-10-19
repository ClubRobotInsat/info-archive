// WebcamTCPIPMJPG.cpp

#ifndef WIN32

#include "WebcamTCPIPMJPG.h"

#include "../commun/Commun.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libuvc/utils.h"

const int WebcamTCPIPMJPG::DEFAULT_PORT = 9999;
const char* WebcamTCPIPMJPG::DEFAULT_DEVICE = "127.0.0.1:9999";

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

/////////////////////////////////////// Implémentation de WebcamTCPIPMJPG //////////////////////////////////

WebcamTCPIPMJPG::WebcamTCPIPMJPG()
        : m_p_data(NULL), m_p_mjpg(NULL), m_p_yuy2(NULL), m_width(0), m_height(0), m_reversed_image_mode(false), nb_gets_sent(0) {}

WebcamTCPIPMJPG::~WebcamTCPIPMJPG() {
	Close();
}


bool WebcamTCPIPMJPG::Init(const char* device) {
	nb_gets_sent = 0;

	// Connexion au serveur et récupération de la taille de l'image
	char str[64] = "";
	int ip_server[4];
	int port = 0;

	socket = std::make_unique<Socket>(SockProtocol::TCP);

	// Récupération de l'adresse IP et du port à utiliser
	sscanf(device, "%d.%d.%d.%d:%d", &ip_server[0], &ip_server[1], &ip_server[2], &ip_server[3], &port);

	// Connexion au serveur
	sprintf(str, "%d.%d.%d.%d", ip_server[0], ip_server[1], ip_server[2], ip_server[3]);
	printf("WebcamTCPIPMJPG : connexion au serveur %s, port %d\n", str, port);
	socket->connect(str, port);

	// Réception de la résolution de la webcam
	printf("WebcamTCPIPMJPG : Reception du message indiquant la resolution de la webcam...");
	socket->receiveMsg((void*)str, sizeof(str));
	sscanf(str, "%dx%d", &m_width, &m_height);
	printf("%dx%d\n", m_width, m_height);

	// Création d'une nouvelle image vide
	m_p_data = new unsigned char[m_width * m_height * 3];
	m_p_mjpg = new unsigned char[m_width * m_height << 1];
	m_p_yuy2 = new unsigned char[m_width * (m_height + 8) * 2];

	for(int x = 0; x < m_width; x++)
		for(int y = 0; y < 3 * m_height; y++)
			m_p_data[x + y * m_width] = 0;
	return true;
}

bool WebcamTCPIPMJPG::WaitForImageSync() {
	if(nb_gets_sent != 0) {
		// Réception de l'image compresse
		socket->receiveMsg(m_p_mjpg, m_width * m_height * 3 * sizeof(unsigned char));
		nb_gets_sent--;

		// decompression de l'image
		if(jpeg_decode(&m_p_yuy2, m_p_mjpg, &m_width, &m_height) < 0) {
			printf("jpeg decode errors\n");
			return false;
		}

		// conversion de l'image dans le bon format
		YUY2ToRGB24(m_p_yuy2, m_p_data, m_width, m_height);

		if(m_reversed_image_mode)
			ReverseData();
	}

	// Récupération d'une image
	return true;
}

bool WebcamTCPIPMJPG::QueryImage() {
	// Envoi du message "GET"
	// printf("WebcamTCPIPMJPG : QueryImage : envoi d'un GET\n");
	socket->sendMsg("GET", strlen("GET") + 1);
	nb_gets_sent++;
	return true;
}

void WebcamTCPIPMJPG::Close() {
	socket->sendMsg("END", strlen("END") + 1);

	delete[] m_p_data;
	m_p_data = NULL;

	delete[] m_p_mjpg;
	m_p_mjpg = NULL;

	delete[] m_p_yuy2;
	m_p_yuy2 = NULL;

	m_width = m_height = 0;

	socket.reset();
}


// Fonction qui retourne verticalement l'image pointée par m_p_data
void WebcamTCPIPMJPG::ReverseData() {
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

#endif // WIN32
