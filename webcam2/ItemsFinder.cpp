// ItemsFinder.cpp

#include "ItemsFinder.h"

ItemsFinder::ItemsFinder(int width, int height) : width(width), height(height) {}

ItemsFinder::~ItemsFinder() {}

void ItemsFinder::setConfig(const Config &config) {
	this->config = config;
}

const ItemsFinder::Config &ItemsFinder::getConfig() const {
	return this->config;
}
