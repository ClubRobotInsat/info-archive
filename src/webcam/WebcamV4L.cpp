// WebcamV4L.cpp

#ifndef WIN32

// Si défini : on utilise la libv4l1, qui fait une émulation des fonctionnalités de V4L1
// à travers V4L2. Ca peut servir si la webcam ne fonctionne qu'avec V4L2...
//#define USE_LIBV4L1
// NB : maintenant, ce symbole est défini si nécessaire par SCons à la compilation.

#ifdef USE_LIBV4L1
#include <linux/version.h>
#include <stdint.h>
#if LINUX_VERSION_CODE > KERNEL_VERSION(3, 0, 0)
#include <libv4l1.h>
#else
#include <linux/videodev.h>
#endif
#endif

#include "WebcamV4L.h"
#include <string.h>

WebcamV4L::WebcamV4L()
        : m_p_data(NULL)
        , m_grab_data(NULL)
        , m_width(DEFAULT_V4L_WIDTH)
        , m_height(DEFAULT_V4L_HEIGHT)
        , m_fd(0)
        , m_frame(0)
        , m_reversed_image_mode(false) {}

WebcamV4L::~WebcamV4L() {
	Close();
}

bool WebcamV4L::Init(const char* device) {
// Ouverture du périphérique
#ifdef USE_LIBV4L1
	if((m_fd = v4l1_open(device, O_RDWR)) < 0)
#else
	if((m_fd = open(device, O_RDWR)) < 0)
#endif
	{
		return false;
	}

	printf("m_fd == %d\n", m_fd);

// On vérifie qu'il s'agisse bien d'un périphérique supportant la capture vidéo
#ifdef USE_LIBV4L1
	if(v4l1_ioctl(m_fd, VIDIOCGCAP, &m_video_capability) != 0)
#else
	if(ioctl(m_fd, VIDIOCGCAP, &m_video_capability) != 0)
#endif
	{
		fprintf(stderr, "Erreur : le périphérique %s ne supporte pas la capture vidéo avec Video4Linux !\n", device);
		fprintf(stderr, "errno == %d : \"%s\"\n", errno, strerror(errno));
		return false;
	}

	printf("Proprietes du peripherique de capture video :\n");
	printf("Nom  : %s\n", m_video_capability.name);
	printf("Type (video_capability::type) : %d\n", m_video_capability.type);
	printf("Nombre de canaux : %d\n", m_video_capability.channels);

	// Affichage d'informations sur les différents channels :
	for(int i = 0; i < m_video_capability.channels; i++) {
		struct video_channel vc;
		vc.channel = i;
#ifdef USE_LIBV4L1
		if(v4l1_ioctl(m_fd, VIDIOCGCHAN, &vc) != 0)
#else
		if(ioctl(m_fd, VIDIOCGCHAN, &vc) != 0)
#endif
		{
			fprintf(stderr, "Erreur : VIDIOCGCHAN\n");
			fprintf(stderr, "errno == %d : \"%s\"\n", errno, strerror(errno));
			return false;
		}

		printf("Nom du canal %d : %s\n", i, vc.name);
	}

	printf("Nombre de peripheriques audio : %d\n", m_video_capability.audios);
	printf("Resolution maximale : %dx%d\n", m_video_capability.maxwidth, m_video_capability.maxheight);
	printf("Resolution minimale : %dx%d\n", m_video_capability.minwidth, m_video_capability.minheight);

// Récupération du video_picture (informations sur la luminosité, la teinte...etc)
#ifdef USE_LIBV4L1
	if(v4l1_ioctl(m_fd, VIDIOCGPICT, &m_video_picture) != 0)
#else
	if(ioctl(m_fd, VIDIOCGPICT, &m_video_picture) != 0)
#endif
	{
		fprintf(stderr, "Erreur : VIDIOCGPICT\n");
		fprintf(stderr, "errno == %d : \"%s\"\n", errno, strerror(errno));
		return false;
	}

	// Affichage des valeurs de base du video_picture
	printf("Valeurs par defaut lors de l'ouverture de %s (entre 0.0 et 1.0) :\n", device);
	printf("- brightness : %2.1f%% (%d)\n", 100.0f * float(m_video_picture.brightness) / 65535.0f, int(m_video_picture.brightness));
	printf("- hue : %2.1f%% (%d)\n", 100.0f * float(m_video_picture.hue) / 65535.0f, int(m_video_picture.hue));
	printf("- contrast : %2.1f%% (%d)\n", 100.0f * float(m_video_picture.contrast) / 65535.0f, int(m_video_picture.contrast));
	printf("- colour : %2.1f%% (%d)\n", 100.0f * float(m_video_picture.colour) / 65535.0f, int(m_video_picture.colour));
	printf("- whiteness : %2.1f%% (%d)\n", 100.0f * float(m_video_picture.whiteness) / 65535.0f, int(m_video_picture.whiteness));
	printf("- depth : %d\n", int(m_video_picture.depth));
	printf("- palette : %d\n", int(m_video_picture.palette));

	// On choisit de nouvelles valeurs pour tout ça
	m_video_picture.brightness = DEFAULT_BRIGHTNESS;
	m_video_picture.hue = DEFAULT_HUE;
	m_video_picture.contrast = DEFAULT_CONTRAST;
	m_video_picture.colour = DEFAULT_COLOUR;
	m_video_picture.whiteness = DEFAULT_WHITENESS;
	m_video_picture.depth = DEFAULT_DEPTH;
	m_video_picture.palette = DEFAULT_PALETTE;

#ifdef USE_LIBV4L1
	if(v4l1_ioctl(m_fd, VIDIOCSPICT, &m_video_picture) != 0)
#else
	if(ioctl(m_fd, VIDIOCSPICT, &m_video_picture) != 0)
#endif
	{
		fprintf(stderr, "Erreur : VIDIOCSPICT\n");
		fprintf(stderr, "errno == %d : \"%s\"\n", errno, strerror(errno));
		return false;
	}

// Re-récuperation et affichage des valeurs du video_picture
#ifdef USE_LIBV4L1
	if(v4l1_ioctl(m_fd, VIDIOCGPICT, &m_video_picture) != 0)
#else
	if(ioctl(m_fd, VIDIOCGPICT, &m_video_picture) != 0)
#endif
	{
		fprintf(stderr, "Erreur : VIDIOCGPICT\n");
		fprintf(stderr, "errno == %d : \"%s\"\n", errno, strerror(errno));
		return false;
	}
	printf("Valeurs apres les avoir choisies :\n");
	printf("- brightness : %2.1f%% (%d)\n", 100.0f * float(m_video_picture.brightness) / 65535.0f, int(m_video_picture.brightness));
	printf("- hue : %2.1f%% (%d)\n", 100.0f * float(m_video_picture.hue) / 65535.0f, int(m_video_picture.hue));
	printf("- contrast : %2.1f%% (%d)\n", 100.0f * float(m_video_picture.contrast) / 65535.0f, int(m_video_picture.contrast));
	printf("- colour : %2.1f%% (%d)\n", 100.0f * float(m_video_picture.colour) / 65535.0f, int(m_video_picture.colour));
	printf("- whiteness : %2.1f%% (%d)\n", 100.0f * float(m_video_picture.whiteness) / 65535.0f, int(m_video_picture.whiteness));
	printf("- depth : %d\n", int(m_video_picture.depth));
	printf("- palette : %d\n", int(m_video_picture.palette));

// On renseigne m_video_mbuf, qui contient des informations utilisées pour mmap
// (taille du buffer à mmaper, nombre de frames et offsets)
#ifdef USE_LIBV4L1
	if(v4l1_ioctl(m_fd, VIDIOCGMBUF, &m_video_mbuf) != 0)
#else
	if(ioctl(m_fd, VIDIOCGMBUF, &m_video_mbuf) != 0)
#endif
	{
		fprintf(stderr, "Erreur : VIDIOCGMBUF\n");
		fprintf(stderr, "errno == %d : \"%s\"\n", errno, strerror(errno));
		return false;
	}

	printf("Nombre de buffers : %d\n", m_video_mbuf.frames);
	printf("Taille des buffers : %d\n", m_video_mbuf.size);

// Appel de mmap() : on attache la mémoire du périphérique à un pointeur situé dans l'espace utilisateur.
#ifdef USE_LIBV4L1
	m_grab_data = (unsigned char*)v4l1_mmap(0, m_video_mbuf.size, PROT_READ | PROT_WRITE, MAP_SHARED, m_fd, 0);
#else
	m_grab_data = (unsigned char*)mmap(0, m_video_mbuf.size, PROT_READ | PROT_WRITE, MAP_SHARED, m_fd, 0);
#endif

	if(m_grab_data == (unsigned char*)-1) {
		fprintf(stderr, "Erreur lors du mmap() !\n");
		fprintf(stderr, "errno == %d : \"%s\"\n", errno, strerror(errno));
		return false;
	}

	// On renseigne le video_mmap "m_video_mmap"
	m_video_mmap.width = m_width;
	m_video_mmap.height = m_height;
	m_video_mmap.format = DEFAULT_PALETTE;

	printf("Nombre de frames : %d\n", m_video_mbuf.frames);
	printf("Taille demandee : %dx%d\n", m_width, m_height);

	// "Query about number of frames and queue them"
	for(m_frame = 0; m_frame < (unsigned int)m_video_mbuf.frames; m_frame++) {
		m_video_mmap.frame = m_frame;
#ifdef USE_LIBV4L1
		if(v4l1_ioctl(m_fd, VIDIOCMCAPTURE, &m_video_mmap) != 0)
#else
		if(ioctl(m_fd, VIDIOCMCAPTURE, &m_video_mmap) != 0)
#endif
		{
			fprintf(stderr, "Erreur : VIDIOCMCAPTURE\n");
			fprintf(stderr, "errno == %d : \"%s\"\n", errno, strerror(errno));
			return false;
		}
	}
	m_frame = 0;

	// Allocation du buffer contenant l'image en RGB
	delete[] m_p_data;
	m_p_data = new unsigned char[m_width * m_height * 3];

	return true;
}

