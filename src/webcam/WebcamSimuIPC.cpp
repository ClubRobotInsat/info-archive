// WebcamSimuIPC.cpp - doc :
// http://linuxgazette.net/104/ramankutty.html
// http://www.labri.fr/perso/billaud/travaux/SYSRESEAU/HTML/sysreseau-15.html

#ifndef WIN32

#include "WebcamSimuIPC.h"

WebcamSimuIPC::WebcamSimuIPC()
        : m_data(NULL), m_width(0), m_height(0), m_reversed_image_mode(false), m_shmid(0), m_semaphores() {}

WebcamSimuIPC::~WebcamSimuIPC() {
	Close();
}

bool WebcamSimuIPC::Init(const char* device) {
	// Récupération du shmid, du semid et des dimensions de l'image
	int semid = 0;
	FILE* f = fopen(device, "r");
	if(!f) {
		fprintf(stderr, "Erreur : impossible d'ouvrir \"%s\"\n", device);
		return false;
	}
	fscanf(f, "%d\n%d\n%d\n%d", &m_shmid, &semid, &m_width, &m_height);
	fclose(f);

	printf("Image : largeur : %d, hauteur : %d\n", m_width, m_height);

	// Récupération de m_data
	if((m_data = (unsigned char*)(shmat(m_shmid, NULL, SHM_RDONLY))) < 0) {
		printf("Erreur dans shm attach. errno: %d\n", errno);
		return false;
	}

	// Initialisation du semaphore
	m_semaphores.setID(semid);

	return true;
}

void WebcamSimuIPC::Close() {
	if(m_data != NULL) {
		shmdt(m_data);
		m_data = NULL;
	}
}

bool WebcamSimuIPC::WaitForImageSync() {
	m_semaphores.waitForZero(0);
	ReverseData();
	return true;
}

bool WebcamSimuIPC::QueryImage() {
	// Rien.
	return true;
}

// Fonction qui passe au mode image inversée ou normale
void WebcamSimuIPC::SetReversedImageMode(bool reversed) {
	m_reversed_image_mode = reversed;
}

// Fonction qui retourne verticalement l'image pointée par m_data
void WebcamSimuIPC::ReverseData() {
	for(int y = 0; y < m_height / 2; y++) {
		for(int x = 0; x < m_width; x++) {
			unsigned char r_temp = 0, g_temp = 0, b_temp = 0;
			int y_comp = m_height - y - 1; // "comp" pour "complémentaire"

			r_temp = m_data[(x + y * m_width) * 3 + 0];
			g_temp = m_data[(x + y * m_width) * 3 + 1];
			b_temp = m_data[(x + y * m_width) * 3 + 2];

			m_data[(x + y * m_width) * 3 + 0] = m_data[(x + y_comp * m_width) * 3 + 0];
			m_data[(x + y * m_width) * 3 + 1] = m_data[(x + y_comp * m_width) * 3 + 1];
			m_data[(x + y * m_width) * 3 + 2] = m_data[(x + y_comp * m_width) * 3 + 2];

			m_data[(x + y_comp * m_width) * 3 + 0] = r_temp;
			m_data[(x + y_comp * m_width) * 3 + 1] = g_temp;
			m_data[(x + y_comp * m_width) * 3 + 2] = b_temp;
		}
	}
}

#endif // !defined WIN32
