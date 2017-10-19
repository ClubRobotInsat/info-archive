// WebcamTGA.cpp

#include "WebcamTGA.h"
#include "log/Log.h"

WebcamTGA::WebcamTGA() : tga(), m_p_data(NULL), m_width(0), m_height(0), m_reversed_image_mode(false) {}

WebcamTGA::~WebcamTGA() {
	Close();
}

bool WebcamTGA::Init(const char* device) {
	TGAErrorCode error = tga.loadFile(device);
	if(error != TGA_OK)
		return false;

	if(tga.getBpp() != 3) {
		logError("le nombre de bits par pixel de \"",
		         device,
		         "\" n'est pas de 3 :"
		         " assurez-vous d'avoir bien supprime le canal alpha de l'image.");
		tga.free();
		return false;
	}

	m_width = tga.getWidth();
	m_height = tga.getHeight();

	m_p_data = new unsigned char[m_width * m_height * 3];

	memset(m_p_data, 0, m_width * m_height * 3);

	return true;
}

void WebcamTGA::Close() {
	tga.free();
	delete[] m_p_data;
	m_p_data = NULL;
}

bool WebcamTGA::WaitForImageSync() {
	if(!tga.isLoaded())
		return false;

	unsigned char* tga_data = tga.getData();

	// La convention haut/bas étant l'inverse pour TGALoader et pour les webcams,
	// on ne retourne l'image que lorsque m_reversed_image_mode == false.
	if(m_reversed_image_mode) {
		// Copie normale :
		int size = m_width * m_height * 3;
		for(int i = 0; i < size; i++)
			m_p_data[i] = tga_data[i];
	} else {
		// On copie en inversant verticalement :
		for(int x = 0; x < m_width; x++) {
			for(int y = 0; y < m_height; y++) {
				int cy = m_height - 1 - y;
				int i = (x + y * m_width) * 3;
				int ci = (x + cy * m_width) * 3;

				m_p_data[i + 0] = tga_data[ci + 0];
				m_p_data[i + 1] = tga_data[ci + 1];
				m_p_data[i + 2] = tga_data[ci + 2];
			}
		}
	}

	return true;
}

bool WebcamTGA::QueryImage() {
	return tga.isLoaded();
}
