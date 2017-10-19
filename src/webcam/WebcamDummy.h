// WebcamDummy.h - implémentation  "fake" de IWebcam qui ne fait que renvoyer une image bidon

#ifndef WEBCAM_DUMMY_H
#define WEBCAM_DUMMY_H

#include "IWebcam.h"
#include <string.h>

//#define DEFAULT_DUMMY_WIDTH 176
//#define DEFAULT_DUMMY_HEIGHT 144
#define DEFAULT_DUMMY_WIDTH 320
#define DEFAULT_DUMMY_HEIGHT 240
#define DEFAULT_DUMMY_DEPTH 3

class WebcamDummy : public IWebcam {
private:
	unsigned char* m_p_data;
	bool m_reversed_image_mode;

private:
	void DrawQuad(int x, int y, int w, int h, unsigned char r, unsigned char g, unsigned char b) {
		for(int px = x; px < x + w; px++) {
			for(int py = y; py < y + h; py++) {
				int i = (px + py * DEFAULT_DUMMY_WIDTH) * 3;
				m_p_data[i + 0] = r;
				m_p_data[i + 1] = g;
				m_p_data[i + 2] = b;
			}
		}
	}

public:
	WebcamDummy() {
		m_p_data = new unsigned char[DEFAULT_DUMMY_WIDTH * DEFAULT_DUMMY_HEIGHT * DEFAULT_DUMMY_DEPTH];
		memset(m_p_data, 0, DEFAULT_DUMMY_WIDTH * DEFAULT_DUMMY_HEIGHT * DEFAULT_DUMMY_DEPTH);
	}

	virtual ~WebcamDummy() {
		delete[] m_p_data;
		m_p_data = NULL;
	}

	virtual bool Init(const char* device = "") {
		return true;
	}
	virtual void Close() {}
	virtual bool WaitForImageSync() {
		// Fond noir :
		memset(m_p_data, 0, DEFAULT_DUMMY_WIDTH * DEFAULT_DUMMY_HEIGHT * DEFAULT_DUMMY_DEPTH);

		// Dessin bidon :
		// - carré rouge :
		DrawQuad(30, 30, 40, 40, 0x7F, 0x00, 0x00);

		// - carré vert :
		DrawQuad(90, 70, 40, 40, 0x00, 0x7F, 0x00);

		// - carré bleu :
		DrawQuad(200, 90, 40, 40, 0x00, 0x00, 0x7F);
		return true;
	}
	virtual bool QueryImage() {
		return true;
	}

	virtual unsigned char* GetData() {
		return m_p_data;
	}
	virtual const unsigned char* GetData() const {
		return m_p_data;
	}
	virtual int GetWidth() const {
		return DEFAULT_DUMMY_WIDTH;
	}
	virtual int GetHeight() const {
		return DEFAULT_DUMMY_HEIGHT;
	}

	virtual void SetReversedImageMode(bool reversed) {
		m_reversed_image_mode = reversed;
	}
	virtual bool IsReversedImageModeSet() const {
		return m_reversed_image_mode;
	}

	virtual WebcamType GetType() const {
		return WEBCAM_DUMMY;
	}
};

#endif // WEBCAM_DUMMY_H
