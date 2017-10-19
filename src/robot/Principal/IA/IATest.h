// IATestDeplacement.h

#ifndef IA_test
#define IA_test

#include "../../../commun/Commun.h"
#include "../librobot/libRobot.h"

class IATest : public StrategiePrincipal {
public:
	IATest(std::vector<std::string> const& args) : StrategiePrincipal(args, RobotColor::Blue) {
		this->initialisation();
	}

protected:
	void initialisation() {
		// Recalage au depart
		// pour ne pas se recaler si on teste dans simu


		logDebug("Recalage du robot");
		// strategie.recalageZoneDepart();
		// {30_cm, 20_cm, 150_cm}
		//		getDeplacement().setRepere({30_cm, 150_cm}, 0_PI);
		getDeplacement().setRepere({ConstantesPrincipal::START_ROBOT_POSITION,
		                            ConstantesPrincipal::START_ROBOT_ANGLE,
		                            _robot->getStrategie().getReference()});

		// Tirette
		logInfo("Attente tirette…");
		//		attendreTirette();
		getchar();
	}

	/// Contient la logique de la stratégie elle-même
	virtual void executer() = 0;
};

/**
 * Ce .h est le placeholder pour tous les *.cpp qui contiennent "Test" dans leur nom
 */

/**
 * IA DE TEST DU DEPLACEMENT
 */
class IATestDeplacement : public IATest {
public:
	using IATest::IATest;

	/// Contient la logique de la stratégie elle-même
	virtual void executer() override;

private:
	void testCarre();
	void testLigneDroite();
	void testAnglesAbsolus();
	void testAnglesRelatifs();
	void testPointerVers();
	void testAllerA();
	void testTrajectoireComplexe();
	void testTournerAbsoluDeterminationSens();
};

/**
 * IA DE TEST DU MECAMANAGER
 */
class IATestMeca : public IATest {
public:
	using IATest::IATest;

	/// Contient la logique de la stratégie elle-même
	virtual void executer() override;

private:
	// TODO: A reecrire
};

class IATestEvitement : public IATest {
public:
	using IATest::IATest;

	virtual void executer() override;

private:
	bool _running = true; // toujours !
};

#endif // IATest
