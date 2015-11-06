// ImageAnalyzer.h

#ifndef IMAGE_ANALYZER_H
#define IMAGE_ANALYZER_H

#include "ImageBinarizer.h"
#include "ItemsFinder.h"

class ImageAnalyzer {
public:
	typedef ItemsFinder::Item Item;
	typedef ItemsFinder::ItemsList ItemsList;

	struct Config {
		ItemsFinder::Config itemsFinderConfig;
		ImageBinarizer::Config imageBinarizerConfig;
	};

private:
	ImageBinarizer binarizer;
	ItemsFinder finder;

	Config config;

public:
	ImageAnalyzer(int width, int height);
	virtual ~ImageAnalyzer();

	void run(const char *dataRGB);

	const ItemsList &getFoundItems() const;

	void setConfig(const Config &config);
	const Config &getConfig() const;
};

#endif // IMAGE_ANALYZER_H
