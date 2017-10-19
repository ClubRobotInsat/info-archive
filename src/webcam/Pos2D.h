// Pos2D.h

#ifndef POS2D_H
#define POS2D_H

struct Pos2D {
	int x, y;
	Pos2D() : x(0), y(0) {}
	Pos2D(int x, int y) {
		this->x = x;
		this->y = y;
	}
	Pos2D(const Pos2D& ref) {
		*this = ref;
	}

	Pos2D& operator=(const Pos2D& ref) {
		this->x = ref.x;
		this->y = ref.y;
		return *this;
	}
};

#endif // POS2D_H
