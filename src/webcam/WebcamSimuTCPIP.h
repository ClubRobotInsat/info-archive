// WebcamSimuTCPIP.h - implémentation de IWebcam s'appuyant sur TCP/IP, communiquant avec le simulateur

#ifndef WEBCAM_SIMU_TCPIP_H
#define WEBCAM_SIMU_TCPIP_H

#include "IWebcam.h"
#include "Socket.h"
#include <errno.h>
#include <memory>
#include <stdio.h>

class WebcamSimuTCPIP : public IWebcam {
private:
	static const int DEFAULT_PORT;
	static const char* DEFAULT_DEVICE;

	unsigned char* m_p_data;
	int m_width, m_height;
	bool m_reversed_image_mode;

	int nb_gets_sent; // Variable permettant de compter le nombre de requêtes GET envoyées dont on n'a
	                  // toujours pas demandé/reçu l'image correspondante

	std::unique_ptr<Socket> socket;

public:
	WebcamSimuTCPIP();
	virtual ~WebcamSimuTCPIP();
	virtual bool Init(const char* device = DEFAULT_DEVICE);
	virtual void Close();
	virtual bool WaitForImageSync(); // Fonction qui attend qu'une image soit arrivée depuis la webcam
	virtual bool QueryImage();       // Fonction qui demande à la webcam de capturer une nouvelle image

	virtual unsigned char* GetData() {
		return m_p_data;
	}
	virtual const unsigned char* GetData() const {
		return m_p_data;
	}
	virtual int GetWidth() const {
		return m_width;
	}
	virtual int GetHeight() const {
		return m_height;
	}

	virtual void SetReversedImageMode(bool reversed); // Fonction qui passe au mode image inversée ou normale
	virtual bool IsReversedImageModeSet() const {
		return m_reversed_image_mode;
	}

	virtual WebcamType GetType() const {
		return WEBCAM_TCPIP;
	}

protected:
	// Fonction qui retourne verticalement l'image pointée par m_p_data
	void ReverseData();
};

#endif // WEBCAM_SIMU_TCPIP_H
