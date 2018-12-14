/*****************************************************************************/
/* robot/Lidar/trouveRobots.cpp                                   2015-05-11 */
/* Club Robot INSA Toulouse                                     Félix Poisot */
/*****************************************************************************/
#include "FindRobots.h"
/*****************************************************************************/

namespace {

	const int MAX_MOVE = 8;  // déplacement en cellules d'un robot entre deux relevés
	const int MIN_SIZE = 12; // quantité de voisins invalidés lors de la validation d'un point.
	const int MIN_PTS = 6;   // nb de points à trouver pour considérer une victoire

	// score minimal pour qu'un condidat soit reporté à l'utilisateur
	const int MIN_WIN = 10;
	const int MAX_WIN = 12; // cap de score.
} // namespace

using namespace std;

void FindRobots::accumulate(const OccupGrid& grid) {
// exécute pour tous les points sur le bord du carré
#define CIRCLE(center, radius, body)                               \
	{                                                              \
		IVec2 pos;                                                 \
		if(radius > 0) {                                           \
			for(int _p = 0; _p < (radius * 2); ++_p) {             \
				pos = center + IVec2(-radius, _p - radius);        \
				{body} pos = center + IVec2(_p - radius, radius);  \
				{body} pos = center + IVec2(radius, radius - _p);  \
				{body} pos = center + IVec2(radius - _p, -radius); \
				{ body }                                           \
			}                                                      \
		} else {                                                   \
			pos = center;                                          \
			body                                                   \
		}                                                          \
	}


// exécute le corps sur tous les points dans un carré, en testant l'intérieur
// d'abord. 'break;' interdit. Coords hors map possibles.
#define SPIRAL(center, maxRad, body)  \
	for(int r = 0; r < maxRad; ++r) { \
		CIRCLE(center, r, body);      \
	}


	IVec2 tp = grid.resolution();
	Vec2 ech = grid.size();
	// un point sur la grille ne peut sauver/devenir qu'un candidat.
	std::vector<char> scanned(static_cast<size_t>(tp.x * tp.y));

	// printf("\ndébut\n");

	// les candidats passés 'vieillissent'
	size_t tc = _candidats.size();
	for(int i = 0; i < tc; ++i) {
		int cWin = 0;

		IVec2 pCand = {static_cast<int>(round((_candidats[i].x / ech.x) * tp.x)),
		               static_cast<int>(round((_candidats[i].y / ech.y) * tp.y))};
		SPIRAL(pCand, MIN_SIZE, {
			int ind = (pos.y * tp.x) + pos.x;
			if(pos.x >= 0 && pos.x < tp.x && pos.y >= 0 && pos.y < tp.y) {
				if(!scanned[ind]) {
					scanned[ind] = 1;
					if(r < MAX_MOVE && grid(pos.x, pos.y)) { // +1 point vers la victoire
						++cWin;
						_nbWin[i] = min(MAX_WIN, _nbWin[i] + 1);

						// pos devient la moyenne des points sauveurs
						_candidats[i] *= 1 - (1 / static_cast<double>(cWin));
						_candidats[i] += (1 / static_cast<double>(cWin)) * Vec2((pos.x / static_cast<double>(tp.x)) * ech.x,
						                                                        (pos.y / static_cast<double>(tp.y)) * ech.y);

						// printf("win d'un vieux point\n");
					}
				}
			}
		});

		// spirale terminée.
		if(cWin < MIN_PTS) {
			// printf("défaite d'un vieux point\n");
			if(_nbWin[i] == 1) { // dernière défaite
				// suppression du candidat
				_nbWin[i] = _nbWin.back();
				_nbWin.pop_back();
				_candidats[i] = _candidats.back();
				_candidats.pop_back();
			} else {
				_nbWin[i]--;
			}
		}
	}

	for(int y = 0; y < tp.y; ++y)
		for(int x = 0; x < tp.x; ++x) {
			int ind = (y * tp.x) + x;
			if(!scanned[ind] && grid(x, y)) { // nouveau point, là où il n'y avait pas de candidat
				// printf("nouveau point\n");
				_candidats.push_back(Vec2((x / (double)tp.x) * ech.x, (y / (double)tp.y) * ech.y));
				_nbWin.push_back(1);

				// limite la densité des nouveaux candidats
				SPIRAL(IVec2(x, y), MIN_SIZE, {
					if(pos.x >= 0 && pos.x < tp.x && pos.y >= 0 && pos.y < tp.y) {
						int ind2 = (pos.y * tp.x) + pos.x;
						scanned[ind] = 1;
					}
				});
			}
		}
}


std::vector<Vec2> FindRobots::get_results() const {
	std::vector<Vec2> res;
	for(int i = 0; i < _candidats.size(); ++i) {
		if(_nbWin[i] > MIN_WIN)
			res.push_back(_candidats[i]);
	}

	return res;
}
