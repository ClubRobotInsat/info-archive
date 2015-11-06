// ImageBinarizer.h

#ifndef IMAGE_BINARIZER_H
#define IMAGE_BINARIZER_H

#include "ColorHSV.h"

class ImageBinarizer {
private:
	unsigned char *dataGray;
	int width, height;

	Config config;

public:
	enum BlurLevel { DONT_BLUR, BLUR_A_LITTLE, BLUR_A_LOT };

	struct Config {
		ColorHSV colorHsvMin;
		ColorHSV colorHsvMax;
		BlurLevel blurLevel;
	};

	ImageBinarizer(int width, int height);
	virtual ~ImageBinarizer();

	void run(const char *dataRGB);

	const char *getDataGray() const;

	void setConfig(const Config &config);
	const Config &getConfig() const;
};

#endif // IMAGE_BINARIZER_H
