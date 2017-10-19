// WebcamKinect.h - implémentation de IWebcam qui renvoie une image provenant d'une Kinect.

#ifndef WEBCAM_KINECT_H
#define WEBCAM_KINECT_H

#include "IWebcam.h"
#include "libfreenect.h"
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>

class WebcamKinect : public IWebcam {
private:
	unsigned char* m_p_data;
	int m_width, m_height;

	unsigned short* m_p_depth_data;
	int m_depth_width, m_depth_height;

	bool m_reversed_image_mode;

	freenect_context* f_ctx;
	freenect_device* f_dev;
	bool die;
	bool got_rgb;
	bool got_depth;
	// back: owned by libfreenect
	// mid: owned by callbacks, "latest frame ready"
	// front: to draw
	uint8_t *rgb_back, *rgb_mid, *rgb_front;
	// Pour le RGB callback
	pthread_mutex_t backbuf_mutex;
	pthread_cond_t frame_cond;
	// Thread tournant derrière
	pthread_t m_thread;

public:
	WebcamKinect();
	virtual ~WebcamKinect();

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

	virtual unsigned short* GetDepthData() {
		return m_p_depth_data;
	}
	virtual const unsigned short* GetDepthData() const {
		return m_p_depth_data;
	}
	virtual int GetDepthWidth() const {
		return m_depth_width;
	}
	virtual int GetDepthHeight() const {
		return m_depth_height;
	}

	virtual void SetReversedImageMode(bool reversed) {
		m_reversed_image_mode = reversed;
	}
	virtual bool IsReversedImageModeSet() const {
		return m_reversed_image_mode;
	}

	virtual WebcamType GetType() const {
		return WEBCAM_KINECT;
	}

	// Callback pour avoir la vidéo
	void VideoCallback(void* _rgb, uint32_t timestamp);
	void DepthCallback(void* _depth, uint32_t timestamp);

	void FreenectUpdateThread();

	static void* FreenectUpdateThreadWrapper(void* user_data) {
		WebcamKinect* kinect = static_cast<WebcamKinect*>(user_data);
		kinect->FreenectUpdateThread();
		return NULL;
	}

private:
	static void freenect_video_callback(freenect_device* dev, void* video, uint32_t timestamp) {
		WebcamKinect* webcam = static_cast<WebcamKinect*>(freenect_get_user(dev));
		webcam->VideoCallback(video, timestamp);
	}

	static void freenect_depth_callback(freenect_device* dev, void* depth, uint32_t timestamp) {
		WebcamKinect* webcam = static_cast<WebcamKinect*>(freenect_get_user(dev));
		webcam->DepthCallback(depth, timestamp);
	}
};

#endif // WEBCAM_KINECT_H
