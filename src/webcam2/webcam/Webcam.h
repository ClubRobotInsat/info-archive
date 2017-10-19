// Webcam.h

#ifndef WEBCAM_H
#define WEBCAM_H

class Webcam {
public:
	enum Type { WEBCAM_NULL, WEBCAM_V4L2, WEBCAM_V4L, WEBCAM_TCPIP, WEBCAM_TGA };

	Webcam() {}
	virtual ~Webcam() {}

	virtual bool Init(const void* initParams) = 0;
	virtual void Close() = 0;

	virtual bool WaitForImageSync() = 0; // Attend qu'une image soit arrivée depuis la webcam
	virtual bool QueryImage() = 0;       // Demande à la webcam de capturer une nouvelle image

	virtual unsigned char* GetDataRGB() = 0;
	virtual const unsigned char* GetDataRGB() const = 0;
	virtual int GetWidth() const = 0;
	virtual int GetHeight() const = 0;

	virtual Type GetType() const = 0;
};

#endif // WEBCAM_H
