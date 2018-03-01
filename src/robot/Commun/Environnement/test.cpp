#include "environment.h"
#include "math.h"
#include "stdio.h"
#include "stdlib.h"
#include <chrono>
#include <string>
#include <vector>

#include "../../../commun/ConstantesCommunes.h"
#include "spline.h"
using namespace std;

void printV(vector<Vector2m>& pts);

void printV(vector<Vector2m>& pts) {
	for(Vector2m& pt : pts) {
		cout << pt << endl;
	}
}

const float gridSize = 0.01;
int main() {
	const int w = 300;
	const int h = 200;
	const int times = 1;
	cout << "Test A*" << endl;

	// Création de la grille de l'A*.
	Distance sx = 40_cm;
	Distance sy = 30_cm;
	Environment helloWorld({w, h}, Distance::makeFromM(gridSize), sy, (sqrt(sx * sx + sy * sy) / 2) * 1.2, Vector2m(0_m, 1_m));

	const Distance WALLS_SIZE{3_cm};
	const Vector2m WALLS_DIMENSIONS{3_m, 2_m};

	const float danger = Environment::DANGER_INFINITY;

	// Murs
	helloWorld.addStaticShape(std::make_unique<Rect>(danger,
	                                                 Vector2m(-WALLS_SIZE, -WALLS_SIZE),
	                                                 Vector2m(WALLS_DIMENSIONS.x + 2 * WALLS_SIZE, WALLS_SIZE)));
	helloWorld.addStaticShape(std::make_unique<Rect>(danger,
	                                                 Vector2m(-WALLS_SIZE, -WALLS_SIZE),
	                                                 Vector2m(WALLS_SIZE, WALLS_DIMENSIONS.y + 2 * WALLS_SIZE)));
	helloWorld.addStaticShape(std::make_unique<Rect>(danger,
	                                                 Vector2m(-WALLS_SIZE, WALLS_DIMENSIONS.y),
	                                                 Vector2m(WALLS_DIMENSIONS.x + 2 * WALLS_SIZE, WALLS_SIZE)));
	helloWorld.addStaticShape(std::make_unique<Rect>(danger,
	                                                 Vector2m(WALLS_DIMENSIONS.x, -WALLS_SIZE),
	                                                 Vector2m(WALLS_SIZE, WALLS_DIMENSIONS.y + 2 * WALLS_SIZE)));

	// Bâtons devant la base
	helloWorld.addStaticShape(std::make_unique<Rect>(danger, Vector2m(0_m, 36_cm), Vector2m(71_cm, 22_mm)));
	helloWorld.addStaticShape(std::make_unique<Rect>(danger, Vector2m(2.29_m, 36_cm), Vector2m(71_cm, 22_mm)));

	// bascule
	helloWorld.addStaticShape(std::make_unique<Rect>(danger, Vector2m(0_m, 0_m), Vector2m(71_cm, 36_cm)));
	helloWorld.addStaticShape(std::make_unique<Rect>(danger, Vector2m(2.29_m, 0_m), Vector2m(71_cm, 36_cm)));

	// Cercles
	helloWorld.addStaticShape(std::make_unique<Circle>(danger, 115_mm, Vector2m(65_cm, 54_cm)));
	helloWorld.addStaticShape(std::make_unique<Circle>(danger, 115_mm, Vector2m(2.35_m, 54_cm)));
	helloWorld.addStaticShape(std::make_unique<Circle>(danger, 115_mm, Vector2m(1.07_m, 1.87_m)));
	helloWorld.addStaticShape(std::make_unique<Circle>(danger, 115_mm, Vector2m(1.93_m, 1.87_m)));
	helloWorld.addStaticShape(std::make_unique<Circle>(danger, 54_cm, Vector2m(0_m, 2_m)));
	helloWorld.addStaticShape(std::make_unique<Circle>(danger, 54_cm, Vector2m(3_m, 2_m)));

	// Batiments centraux
	helloWorld.addStaticShape(std::make_unique<Rect>(danger, Vector2m(1.432_m, 1.2_m), Vector2m(136_mm, 60_cm)));
	helloWorld.addStaticShape(std::make_unique<Rect>(danger, Vector2m(886.2_mm, 1.55_m), Vector2m(136_mm, 60_cm), 45_deg));
	helloWorld.addStaticShape(std::make_unique<Rect>(danger, Vector2m(2.15_m, 1.25_m), Vector2m(136_mm, 60_cm), -45_deg));
	helloWorld.addStaticShape(std::make_unique<Circle>(danger, 20_cm, Vector2m(1.5_m, 2_m)));

	// Batiments latéraux
	helloWorld.addStaticShape(std::make_unique<Rect>(danger, Vector2m(0_m, 678_mm), Vector2m(108_mm, 494_mm)));
	helloWorld.addStaticShape(std::make_unique<Rect>(danger, Vector2m(2.892_m, 678_mm), Vector2m(108_mm, 494_mm)));


	// Trajets
	vector<Vector2m> tests = {Vector2m(270_cm, -30_cm),
	                          Vector2m(50_cm, 30_cm),

	                          Vector2m(219_cm, 70_cm),
	                          Vector2m(50_cm, 30_cm)};

	// Exécution de plusieurs tests => save to tga
	for(int i = 0; i < tests.size() / 2; i++) {
		auto t = chrono::system_clock::now();
		helloWorld.clearDebugValues();

		auto linear_pts = helloWorld.getTrajectory(tests[i * 2], tests[i * 2 + 1], 3_s, false);
		linear_pts.emplace_back(tests[i * 2]);
		helloWorld.saveToTGA(("TrajetLineaire" + std::to_string(i) + ".tga").c_str(), linear_pts);


		/// Test des interpolations par splines cubiques paramétrées
		auto cubic_pts = helloWorld.getTrajectory(tests[i * 2], tests[i * 2 + 1], 3_s, false);
		// On vérifie qu'il n'y a pas deux points consécutifs égaux (sinon les dérivées secondes font du caca)
		cubic_pts.erase(std::unique(cubic_pts.begin(), cubic_pts.end()), cubic_pts.end());

		std::vector<Vector2m> robot_pts = {{1_dm, 9_dm}, // R
		                                   {1_dm, -85_cm},  {1_dm, -9_dm},   {3_dm, -85_cm},  {45_cm, -7_dm},
		                                   {55_cm, -45_cm}, {45_cm, -2_dm},  {3_dm, -5_cm},   {15_cm, 0_m},
		                                   {16_cm, 1_cm},   {6_dm, 9_dm},

		                                   {9_dm, 9_dm}, // O
		                                   {1.15_m, 0_m},   {1_m, -85_cm},   {9_dm, -9_dm},   {8_dm, -85_cm},
		                                   {65_cm, 0_m},    {75_cm, 9_dm},   {9_dm, 9_dm},

		                                   {1.3_m, 9_dm}, // B
		                                   {1.3_m, 89_cm},  {1.3_m, -85_cm}, {1.3_m, -9_dm},  {1.45_m, -85_cm},
		                                   {1.6_m, -7_dm},  {1.7_m, -45_cm}, {1.6_m, -2_dm},  {1.5_m, -5_cm},
		                                   {1.35_m, 0_m},   {1.65_m, 2_dm},  {1.75_m, 45_cm}, {1.65_m, 7_dm},
		                                   {1.5_m, 85_cm},  {1.3_m, 95_cm},  {1.29_m, 95_cm},

		                                   {2.1_m, 9_dm}, // O
		                                   {2.35_m, 0_m},   {2.2_m, -85_cm}, {2.1_m, -9_dm},  {2_m, -85_cm},
		                                   {1.85_m, 0_m},   {1.95_m, 9_dm},  {2.1_m, 9_dm},

		                                   {2.7_m, 8_dm}, // T
		                                   {2.7_m, 79_cm},  {2.7_m, -89_cm}, {2.7_m, -9_dm},  {2.24_m, -9_dm},
		                                   {2.25_m, -9_dm}, {3.04_m, -85_cm}};
		parameter_spline spline;
		spline.set_points(cubic_pts);

		auto curvature_informations = spline.get_curvature_advance(50_cm);
		cout << "Il y a " << curvature_informations.size() << " points" << endl;
		/*for(auto infos : curvature_informations) {
		    std::cout << "Coefficient de vitesse gauche : " << infos.speedCoeffL <<
		               "\nCoefficient de vitesse droit : "  << infos.speedCoeffR <<
		               "\nDistance à parcourir : " << infos.length << '\n' << endl;
		}*/
		vector<Vector2m> new_pts = spline.get_points();
		helloWorld.saveToTGA(("TrajetCubique" + std::to_string(i) + ".tga").c_str(), new_pts);


		auto time = chrono::system_clock::now() - t;
		cout << "Trajet " << i << " terminé ( " << (chrono::duration_cast<chrono::microseconds>(time).count()) / times << "µs)\n"
		     << endl;
	}

	return 0;
}