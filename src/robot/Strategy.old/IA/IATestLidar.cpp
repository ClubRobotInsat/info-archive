//
//  IATestLidar.cpp
//  Club Robot
//
//  Created by Rémi on 01/06/2015.
//

#include "../../../Principal/librobot/libRobot.h"
#include <Commun.h>

class IATestLidar : public StrategiePrincipal {
public:
	/// WARNING : Verifier que la couleur du robot c'est bon.
	IATestLidar(std::vector<std::string> const& args) : StrategiePrincipal(args, RobotColor::Green) {
		this->initialisation();
	}

protected:
	void initialisation() {
		// Recalage au depart
		// pour ne pas se recaler si on teste dans simu

		logDebug6("Recalage du robot");
		getDeplacement().setRepere(Vector2m(24_cm, 99_cm), 180_deg);

		// Tirette
		logInfo("Attente tirette…");
		// if(getenv("EPIA"))
		// attendreTirette();
		getchar();
	}

	/// Contient la logique de la stratégie elle-même
	virtual void executer() override;

	void bilanPosition() {
		auto posRobot = _robot->lireCoordonnees().getPos2D();
		logInfo("Je suis en : ", posRobot);

		auto robotsDetectes = this->tousLesRobots();
		int i = 1;
		for(auto& p : robotsDetectes) {
			if(!p.second) {
				auto pos = p.first;
				logInfo("Robot ", i++, "détecté, delta (moi - robot) =", posRobot - pos);
			}
		}
	}

	void allerABilan(Vector2m pos) {
		_dep->allerA(pos, SensAvance::Avant, 20_s);

		bilanPosition();
		getchar();
	}
};

int main(int argc, char* argv[]) {
	Log::open(argc, argv, false);

	logDebug6("Initialisation du robot...Veuillez patienter...");
	IATestLidar strategie({argv, argv + argc});

	logDebug6("Démarre la stratégie");
	strategie.demarrer(MATCH_DURATION * 1000);

	return 0;
}

void IATestLidar::executer() {
	_dep->avancer(40_cm, SensAvance::Arriere);

	auto t = std::thread([this]() {
		while(true) {
			auto posRobot = _robot->lireCoordonnees().getPos2D();
			logInfo("Je suis en : ", posRobot);

			auto robotsDetectes = this->tousLesRobots();
			int i = 1;
			for(auto& p : robotsDetectes) {
				if(!p.second) {
					auto pos = p.first;
					logInfo("Robot ", i++, " détecté : ", pos, " delta : ", (pos - posRobot).norm());
				}
			}

			sleep(0.5_s);
		}
	});

	auto const dimX = 1.5_m, dimY = 1_m;
	Vector2m pos = {0_m, 0_m};
	while(true) {
		auto val1 = ((rand() % 1007 / 1007.0f) * 2 - 1) * dimX, val2 = ((rand() % 1007 / 1007.0f) * 2 - 1) * dimY;
		while(abs(val1) < 30_cm) {
			val1 *= 2;
		}
		while(abs(val2) < 30_cm) {
			val2 *= 2;
		}

		auto nextPos = pos + Vector2m(val1, val2);
		nextPos.x = std::min(std::max(nextPos.x, 70_cm), 1.7_m);
		nextPos.y = std::min(std::max(nextPos.y, 75_cm), 1.5_m);

		pos = nextPos;

		logMagenta(nextPos);

		this->getDeplacement().allerA(nextPos);
	}

	/*while(true) {
	    logInfo("Entrez les prochaines coordonnées :");
	    double v1, v2;
	    std::cin >> v1 >> v2;
	    allerABilan({Distance::makeFromM(v1), Distance::makeFromM(v2)});
	}*/

	/*allerABilan({1_m, 1_m});
	allerABilan({1_m, 1.5_m});
	allerABilan({0.5_m, 1.5_m});*/
}
