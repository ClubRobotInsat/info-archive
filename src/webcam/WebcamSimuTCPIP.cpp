// WebcamSimuTCPIP.cpp

#include "WebcamSimuTCPIP.h"
#include "../commun/Commun.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int WebcamSimuTCPIP::DEFAULT_PORT = 4242;
const char* WebcamSimuTCPIP::DEFAULT_DEVICE = "127.0.0.1:4242";

WebcamSimuTCPIP::WebcamSimuTCPIP()
        : m_p_data(NULL), m_width(0), m_height(0), m_reversed_image_mode(false), nb_gets_sent(0) {}

WebcamSimuTCPIP::~WebcamSimuTCPIP() {
	Close();
}

bool WebcamSimuTCPIP::Init(const char* device) {
	nb_gets_sent = 0;

	// Connexion au serveur et récupération de la taille de l'image
	char str[64] = "";
	int ip_server[4];
	int port = 0;

	socket = std::make_unique<Socket>(SockProtocol::TCP);

	// Récupération de l'adresse IP et du port à utiliser
	sscanf(device, "%d.%d.%d.%d:%d", &ip_server[0], &ip_server[1], &ip_server[2], &ip_server[3], &port);

	// Connexion au serveur (simu)
	sprintf(str, "%d.%d.%d.%d", ip_server[0], ip_server[1], ip_server[2], ip_server[3]);
	printf("WebcamSimuTCPIP : connexion au serveur %s, port %d\n", str, port);
	if(!socket->connect(str, port)) {
		fprintf(stderr, "WebcamSimuTCPIP : impossible de se connecter !\n");
		return false;
	}

	// Réception de la résolution de la webcam
	printf("WebcamSimuTCPIP : Reception du message indiquant la resolution de la webcam...");
	socket->receiveMsg((void*)str, sizeof(str));
	sscanf(str, "%dx%d", &m_width, &m_height);
	printf("%dx%d\n", m_width, m_height);

	// Création d'une nouvelle image vide
	m_p_data = new unsigned char[m_width * m_height * 3];

	for(int x = 0; x < m_width; x++)
		for(int y = 0; y < 3 * m_height; y++)
			m_p_data[x + y * m_width] = 0;
	return true;
}

bool WebcamSimuTCPIP::WaitForImageSync() {
	// Réception d'une image

	// printf("WebcamSimuTCPIP : WaitForImageSync\n");
	if(nb_gets_sent == 0) {
		// printf("Aucun GET n'a ete envoye : on passe\n");
	} else {
		// printf("Reception d'une image\n");
		socket->receiveMsg(m_p_data, m_width * m_height * 3 * sizeof(unsigned char));
		nb_gets_sent--;
	}

	// Si besoin est, on retourne l'image
	if(m_reversed_image_mode)
		ReverseData();

	// Récupération d'une image
	return true;
}

bool WebcamSimuTCPIP::QueryImage() {
	// Envoi du message "GET"
	// printf("WebcamSimuTCPIP : QueryImage : envoi d'un GET\n");
	socket->sendMsg("GET", strlen("GET") + 1);
	nb_gets_sent++;
	return true;
}

void WebcamSimuTCPIP::Close() {
	socket->sendMsg("END", strlen("END") + 1);

	delete[] m_p_data;
	m_p_data = NULL;

	m_width = m_height = 0;

	socket.reset();
}

// Fonction qui passe au mode image inversée ou normale
void WebcamSimuTCPIP::SetReversedImageMode(bool reversed) {
	m_reversed_image_mode = reversed;
}

// Fonction qui retourne verticalement l'image pointée par m_p_data
void WebcamSimuTCPIP::ReverseData() {
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
