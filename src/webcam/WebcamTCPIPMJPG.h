// WebcamTCPIPMJPG.h

#ifndef WEBCAMTCPIPMJPG_H_
#define WEBCAMTCPIPMJPG_H_

// Version Windows : c'est la webcam "dummy".
#ifdef WIN32

#include "WebcamDummy.h"
typedef WebcamDummy WebcamTCPIPMJPG;

#else

#include "IWebcam.h"
#include "Socket.h"
#include <errno.h>
#include <memory>
#include <stdio.h>

class WebcamTCPIPMJPG : public IWebcam {
private:
	static const int DEFAULT_PORT;
	static const char* DEFAULT_DEVICE;

	unsigned char *m_p_data, *m_p_mjpg, *m_p_yuy2;
	int m_width, m_height;

	bool m_reversed_image_mode;

	int nb_gets_sent; // Variable permettant de compter le nombre de requêtes GET envoyées dont on n'a
	                  // toujours pas demandé/reçu l'image correspondante

	std::unique_ptr<Socket> socket;

public:
	WebcamTCPIPMJPG();
	virtual ~WebcamTCPIPMJPG();

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

	// Fonctions permettant d'utiliser un mode qui inverse l'image
	virtual void SetReversedImageMode(bool reversed) {
		m_reversed_image_mode = reversed;
	}
	virtual bool IsReversedImageModeSet() const {
		return m_reversed_image_mode;
	}

	virtual WebcamType GetType() const {
		return WEBCAM_TCPIP_MJPG;
	}

private:
	// Fonction qui retourne verticalement l'image pointée par m_p_data
	void ReverseData();
};

#endif // WIN32

#endif /* WEBCAMTCPIPMJPG_H_ */
