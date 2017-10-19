// WebcamUVCMJPG.h

#ifndef WEBCAMUVCMJPG_H_
#define WEBCAMUVCMJPG_H_

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

#define DEFAULT_UVC_DEVICE "/dev/video0"

class WebcamUVCMJPG : public IWebcam {
private:
	int m_width, m_height;
	struct vdIn* videoIn;

public:
	WebcamUVCMJPG();
	virtual ~WebcamUVCMJPG();
	virtual bool Init(const char* device = DEFAULT_UVC_DEVICE);
	virtual void Close();
	virtual bool WaitForImageSync(); // Fonction qui attend qu'une image soit arrivée depuis la webcam
	virtual bool QueryImage();       // Fonction qui demande à la webcam de capturer une nouvelle image

	// attention l'image obtenue par GetData est compressee en MJPEG
	virtual unsigned char* GetData() {
		return videoIn->tmpbuffer;
	}
	virtual const unsigned char* GetData() const {
		return videoIn->tmpbuffer;
	}
	virtual int GetDataSize() const {
		return videoIn->buf.bytesused;
	}
	virtual int GetWidth() const {
		return m_width;
	}
	virtual int GetHeight() const {
		return m_height;
	}

	// Fonctions permettant d'utiliser un mode qui inverse l'image
	virtual void SetReversedImageMode(bool reversed) {
		fprintf(stderr, "Erreur : WebcamUVCMJPG::SetReversedImageMode non implemente\n");
	}
	virtual bool IsReversedImageModeSet() const {
		return false;
	}

	virtual WebcamType GetType() const {
		return WEBCAM_UVC_MJPG;
	}

private:
	// Fonction qui retourne verticalement l'image pointée par m_p_data
	void ReverseData();
};

#endif // WIN32
#endif /* WEBCAMUVCMJPG_H_ */
