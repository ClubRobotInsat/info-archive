// WebcamUVCMJPG.cpp

#include "WebcamUVCMJPG.h"
#include <iostream>

using namespace std;

#ifndef WIN32

//////////////////////////////////// Constantes ///////////////////////////////////////

// Nombre de FPS d'une vidéo lorsque l'on utilise la possibilité d'enregistrer un .avi,
// ce que l'on ne fait pas ici.
#define FPS 15.0f

// Format utilisé ; j'ai eu des problèmes avec MJPEG comme avec YUYV, ça ne marchait pas à tous les coups,
// des fois du noir et blanc...
#define FORMAT V4L2_PIX_FMT_MJPEG
//#define FORMAT V4L2_PIX_FMT_YUYV

#define GRAB_METHOD \
	1 // Peut valoir 0 ou 1 ; apparemment, avec 0, c'est du "streaming"
	  // (V4L2_CAP_STREAMING) tandis qu'avec 0 c'est du "read I/O"
	  // (V4L2_CAP_READWRITE)...

#define AVI_FILE_NAME "video1.avi" // Inutilisé ^^

/////////////////////////////////// Fonctions utilitaires //////////////////////////////

static int video_enable(struct vdIn* webcam) {
	int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	int ret;

	ret = ioctl(webcam->fd, VIDIOC_STREAMON, &type);
	if(ret < 0) {
		perror("Unable to start capture");
		return ret;
	}
	webcam->isstreaming = 1;
	return 0;
}

// met dans vd->tmpbuffer l'image compresse
// la taille est dans vd->buf.bytesused
int uvcGrabMpeg(struct vdIn* webcam) {
	int ret;

	if(!webcam->isstreaming)
		if(video_enable(webcam))
			return -1;
	memset(&webcam->buf, 0, sizeof(struct v4l2_buffer));
	webcam->buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	webcam->buf.memory = V4L2_MEMORY_MMAP;
	ret = ioctl(webcam->fd, VIDIOC_DQBUF, &webcam->buf);
	if(ret < 0) {
		perror("Unable to dequeue buffer");
		return -1;
	}

	if(webcam->buf.bytesused <= 0xaf) { /* Prevent crash on empty image */
		printf("Ignoring empty buffer ...\n");
		return 0;
	}
	memcpy(webcam->tmpbuffer, webcam->mem[webcam->buf.index], webcam->buf.bytesused);

	ret = ioctl(webcam->fd, VIDIOC_QBUF, &webcam->buf);
	if(ret < 0) {
		perror("Unable to requeue buffer");
		return -1;
	}

	return 0;
}

/////////////////////////////////////// Implémentation de WebcamUVC //////////////////////////////////

WebcamUVCMJPG::WebcamUVCMJPG() : m_width(DEFAULT_UVC_WIDTH), m_height(DEFAULT_UVC_HEIGHT), videoIn(NULL) {}

WebcamUVCMJPG::~WebcamUVCMJPG() {
	Close();
}

bool WebcamUVCMJPG::Init(const char* device) {
	cout << "ouverture de la webcam sur le fichier " << device << endl;
	videoIn = (struct vdIn*)calloc(1, sizeof(struct vdIn));
	if(init_videoIn(videoIn, device, DEFAULT_UVC_WIDTH, DEFAULT_UVC_HEIGHT, FPS, V4L2_PIX_FMT_MJPEG, 1, "video1.avi") < 0) {
		cerr << "echec de l'ouverture de la webcam " << device << " avec l'api UVC" << endl;
		return false;
	}
	if(videoIn->formatIn != V4L2_PIX_FMT_MJPEG) {
		cerr << "la webcam ne supporte pas le format MJPG" << endl;
		return false;
	}

	return true;
}

void WebcamUVCMJPG::Close() {
	if(videoIn != NULL) {
		close_v4l2(videoIn);
		free(videoIn);
		videoIn = NULL;
	}
}

bool WebcamUVCMJPG::WaitForImageSync() // Fonction qui attend qu'une image soit arrivée depuis la webcam
{
	return true;
}

bool WebcamUVCMJPG::QueryImage() // Fonction qui demande à la webcam de capturer une nouvelle image
{
	if(uvcGrabMpeg(videoIn) < 0) {
		fprintf(stderr, "Erreur : uvcGrab()\n");
		return false;
	}

	return true;
}

#endif // !defined WIN32