bool WebcamV4L::WaitForImageSync() {
// On attend que l'image qui a été demandée pour la frame m_frame soit arrivée
#ifdef USE_LIBV4L1
	while(v4l1_ioctl(m_fd, VIDIOCSYNC, &m_frame) < 0)
#else
	while(ioctl(m_fd, VIDIOCSYNC, &m_frame) < 0)
#endif
	{
		if(errno == EINTR) {
			fprintf(stderr, "-");
			continue;
		} else {
			// Erreur : on quitte
			fprintf(stderr, "Erreur : VIDIOCSYNC\n");
			fprintf(stderr, "errno == %d : \"%s\"\n", errno, strerror(errno));
			return false;
		}
	}

	// On renseigne m_p_data
	YUV420PToRGB24(m_grab_data + m_video_mbuf.offsets[m_frame]);

	// Si besoin est, on retourne l'image
	if(m_reversed_image_mode)
		ReverseData();

	return true;
}

bool WebcamV4L::QueryImage() {
	m_video_mmap.frame = m_frame;

// On demande au périphérique d'envoyer une nouvelle image
#ifdef USE_LIBV4L1
	if(v4l1_ioctl(m_fd, VIDIOCMCAPTURE, &m_video_mmap) < 0)
#else
	if(ioctl(m_fd, VIDIOCMCAPTURE, &m_video_mmap) < 0)
#endif
	{
		fprintf(stderr, "Erreur : VIDIOCMCAPTURE\n");
		fprintf(stderr, "errno == %d : \"%s\"\n", errno, strerror(errno));
		return false;
	}

	// Passage à la frame suivante
	m_frame++;
	if(m_frame >= (unsigned int)m_video_mbuf.frames)
		m_frame = 0;

	return true;
}

