// WebcamKinect.cpp

#include "WebcamKinect.h"
#include "Color.h"
#include "assert.h"
#include "log/Log.h"

// DEFINES
//#define VIDEO_RESOLUTION      FREENECT_RESOLUTION_LOW   /**< QVGA - 320x240 */
#define VIDEO_RESOLUTION FREENECT_RESOLUTION_MEDIUM /**< VGA  - 640x480 */
//#define VIDEO_RESOLUTION      FREENECT_RESOLUTION_HIGH  /**< SXGA - 1280x1024 */

#define VIDEO_FORMAT FREENECT_VIDEO_RGB

//#define DEPTH_RESOLUTION      FREENECT_RESOLUTION_LOW   /**< QVGA - 320x240 */
#define DEPTH_RESOLUTION FREENECT_RESOLUTION_MEDIUM /**< VGA  - 640x480 */
//#define DEPTH_RESOLUTION      FREENECT_RESOLUTION_HIGH  /**< SXGA - 1280x1024 */

#define DEPTH_FORMAT FREENECT_DEPTH_11BIT /**< 11 bit depth information in one uint16_t/pixel */
//#define DEPTH_FORMAT 	FREENECT_DEPTH_10BIT         /**< 10 bit depth information in one uint16_t/pixel */
//#define DEPTH_FORMAT 	FREENECT_DEPTH_11BIT_PACKED  /**< 11 bit packed depth information */
//#define DEPTH_FORMAT 	FREENECT_DEPTH_10BIT_PACKED  /**< 10 bit packed depth information */
//#define DEPTH_FORMAT 	FREENECT_DEPTH_REGISTERED    /**< processed depth data in mm, aligned to 640x480 RGB */
//#define DEPTH_FORMAT 	FREENECT_DEPTH_MM            /**< depth to each pixel in mm, but left unaligned to RGB image */


WebcamKinect::WebcamKinect()
        : m_p_data(NULL)
        , m_width(0)
        , m_height(-1)
        , m_p_depth_data(NULL)
        , m_depth_width(-1)
        , m_depth_height(-1)
        , m_reversed_image_mode(false)
        , f_ctx(NULL)
        , f_dev(NULL)
        , die(false)
        , got_rgb(false)
        , got_depth(false)
        , rgb_back(NULL)
        , rgb_mid(NULL)
        , rgb_front(NULL) {
	pthread_mutex_init(&backbuf_mutex, NULL);
	pthread_cond_init(&frame_cond, NULL);
}

WebcamKinect::~WebcamKinect() {
	Close();
}

void WebcamKinect::Close() {
	if(f_dev != NULL) {
		die = true;

		logDebug0("Vidéo stoppée");
		freenect_stop_video(f_dev);
		logDebug0("Connection à la Kinect terminée");
		freenect_close_device(f_dev);
		freenect_shutdown(f_ctx);
		pthread_join(m_thread, NULL);
		delete[] m_p_data;
		delete[] m_p_depth_data;
		delete[] rgb_back;
		delete[] rgb_mid;
		delete[] rgb_front;
		m_p_data = NULL;
		m_p_depth_data = NULL;
		rgb_back = NULL;
		rgb_mid = NULL;
		rgb_front = NULL;
		f_dev = NULL;
		f_ctx = NULL;
		logDebug0("Kinect éteinte!");
	}
}

