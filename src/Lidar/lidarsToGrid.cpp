#include "lidarsToGrid.h"

#define PI 3.14159265

// Décommenter pour considérer les points entre deux relevés d'un lidar comme occupés.
//#define RATISSE_LIGNES

OccupGrid::OccupGrid(double width, double height, int resX, int resY)
        : _width(width), _height(height), _resX(resX), _resY(resY), _mat(resX * resY) {}

void OccupGrid::reset() {
	for(auto& c : _mat) {
		c = 0;
	}
}

#ifndef RATISSE_LIGNES

void OccupGrid::accumulate(FrameLidar t, Vec2 pos, Angle orient) {
	Angle angle;

	int i = 0;
	for(auto& dist : t.points) {
		angle = i * t.angularResolution + t.begin + orient;

		double x = pos.x + dist.toM() * cos(angle);
		double y = pos.y + dist.toM() * sin(angle);

		// write true if point is inside grid.
		int px = static_cast<int>(std::round((x * _resX) / _width));
		int py = static_cast<int>(std::round((y * _resY) / _height));

		if(px >= 0 && px < _resX && py >= 0 && py < _resY) {
			(*this)(px, py) = 1;
		}

		++i;
	}
}

#else

void OccupGrid::accumulate(FrameLidar t, Vec2 pos, Angle orient) {
	Angle angle = orient + t.begin;

	Vec2 tmp;
	Distance dist = t.points.front();
	tmp.x = pos.x + dist.toM() * cos(angle);
	tmp.y = pos.y + dist.toM() * sin(angle);
	IVec2 prev;
	prev.x = (tmp.x * _resX) / _width;
	prev.y = (tmp.y * _resY) / _height;

	IVec2 pt;
	for(size_t i = 1; i < t.points.size(); ++i) {
		dist = t.points[i];
		angle = i * t.angularResolution + t.begin + orient;

		tmp.x = pos.x + dist.toM() * cos(angle);
		tmp.y = pos.y + dist.toM() * sin(angle);

		// write true if point is inside grid.
		pt.x = (tmp.x * _resX) / _width;
		pt.y = (tmp.y * _resY) / _height;

		_rsLine(prev, pt);

		prev = pt;
	}
}

void OccupGrid::_rsLine(IVec2 a, IVec2 b) {
	Vec2 cur = a;
	Vec2 fin = b;

	int px, py;

	Vec2 inc = b - a;
	if(abs(inc.x) > abs(inc.y))
		inc /= 1.1 * abs(inc.x);
	else if(abs(inc.y))
		inc /= 1.1 * abs(inc.y);
	// else: keep null vector.

	float d;
	do {
		px = std::round(cur.x);
		py = std::round(cur.y);
		if(px >= 0 && px < _resX && py >= 0 && py < _resY)
			(*this)(px, py) = 1;

		cur += inc;

	} while(px != b.x || py != b.y);
}

#endif