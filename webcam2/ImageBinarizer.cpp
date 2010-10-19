// ImageBinarizer.cpp

#include "ImageBinarizer.h"

ImageBinarizer::ImageBinarizer(int width, int height)
: width(width), height(height)
{
	this->dataGray = new unsigned char[width * height];
}

ImageBinarizer::~ImageBinarizer()
{
	delete [] this->dataGray;
}

const char* ImageBinarizer::getDataGray()
{
	return this->dataGray;
}

void ImageBinarizer::setConfig(const Config &config)
{
	this->config = config;
}

const ImageBinarizer::Config& getConfig() const
{
	return this->config;
}
