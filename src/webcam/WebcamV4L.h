// WebcamV4L.h - implémentation de IWebcam s'appuyant sur Video4Linux

#ifndef WEBCAM_V4L_H
#define WEBCAM_V4L_H

// Version Windows : c'est la webcam "dummy".
#ifdef WIN32

#include "WebcamDummy.h"
typedef WebcamDummy WebcamV4L;

#else

#include "IWebcam.h"

#include <asm/types.h>
#include <linux/version.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#if LINUX_VERSION_CODE > KERNEL_VERSION(3, 0, 0)
#include <libv4l1.h>
#else
#include <linux/videodev.h>
#endif
#include <asm/types.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

//#define DEFAULT_V4L_WIDTH 176
//#define DEFAULT_V4L_HEIGHT 144

//#define DEFAULT_V4L_WIDTH 320
//#define DEFAULT_V4L_HEIGHT 240

//#define DEFAULT_V4L_WIDTH 352
//#define DEFAULT_V4L_HEIGHT 288

#define DEFAULT_V4L_WIDTH 640
#define DEFAULT_V4L_HEIGHT 480

#define DEFAULT_V4L_DEPTH 3
#define DEFAULT_V4L_DEVICE "/dev/video0"

// Valeurs de 0 à 65536
#define DEFAULT_BRIGHTNESS 27136
#define DEFAULT_HUE 0
#define DEFAULT_CONTRAST 11264
#define DEFAULT_COLOUR 0
#define DEFAULT_WHITENESS 0
#define DEFAULT_DEPTH 24
//#define DEFAULT_PALETTE		VIDEO_PALETTE_RGB24
#define DEFAULT_PALETTE VIDEO_PALETTE_YUV420P

class WebcamV4L : public IWebcam {
private:
	unsigned char *m_p_data, *m_grab_data;
	int m_width, m_height, m_depth;
	unsigned int m_fd, m_frame; // m_fd : file descriptor; m_frame : numéro de la frame dans notre buffer
	                            // MMap actuelle (oui c'est pas très clair...).
	bool m_reversed_image_mode;

	struct video_picture m_video_picture; // informations sur la luminosité, la teinte...etc
	struct video_capability m_video_capability;
	struct video_mmap m_video_mmap;
	struct video_mbuf m_video_mbuf;

public:
	WebcamV4L();
	virtual ~WebcamV4L();
	virtual bool Init(const char* device = DEFAULT_V4L_DEVICE);
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
		return WEBCAM_V4L;
	}

protected:
	// Fonction qui retourne verticalement l'image pointée par m_p_data
	void ReverseData();

	// Fonction qui prend en entrée un pointeur vers une image en YUV420P et qui met à jour m_p_data,
	// qui pointe alors vers une image correcte en RGB24.
	void YUV420PToRGB24(unsigned char* data);

	// Changement d'espace colorimétrique
	void YUVToRGB(unsigned char y, unsigned char u, unsigned char v, unsigned char* pr, unsigned char* pg, unsigned char* pb);

	// Restreindre une valeur à un sol et un plafond
	static double clamp(double value, double min, double max);
};

#endif // !defined WIN32

#endif // WEBCAM_V4L_H