void WebcamV4L::Close() {
	if(m_fd >= 0) {
#ifdef USE_LIBV4L1
		v4l1_munmap(m_grab_data, m_video_mbuf.size);
		v4l1_close(m_fd);
#else
		munmap(m_grab_data, m_video_mbuf.size);
		close(m_fd);
#endif
		m_fd = -1;

		delete[] m_p_data;
		m_p_data = NULL;
	}
}

// Fonction qui passe au mode image inversée ou normale
void WebcamV4L::SetReversedImageMode(bool reversed) {
	m_reversed_image_mode = reversed;
}

// Fonction qui retourne verticalement l'image pointée par m_p_data
void WebcamV4L::ReverseData() {
	for(int y = 0; y < m_height / 2; y++) {
		for(int x = 0; x < m_width; x++) {
			unsigned char r_temp = 0, g_temp = 0, b_temp = 0;
			int y_comp = m_height - y - 1; // "comp" pour "complémentaire"

			r_temp = m_p_data[(x + y * m_width) * 3 + 0];
			g_temp = m_p_data[(x + y * m_width) * 3 + 1];
			b_temp = m_p_data[(x + y * m_width) * 3 + 2];

			m_p_data[(x + y * m_width) * 3 + 0] = m_p_data[(x + y_comp * m_width) * 3 + 0];
			m_p_data[(x + y * m_width) * 3 + 1] = m_p_data[(x + y_comp * m_width) * 3 + 1];
			m_p_data[(x + y * m_width) * 3 + 2] = m_p_data[(x + y_comp * m_width) * 3 + 2];

			m_p_data[(x + y_comp * m_width) * 3 + 0] = r_temp;
			m_p_data[(x + y_comp * m_width) * 3 + 1] = g_temp;
			m_p_data[(x + y_comp * m_width) * 3 + 2] = b_temp;
		}
	}
}

