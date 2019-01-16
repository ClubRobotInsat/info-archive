/******************************************************************************/
/* info/robot/Lidar/lidarServer.cpp                                2015-05-08 */
/* Club Robot INSA Toulouse                                      Félix Poisot */
/******************************************************************************/
#include "lidarServer.h"
#include <Clock.h>
#include <log/Log.h>
/******************************************************************************/

using namespace std;

namespace Lidar {
	LidarThread::LidarThread(bool useGL) : _hasGL(useGL) {}

	void LidarThread::run() {
		StopWatch clkReco;
		Filtre fSick;
		Filtre fHokuyo;
		Vector2m table_size = toVec2(GLOBAL_CONSTANTS().get_table_size());
		OccupGrid grid(table_size, 150, 100);

		if(_hasGL) {
			_aff = std::make_unique<Display>(table_size);
		}

		for(;;) {
			/* reconnecter les Lidars: contôler la fréquence des tentatives */
			if(clkReco.getElapsedTime() > 2_s) {
				if(_sick == nullptr) {
					try {
						_sick = Lidar::open_lidar(Type::Sick);
					} catch(...) {
					}
				}
				if(_hokuyo == nullptr) {
					try {
						logDebug("Ouverture Hokuyo...");
						_hokuyo = Lidar::open_lidar(Type::Hokuyo);
						logDebug("réussie !");
					} catch(...) {
					}
				}
				clkReco.reset();
			}

			/* accumule les données des lidars disponibles */
			if(_aff != nullptr) {
				_aff->begin();
			}
			grid.reset();
			if(_sick != nullptr) {
				try {
					repere::Coordinates coords({0_m, 1_m}, 0_deg);
					auto t = _sick->get_frame();
					auto ft = fSick.get_frame(t);
					grid.accumulate(ft, coords);
					if(_aff != nullptr) {
						_aff->frame_lidar(ft, coords, {0, 0.5, 0, 0.7f});
					}
				} catch(std::runtime_error&) {
					logWarn("lidar perdu !");
					_sick.reset();
				}
			}
			if(_hokuyo != nullptr) {
				try {
					repere::Coordinates coords({3_m, 2_m}, 180_deg);
					auto t = _hokuyo->get_frame();
					auto ft = fHokuyo.get_frame(t);
					// Petit lidar dans le coin du haut, rasPi dans le coin du bas.
					grid.accumulate(ft, coords);
					if(_aff != nullptr) {
						_aff->frame_lidar(ft, coords, {0, 0.5, 0.5, 0.7f});
					}
				} catch(std::runtime_error&) {
					logWarn("lidar perdu !");
					_hokuyo.reset();
				}
			}

			if(_aff != nullptr) {
				_aff->grid(grid, {0, 0, 0});
			}

			/* interprète, et expose aux autres threads. */
			_lTr.lock();
			_tr.accumulate(grid);
			if(_aff != nullptr) {
				_aff->candidates(_tr.get_results(), {1, 0.5f, 0});
			}
			_lTr.unlock();

			if(_aff != nullptr) {
				_aff->end();
			}
		}
	}

	vector<bool> LidarThread::status() const {
		return {_sick != nullptr, _hokuyo != nullptr};
	}

	vector<repere::Position> LidarThread::records() {
		std::lock_guard<std::mutex> lk(_lTr);
		return _tr.get_results();
	}

	string LidarThread::recordsAsText() { // dans un format machine-friendly
		auto arr = records();
		std::ostringstream oss;
		oss << arr.size() << ' ';
		for(auto v : arr) {
			oss << static_cast<int>(v.getX().toMm()) << ' ' << static_cast<int>(v.getY().toMm()) << ' ';
		}
		oss << '\n';
		return oss.str();
	}

	string LidarThread::recordsChrMap() {
		auto r = records();

		const int W = 80;
		const int H = 28;
#define wr(x, y, c) mat[(y) * (W + 1) + (x)] = c

		double xdens = W / GLOBAL_CONSTANTS().get_table_size().x.toM(); // chr / m
		double ydens = H / GLOBAL_CONSTANTS().get_table_size().y.toM();
		vector<char> mat((W + 1) * (H), '.');
		for(int i = 0; i < H; ++i) {
			wr(W, i, '\n');
		}
		mat.back() = '\0';

		for(auto v : r) {
			wr(min<int>(round(v.getX().toM() * xdens), W), min<int>(round(v.getY().toM() * ydens), H), 'X');
		}

		return string(mat.data());
	}
} // namespace Lidar
