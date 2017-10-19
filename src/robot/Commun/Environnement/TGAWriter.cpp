//
// Created by scriptopathe on 23/04/15.
//

#include "TGAWriter.h"

void TGAWriter::drawPoint(int color, int x, int y) {
	int index = x + (m_height - y - 1) * m_width;

	if(index < m_image.size())
		m_image[index] = color;
}

void TGAWriter::drawLine(int color, Vector2m start, Vector2m end) {
	Vector2m direction = end - start;
	direction *= 1.0f / direction.norm().toM();
	Vector2m pos = start;
	while((end - pos).norm().toM() > 1) {
		pos += direction * 0.5f;
		drawPoint(color, (int)pos.x.toM(), (int)pos.y.toM());
	}
}

FILE* TGAWriter::writeTGAHeader(const char* path, int w, int h) {
	FILE* fptr;
	if((fptr = fopen(path, "w")) == NULL) {
		fprintf(stderr, "Failed to open outputfile\n");
		exit(-1);
	}
	putc(0, fptr);
	putc(0, fptr);
	putc(2, fptr);
	putc(0, fptr);
	putc(0, fptr);
	putc(0, fptr);
	putc(0, fptr);
	putc(0, fptr);
	putc(0, fptr);
	putc(0, fptr);
	putc(0, fptr);
	putc(0, fptr);
	putc((w & 0x00FF), fptr);
	putc((w & 0xFF00) / 256, fptr);
	putc((h & 0x00FF), fptr);
	putc((h & 0xFF00) / 256, fptr);
	putc(32, fptr);
	putc(0, fptr);

	return fptr;
}

TGAWriter::TGAWriter(int w, int h) {
	m_width = w;
	m_height = h;
	m_image = std::vector<int>(w * h);
}

void TGAWriter::save(const char* path) {
	FILE* file = writeTGAHeader(path, m_width, m_height);
	fwrite(m_image.data(), m_image.size() * 4, 1, file);
	fclose(file);
}