// Fonction qui prend en entrée un pointeur vers une image en YUV420P et qui met à jour m_p_data,
// qui pointe alors vers une image correcte en RGB24.
void WebcamV4L::YUV420PToRGB24(unsigned char* data) {
	int taille = m_width * m_height;
	unsigned char* py = data;
	unsigned char* pu = data + 1 + taille;
	unsigned char* pv = pu + 1 + taille / 4;
	unsigned char y = 0, u = 0, v = 0;
	unsigned char r = 0, g = 0, b = 0;
	int p = 0;
	int j_debut = 0;
	int j = j_debut;

	unsigned int i = 0;

	for(int ligne = 0; ligne < m_height; ligne++) {
		if(ligne % 2 != 0)
			j = j_debut;
		else
			j_debut = j;

		for(int colonne = 0; colonne < m_width; colonne++) {
			y = py[p];
			p++;
			u = pu[j];
			v = pv[j];

			YUVToRGB(y, u, v, &r, &g, &b);

			m_p_data[i] = r;
			i++;
			m_p_data[i] = g;
			i++;
			m_p_data[i] = b;
			i++;

			if(colonne % 2 == 0)
				j++;
		}
	}
}

// Version de MouMouTe, qui est censée ne pas perdre de l'information qui
// selon lui serait perdue par la version ci-dessus, mais qui en pratique ne marche pas ^^
/*
void WebcamV4L::YUV420PToRGB24(unsigned char* data)
{
   int taille = m_width*m_height;
   unsigned char* py = data;
   unsigned char* pu = data + 1 + taille;
   unsigned char* pv = pu + 1 + taille/4;
   unsigned char y=0, u=0, v=0;
   unsigned char r=0, g=0, b=0;
   int ligne_offset_y, ligne_offset_uv;

   unsigned int i=0;

   for(int ligne=0 ; ligne < m_height ; ligne++)
   {
      ligne_offset_y = ligne*m_height;
      ligne_offset_uv = (ligne/2)*(m_height/2);
      for(int colonne = 0 ; colonne < m_width ; colonne++)
      {
         y = py[ligne_offset_y + colonne];
         u = pu[ligne_offset_uv + (colonne/2)];
         v = pv[ligne_offset_uv + (colonne/2)];

         YUVToRGB(y, u, v, &r, &g, &b);

         m_p_data[i] = r; i++;
         m_p_data[i] = g; i++;
         m_p_data[i] = b; i++;
      }
   }
}
*/
// Changement d'espace colorimétrique
void WebcamV4L::YUVToRGB(unsigned char y, unsigned char u, unsigned char v, unsigned char* pr, unsigned char* pg, unsigned char* pb) {
	// Coeffs tirés de la page Wikipedia "YUV" - donne un effet assez stylé mais pas du tout voulu ^^
	//*pr = (unsigned char)(float(y) - 0.00004f * float(u) + 1.140f  * float(v));
	//*pg = (unsigned char)(float(y) - 0.395f   * float(u) - 0.581f  * float(v));
	//*pb = (unsigned char)(float(y) + 2.032f   * float(u) - 0.0005f * float(v));

	// Coeffs tirés de http://people.via.ecp.fr/~remi/ecp/tpi/rapport/yuv.html :
	*pr = (unsigned char)(clamp(double(y) - 0.0009267 * (double(u) - 128.0) + 1.4016868 * (double(v) - 128.0), 0.0, 255.0));
	*pg = (unsigned char)(clamp(double(y) - 0.3436954 * (double(u) - 128.0) - 0.7141690 * (double(v) - 128.0), 0.0, 255.0));
	*pb = (unsigned char)(clamp(double(y) + 1.7721604 * (double(u) - 128.0) + 0.0009902 * (double(v) - 128.0), 0.0, 255.0));
}

// Restreindre une valeur à un sol et un plafond
double WebcamV4L::clamp(double value, double min, double max) {
	if(value < min)
		value = min;
	if(value > max)
		value = max;
	return value;
}

#endif // !defined WIN32
