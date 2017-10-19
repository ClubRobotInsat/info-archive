//
// Created by scriptopathe on 23/04/15.
//

#ifndef ROOT_TGAWRITER_H
#define ROOT_TGAWRITER_H

#include <Commun.h>
#include <iostream>
#include <vector>

class TGAWriter {
public:
	TGAWriter(int w, int h);
	void drawPoint(int color, int x, int y);
	void drawLine(int color, Vector2m start, Vector2m end);
	void save(const char* path);


	int getWidth() const {
		return m_width;
	}

	void setWidth(int m_width) {
		TGAWriter::m_width = m_width;
	}

	int getHeight() const {
		return m_height;
	}

	void setHeight(int m_height) {
		TGAWriter::m_height = m_height;
	}

private:
	int m_width;
	int m_height;
	std::vector<int> m_image;
	FILE* writeTGAHeader(const char* path, int w, int h);
};


#endif // ROOT_TGAWRITER_H
