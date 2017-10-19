// WebcamUVC.h - implémentation de IWebcam s'appuyant sur le driver uvcvideo (Cf la "QuickCam Team")
// Ca marche bien avec les webcams Logitech normalement ; testé avec la Logitech E3500.

#ifndef WEBCAM_UVC_H
#define WEBCAM_UVC_H

// Version Windows : c'est la webcam "dummy".
#ifdef WIN32

#include "WebcamDummy.h"
typedef WebcamDummy WebcamUVC;

#else

#include "libuvc/color.h"
#include "libuvc/v4l2uvc.h"

#include "IWebcam.h"
#include <errno.h>
#include <stdio.h>

#define DEFAULT_UVC_WIDTH 640
#define DEFAULT_UVC_HEIGHT 480

//#define DEFAULT_UVC_WIDTH 320
//#define DEFAULT_UVC_HEIGHT 240

#define DEFAULT_UVC_DEVICE "/dev/video0"

class WebcamUVC : public IWebcam {
private:
	unsigned char* m_p_data;
	int m_width, m_height;
	bool m_reversed_image_mode;

	struct vdIn* videoIn;

public:
	WebcamUVC();
	virtual ~WebcamUVC();
	virtual bool Init(const char* device = DEFAULT_UVC_DEVICE);
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
		return WEBCAM_UVC;
	}

private:
	// Fonction qui retourne verticalement l'image pointée par m_p_data
	void ReverseData();
};

#endif // !defined WIN32

#endif // WEBCAM_UVC_H
