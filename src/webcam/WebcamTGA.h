// WebcamTGA.h - implémentation de IWebcam qui renvoie une image fixe à partir d'un fichier TGA.

#ifndef WEBCAM_TGA_H
#define WEBCAM_TGA_H

#include "IWebcam.h"
#include "TGALoader.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>

#define DEFAULT_TGA_DEPTH 3

class WebcamTGA : public IWebcam {
private:
	TGALoader tga;
	unsigned char* m_p_data;
	int m_width, m_height;
	bool m_reversed_image_mode;

public:
	WebcamTGA();
	virtual ~WebcamTGA();

	virtual bool Init(const char* device);
	virtual void Close();
	virtual bool WaitForImageSync();
	virtual bool QueryImage();

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

	virtual void SetReversedImageMode(bool reversed) {
		m_reversed_image_mode = reversed;
	}
	virtual bool IsReversedImageModeSet() const {
		return m_reversed_image_mode;
	}

	virtual WebcamType GetType() const {
		return WEBCAM_TGA;
	}
};

#endif // WEBCAM_TGA_H
