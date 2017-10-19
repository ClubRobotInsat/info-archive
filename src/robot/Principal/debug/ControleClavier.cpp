// ControleClavier.cpp - contrôle de Titan par le clavier grâce à une fenêtre GLFW

#include <iostream>
#include <string>

#include "../../../commun/GLFW/include/GL/glfw.h"

#define WIN_WIDTH 100
#define WIN_HEIGHT 100

#include "../../../commun/Commun.h"
#include "../librobot/libRobot.h"
#include <memory>

// Variables globales :
namespace {
	std::unique_ptr<RobotPrincipal> robot;
	std::unique_ptr<MecaManagerPrincipal> meca;
}

GLFWCALL void clavier(int touche, int etat);


// Creation de la fenetre
static void init() {
	logInfo("Création de la fenêtre");

	glfwInit();

	if(!glfwOpenWindow(WIN_WIDTH, WIN_HEIGHT, 0, 0, 0, 0, 16, 0, GLFW_WINDOW)) {
		logError("Impossible d'ouvrir une fenêtre !");
		glfwTerminate();
		exit(-1);
	}
	glfwSetWindowTitle("Contrôle clavier");
}

// Gestion des touches du clavier
GLFWCALL void clavier(int touche, int etat) {
	int enDeplacement;

	switch(etat) {
		case GLFW_PRESS:
			switch(touche) {
				case GLFW_KEY_UP:
					robot->getCarte<DEPLACEMENT>().avancerInfini(600_mm_s, SensAvance::Avant);
					enDeplacement = true;
					break;
				case GLFW_KEY_DOWN:
					robot->getCarte<DEPLACEMENT>().avancerInfini(600_mm_s, SensAvance::Arriere);
					enDeplacement = true;
					break;
				case GLFW_KEY_RIGHT:
					robot->getCarte<DEPLACEMENT>().tournerInfini(4_rad_s, SensRotation::Horaire);
					enDeplacement = true;
					break;
				case GLFW_KEY_LEFT:
					robot->getCarte<DEPLACEMENT>().tournerInfini(4_rad_s, SensRotation::Trigo);
					enDeplacement = true;
					break;

					// BRAS HAUT GAUCHE
					/*
					case 'Q':
					    meca->positionnerServoBras(SERVO_BRAS_G_HAUT, POSITION_BRAS_RANGE);
					    break;
					case 'W':
					    meca->positionnerServoBras(SERVO_BRAS_G_HAUT, POSITION_BRAS_SOUFFLE_BOUGIE);
					    break;
					case 'E':
					    meca->positionnerServoBras(SERVO_BRAS_G_HAUT, POSITION_BRAS_HAUT);
					    break;

					    // BRAS BAS GAUCHE
					case 'A':
					    meca->positionnerServoBras(SERVO_BRAS_G_BAS, POSITION_BRAS_RANGE);
					    break;
					case 'S':
					    meca->positionnerServoBras(SERVO_BRAS_G_BAS, POSITION_BRAS_SOUFFLE_BOUGIE);
					    break;
					case 'D':
					    meca->positionnerServoBras(SERVO_BRAS_G_BAS, POSITION_BRAS_HAUT);
					    break;

					    // BRAS HAUT DROIT
					case 'P':
					    meca->positionnerServoBras(SERVO_BRAS_D_HAUT, POSITION_BRAS_RANGE);
					    break;
					case 'O':
					    meca->positionnerServoBras(SERVO_BRAS_D_HAUT, POSITION_BRAS_SOUFFLE_BOUGIE);
					    break;
					case 'I':
					    meca->positionnerServoBras(SERVO_BRAS_D_HAUT, POSITION_BRAS_HAUT);
					    break;

					case ';':
					    meca->positionnerServoBras(SERVO_BRAS_D_BAS, POSITION_BRAS_RANGE);
					    break;
					case 'L':
					    meca->positionnerServoBras(SERVO_BRAS_D_BAS, POSITION_BRAS_SOUFFLE_BOUGIE);
					    break;
					case 'K':
					    meca->positionnerServoBras(SERVO_BRAS_D_BAS, POSITION_BRAS_HAUT);
					    break;


					    // ASCENSEUR GAUCHE
					case 'B':
					    meca->posAscGaucheBloquant(ASC_BAS);
					    break;
					case 'G':
					    meca->positionnerAscGauche(ASC_MAGIC);
					    break;
					case 'T':
					    meca->positionnerAscGauche(ASC_HAUT);
					    break;

					    // ASCENSEUR DROIT
					case 'N':
					    meca->posAscDroiteBloquant(ASC_BAS);
					    break;
					case 'H':
					    meca->positionnerAscDroite(ASC_MAGIC);
					    break;
					case 'Y':
					    meca->positionnerAscDroite(ASC_HAUT);
					    break;


					    // PINCE ASC
					case 'Z':
					    meca->positionnerServoAsc(SERVO_G_ASC_G, POSITION_PINCE_ASC_BLOCAGE);
					    meca->positionnerServoAsc(SERVO_G_ASC_D, POSITION_PINCE_ASC_BLOCAGE);
					    break;
					case 'X':
					    meca->positionnerServoAsc(SERVO_G_ASC_G, POSITION_PINCE_ASC_OUVERTE);
					    meca->positionnerServoAsc(SERVO_G_ASC_D, POSITION_PINCE_ASC_OUVERTE);
					    break;
					case 'C':
					    meca->positionnerServoAsc(SERVO_D_ASC_G, POSITION_PINCE_ASC_BLOCAGE);
					    meca->positionnerServoAsc(SERVO_D_ASC_D, POSITION_PINCE_ASC_BLOCAGE);
					    break;
					case 'V':
					    meca->positionnerServoAsc(SERVO_D_ASC_G, POSITION_PINCE_ASC_OUVERTE);
					    meca->positionnerServoAsc(SERVO_D_ASC_D, POSITION_PINCE_ASC_OUVERTE);
					    break;
					    */
			}
			break;

		case GLFW_RELEASE:
			robot->getCarte<DEPLACEMENT>().arretUrgence();
			break;

		default:
			robot->getCarte<DEPLACEMENT>().arretUrgence();
			break;
	}
}

int main(int argc, char* argv[]) {
	printf("Contrôle clavier :\n");
	printf("===================================\n");
	printf("- flèches directionnelles : déplacer le robot\n");
	printf("- A/Z/E : Position bras gauche-haut\n");
	printf("- Q/S/D : Position bras gauche-bas\n");
	printf("- P/O/I : Position bras droite-haut\n");
	printf("- M/L/K : Position bras droite-bas\n\n");
	printf("- B/G/T : Position Ascenseur gauche\n");
	printf("- N/H/Y : Position Ascenseur droite\n\n");
	printf("- W/X : Position Pince Gauche\n");
	printf("- C/V : Position Pince Droite\n");
	printf("===================================\n");

	bool running = true;

	logInfo("Création du robot");
	robot = std::make_unique<RobotPrincipal>(std::vector<std::string>({argv, argv + argc}));

	logInfo("Création du MecaManager");
	meca = std::make_unique<MecaManagerPrincipal>(*robot);

	init();

	glfwSetKeyCallback(clavier);

	while(running) {
		glfwSwapBuffers();
		running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
		sleep(20_ms);
	}

	glfwTerminate();


	return 0;
}
