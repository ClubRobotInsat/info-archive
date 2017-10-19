// ImageAnalyser.cpp

#include "ImageAnalyzer.h"

ImageAnalyzer::ImageAnalyzer(int width, int height) : binarizer(width, height), finder(width, height) {}

ImageAnalyzer::~ImageAnalyzer() {}

void ImageAnalyzer::run(const char* dataRGB) {
	binarizer.run(dataRGB);
	finder.run(binarizer.getDataGray());
}

const ImageAnalyzer::ItemsList& ImageAnalyzer::getFoundItems() const {
	return (const ImageAnalyzer::ItemsList&)finder.getFoundItems();
}

void ImageAnalyzer::setConfig(const Config& config) {
	this->config = config;
	this->binarizer.setConfig(config.imageBinarizerConfig);
	this->finder.setConfig(config.itemsFinderConfig);
}

const ImageAnalyzer::Config& getConfig() const {
	return config;
}
