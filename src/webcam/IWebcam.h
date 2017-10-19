// IWebcam.h - interface d'une Webcam

#ifndef IWEBCAM_H
#define IWEBCAM_H

class IWebcam {
public:
	enum WebcamType {
		WEBCAM_UVC,
		WEBCAM_V4L,
		WEBCAM_TCPIP,
		WEBCAM_IPC,
		WEBCAM_DUMMY,
		WEBCAM_TCPIP_MJPG,
		WEBCAM_UVC_MJPG,
		WEBCAM_TGA,
		WEBCAM_KINECT
	};

	IWebcam() {}
	virtual ~IWebcam() {}

	virtual bool Init(const char* device) = 0; // device peut correspondre à un fichier genre "/dev/video0"
	// pour WebcamV4L, ou "../simulateur/simu2008/simulateur2008" pour WebcamSimuIPC,
	// ou encore 4242 (casté en const char*) pour WebcamSimuTCPIP
	virtual void Close() = 0;
	virtual bool WaitForImageSync() = 0; // Fonction qui attend qu'une image soit arrivée depuis la webcam
	virtual bool QueryImage() = 0;       // Fonction qui demande à la webcam de capturer une nouvelle image

	virtual unsigned char* GetData() = 0;
	virtual const unsigned char* GetData() const = 0;
	virtual int GetWidth() const = 0;
	virtual int GetHeight() const = 0;

	// Depth data
	virtual unsigned short* GetDepthData() {
		static unsigned short fakeData = 0;
		return &fakeData;
	}
	virtual const unsigned short* GetDepthData() const {
		static unsigned short fakeData = 0;
		return &fakeData;
	}
	virtual int GetDepthWidth() const {
		return 1;
	}
	virtual int GetDepthHeight() const {
		return 1;
	}

	// Fonction qui passe au mode image inversée ou normale
	virtual void SetReversedImageMode(bool reversed = true) = 0;
	virtual bool IsReversedImageModeSet() const = 0;

	virtual WebcamType GetType() const = 0;
};

#endif // IWEBCAM_H