bool WebcamKinect::Init(const char* device) {
	if(freenect_init(&f_ctx, NULL) < 0) {
		logError("freenect_init() failed");
		Close();
		return false;
	}

	freenect_set_log_level(f_ctx, FREENECT_LOG_INFO);
	freenect_select_subdevices(f_ctx, (freenect_device_flags)(FREENECT_DEVICE_CAMERA));

	int nr_devices = freenect_num_devices(f_ctx);
	logDebug0("Number of devices found -> ", nr_devices);

	if(nr_devices < 1) {
		logError("No device found, closing...");
		Close();
		return false;
	}

	int user_device_number = 0;
	if(freenect_open_device(f_ctx, &f_dev, user_device_number) < 0) {
		logError("Could not open device");
		Close();
		return false;
	}

	freenect_set_user(f_dev, this);

	// ----------- Init couleur ----------
	freenect_frame_mode video_frame_mode = freenect_find_video_mode(VIDEO_RESOLUTION, VIDEO_FORMAT);
	freenect_set_video_mode(f_dev, video_frame_mode);

	m_width = video_frame_mode.width;
	m_height = video_frame_mode.height;
	logInfo("Color dimensions: ", m_width, "x", m_height);

	rgb_back = new uint8_t[m_width * m_height * 3];
	rgb_mid = new uint8_t[m_width * m_height * 3];
	rgb_front = new uint8_t[m_width * m_height * 3];
	m_p_data = new unsigned char[m_width * m_height * 3];
	memset(rgb_back, 0, m_width * m_height * 3);
	memset(rgb_mid, 0, m_width * m_height * 3);
	memset(rgb_front, 0, m_width * m_height * 3);
	memset(m_p_data, 0, m_width * m_height * 3);

	freenect_set_video_buffer(f_dev, rgb_back);
	freenect_set_video_callback(f_dev, freenect_video_callback);
	if(freenect_start_video(f_dev) < 0) {
		logError("Impossible de démarrer la video!");
		Close();
	}
	logDebug0("Video démarrée!");

	// ----------- Init depth ----------
	freenect_frame_mode depth_frame_mode = freenect_find_depth_mode(DEPTH_RESOLUTION, DEPTH_FORMAT);
	m_depth_width = depth_frame_mode.width;
	m_depth_height = depth_frame_mode.height;
	logInfo("Depth dimensions: ", m_depth_width, m_depth_height);
	// TODO !!!

	if(pthread_create(&m_thread, NULL, &FreenectUpdateThreadWrapper, (void*)this) != 0)
		logError("Cannot initialize freenect thread");
	return true;
}

void WebcamKinect::VideoCallback(void* rgb, unsigned int timestamp) {
	pthread_mutex_lock(&backbuf_mutex);

	// swap buffers
	assert(rgb_back == rgb);
	rgb_back = rgb_mid;
	freenect_set_video_buffer(f_dev, rgb_back);
	rgb_mid = (uint8_t*)rgb;

	got_rgb = true;
	pthread_cond_signal(&frame_cond);
	pthread_mutex_unlock(&backbuf_mutex);
}

void WebcamKinect::DepthCallback(void* _depth, uint32_t timestamp) {
	// TODO!!
	while(!die) {
		if(freenect_process_events(f_ctx) < 0)
			printf("Cannot process freenect events\n");
	}
}

void WebcamKinect::FreenectUpdateThread() {
	while(!die) {
		if(freenect_process_events(f_ctx) < 0)
			printf("Cannot process freenect events\n");
	}
}

// Runs the thread to actualize the freenect state !
// Do not call directly, thread runs here
bool WebcamKinect::WaitForImageSync() {
	while(!got_rgb) {
		pthread_cond_wait(&frame_cond, &backbuf_mutex);
	}
	uint8_t* tmp;
	if(got_rgb) {
		tmp = rgb_front;
		rgb_front = rgb_mid;
		rgb_mid = tmp;
		got_rgb = false;
	}

	/// ON A LES DATA EN RGB
	// On copie les data dans m_p_data
	for(int i = 0; i < m_height; i++) {
		for(int j = 0; j < m_width; j++) {
			Color pixel(rgb_front[3 * (i * m_width + j) + 0],
			            rgb_front[3 * (i * m_width + j) + 1],
			            rgb_front[3 * (i * m_width + j) + 2]);
			m_p_data[3 * (i * m_width + j) + 0] = pixel.r;
			m_p_data[3 * (i * m_width + j) + 1] = pixel.g;
			m_p_data[3 * (i * m_width + j) + 2] = pixel.b;
		}
	}
	return true;
}

bool WebcamKinect::QueryImage() {
	return true;
}
