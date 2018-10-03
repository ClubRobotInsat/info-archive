/******************************************************************************/
/* info/robot/Lidar/lidarServer.cpp                                2015-05-08 */
/* Club Robot INSA Toulouse                                      Félix Poisot */
/******************************************************************************/
#include "lidarServer.h"
#include <Clock.h>
#include <log/Log.h>
/******************************************************************************/

using namespace std;

LidarThread::LidarThread(bool useGL) {
	_hasGL = useGL;
}

const double WIDTH = 3.0;
const double HEIGHT = 2.0;

void LidarThread::run() {
	StopWatch clkReco;
	Filtre fSick;
	Filtre fHokuyo;
	OccupGrid grid(WIDTH, HEIGHT, 150, 100);

	if(_hasGL)
		_aff = std::make_unique<Affiche>(Vec2(WIDTH, HEIGHT));

	for(;;) {
		/* reconnecter les Lidars: contôler la fréquence des tentatives */
		if(clkReco.getElapsedTime() > 2_s) {
			if(!_sick) {
				try {
					_sick = Lidar::openLidar(Lidar::Sick);
				} catch(...) {
				}
			}
			if(!_hokuyo) {
				try {
					logDebug("Ouverture Hokuyo...");
					_hokuyo = Lidar::openLidar(Lidar::Hokuyo);
					logDebug("réussie !");
				} catch(...) {
				}
			}
			clkReco.reset();
		}

		/* accumule les données des lidars disponibles */
		if(_aff)
			_aff->begin();
		grid.reset();
		if(_sick) {
			try {
				auto t = _sick->getTrame();
				auto ft = fSick.getTrame(t);
				grid.accumulate(ft, {0.0, 1.0}, 0_deg);
				if(_aff)
					_aff->trameLidar(ft, {0, 1}, 0_deg, {0, 0.5, 0, 0.7f});
			} catch(std::runtime_error&) {
				logWarn("lidar perdu !");
				_sick.reset();
			}
		}
		if(_hokuyo) {
			try {
				auto t = _hokuyo->getTrame();
				auto ft = fHokuyo.getTrame(t);
				// Petit lidar dans le coin du haut, rasPi dans le coin du bas.
				grid.accumulate(ft, {3.0, 2.0}, 180_deg);
				if(_aff)
					_aff->trameLidar(ft, {3, 2}, 180_deg, {0, 0.5, 0.5, 0.7f});
			} catch(std::runtime_error&) {
				logWarn("lidar perdu !");
				_hokuyo.reset();
			}
		}

		if(_aff)
			_aff->grille(grid, {0, 0, 0});

		/* interprète, et expose aux autres threads. */
		_lTr.lock();
		_tr.accumulate(grid);
		if(_aff)
			_aff->candidats(_tr.getResults(), {1, 0.5f, 0});
		_lTr.unlock();

		if(_aff)
			_aff->end();
	}
}

vector<bool> LidarThread::status() const {
	return {_sick != nullptr, _hokuyo != nullptr};
}

vector<Vec2> LidarThread::releves() {
	_lTr.lock();
	auto r = _tr.getResults();
	_lTr.unlock();
	return r;
}

string LidarThread::relevesAsText() { // dans un format machine-friendly
	auto arr = releves();
	std::ostringstream oss;
	oss << arr.size() << ' ';
	for(auto v : arr) {
		oss << (int)(v.x * 1000) << ' ' << (int)(v.y * 1000) << ' ';
	}
	oss << '\n';
	return oss.str();
}

string LidarThread::relevesChrMap() {
	auto r = releves();

	const int W = 80;
	const int H = 28;
#define wr(x, y, c) mat[(y) * (W + 1) + (x)] = c

	double xdens = W / WIDTH; // chr / m
	double ydens = H / HEIGHT;
	vector<char> mat((W + 1) * (H), '.');
	for(int i = 0; i < H; ++i)
		wr(W, i, '\n');
	mat.back() = '\0';

	for(auto v : r) {
		wr(min<int>(round(v.x * xdens), W), min<int>(round(v.y * ydens), H), 'X');
	}

	return string(mat.data());
}