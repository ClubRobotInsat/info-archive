//
// Created by louis on 10/09/16.
//

#include "IAWiimote.h"
#include "../petri/Deplacement.h"
#include "../petri/Moteur.h"
#include "../petri/Servo.h"

//#define TEST_FCT_DEPLA

#define MANETTE_COUCHEE

int main(int argc, char* argv[]) {
	Log::open(argc, argv, false);

	logDebug6("Initialisation du robot...Veuillez patienter...");
	IAWiimote strategie({argv, argv + argc});

	strategie.demarrer(10000_s);
}

IAWiimote::IAWiimote(std::vector<std::string> const& args)
        : StrategiePrincipal(args, Constantes::RobotColor::Orange), typeDeplacement(TypeDeplacement::IMMOBILE) {

	// On donne une vitesse angulaire pas trop grande dès le début
	setVitesseAngulaire(getDeplacement().getVitesseAngulaire() * 0.5);

	inputProvider.registerEventHandler(this);

	getDeplacement().setRepere(Coordonnees{_initial_position, _initial_angle});
}

void IAWiimote::executer() {
	inputProvider.startListening();

	logDebug("Fin de la connexion avec les wiimotes.");

#ifdef TEST_FCT_DEPLA
	avancerDe(20_cm, 5_s);

/*avancer(SensAvance::Avant);
usleep(1000000);
stop();

//tournerRelatif(90_deg, 0.01_ms);
logDebug("J'ai tourné !");
usleep(10000000);
logDebug("Je me suis arrêté !");*/
#else
	exit(0);
#endif
}

void IAWiimote::processWiimoteInput(WiimoteState& state) {
	// std::cout << "\n";
	// déplacement
	if(state.hasNunchuk()) {

		state.getNunchukJoystickPosition(angleLu, magnitudeLue);

		if(magnitudeLue > 0.2) {
			switch(getTypeDeplacement(angleLu)) {
				case AVANCER:
					avancer(SensAvance::Avant, magnitudeLue);
					break;
				case AVANCER_DROITE:
					// avancerCourbe(SensAvance::Avant, SensRotation::Horaire, magnitudeLue);
					break;
				case TOURNER_DROITE:
					tourner(SensRotation::Horaire, magnitudeLue);
					break;
				case RECULER_DROITE:
					// avancerCourbe(SensAvance::Arriere, SensRotation::Trigo, magnitudeLue);
					break;
				case RECULER:
					avancer(SensAvance::Arriere, magnitudeLue);
					break;
				case RECULER_GAUCHE:
					// avancerCourbe(SensAvance::Arriere, SensRotation::Horaire, magnitudeLue);
					break;
				case TOURNER_GAUCHE:
					tourner(SensRotation::Trigo, magnitudeLue);
					break;
				case AVANCER_GAUCHE:
					// avancerCourbe(SensAvance::Avant, SensRotation::Trigo, magnitudeLue);
					break;
				default:
					stop();
					break;
			}
		} else {
			stop();
		}
	} else {
// Soit on tient la manette comme dans mario kart, soit comme dans Wii Sport Resort
#ifdef MANETTE_COUCHEE
		if(state.isPressed(BUTTON_UP)) {
			tourner(SensRotation::Horaire);
		} else if(state.isPressed(BUTTON_LEFT)) {
			avancer(SensAvance::Arriere);
		} else if(state.isPressed(BUTTON_RIGHT)) {
			avancer(SensAvance::Avant);
		} else if(state.isPressed(BUTTON_DOWN)) {
			tourner(SensRotation::Trigo);
		} else {
			stop();
		}
#else
		if(state.isPressed(BUTTON_UP)) {
			avancer(SensAvance::Arriere);
		} else if(state.isPressed(BUTTON_LEFT)) {
			tourner(SensRotation::Trigo);
		} else if(state.isPressed(BUTTON_RIGHT)) {
			tourner(SensRotation::Horaire);
		} else if(state.isPressed(BUTTON_DOWN)) {
			avancer(SensAvance::Avant);
		} else {
			stop();
		}
#endif

// ascenseurs
#ifdef MANETTE_COUCHEE
		if(state.isPressed(BUTTON_B) || state.isPressed(BUTTON_PLUS)) {
			if (_soutes_ouvertes) {
				fermerSouteG();
				fermerSouteD();
			} else {
				ouvrirSouteD();
				ouvrirSouteG();
			}

			_soutes_ouvertes = !_soutes_ouvertes;
			//monterAscenseursDe(1);
		} else if(state.isPressed(BUTTON_A) || state.isPressed(BUTTON_MINUS)) {
#else
		if(state.isPressed(BUTTON_1)) {
			monterAscenseursDe(1);
		} else if(state.isPressed(BUTTON_2)) {
#endif
			if(_abeille_ouverte) {
				fermerAbeille();
			} else {
				ouvrirAbeille();
			}
			_abeille_ouverte = !_abeille_ouverte;
		} else if(state.isPressed(BUTTON_HOME)) {
			if (_porte_ouverte) {
				fermerPorteCube();
			} else {
				ouvrirPorteCube();
			}
			_porte_ouverte = !_porte_ouverte;
		}
		/*else
		    // On revient à la position initiale
		    if(state.isPressed(BUTTON_HOME)) {
			allerA_vec(_initial_position);
			tournerAbsolu(_initial_angle);
			std::cout << "Home sweet home" << std::endl;
		} else if(state.isPressed(BUTTON_PLUS)) { // Modifie la vitesse de manière permanente (en plus de 1 et 2)
			setVitesseLineaireRapide();
			setVitesseAngulaireRapide();
		} else if(state.isPressed(BUTTON_MINUS)) {
			setVitesseLineaireLente();
			setVitesseAngulaireLente();
		}*/ else

// accélération - décélération
#ifdef MANETTE_COUCHEE
		    if(state.isPressed(BUTTON_2)) {
#else
		    if(state.isPressed(BUTTON_A)) {
#endif
			if(!linearVelocityVeryFast) {
				if(!linearVelocityFast) {
					setVitesseLineaire(getDeplacement().getVitesseLineaire() * 4);
					linearVelocityVeryFast = true;
					linearVelocityFast = true;
				} else {
					setVitesseLineaire(getDeplacement().getVitesseLineaire() * 2);
					linearVelocityVeryFast = true;
				}
			}

			if(!angularVelocityVeryFast) {
				if(!angularVelocityFast) {
					setVitesseAngulaire(getDeplacement().getVitesseAngulaire() * 4);
					angularVelocityVeryFast = true;
					angularVelocityFast = true;
				} else {
					setVitesseAngulaire(getDeplacement().getVitesseAngulaire() * 2);
					angularVelocityVeryFast = true;
				}
			}
#ifdef MANETTE_COUCHEE
		} else if(state.isPressed(BUTTON_1)) {
#else
		} else if(state.isPressed(BUTTON_B)) {
#endif
			if(linearVelocityVeryFast) {
				setVitesseLineaire(getDeplacement().getVitesseLineaire() * 0.25);
				linearVelocityVeryFast = false;
				linearVelocityFast = false;
			} else if(linearVelocityFast) {
				setVitesseLineaire(getDeplacement().getVitesseLineaire() * 0.5);
				linearVelocityFast = false;
			}

			if(angularVelocityVeryFast) {
				setVitesseAngulaire(getDeplacement().getVitesseAngulaire() * 0.25);
				angularVelocityVeryFast = false;
				angularVelocityFast = false;
			} else if(angularVelocityFast) {
				setVitesseAngulaire(getDeplacement().getVitesseAngulaire() * 0.5);
				angularVelocityFast = false;
			}
		} else {
			if(linearVelocityVeryFast) {
				setVitesseLineaire(getDeplacement().getVitesseLineaire() * 0.5);
				linearVelocityVeryFast = false;
				linearVelocityFast = true;
			} else if(!linearVelocityFast) {
				setVitesseLineaire(getDeplacement().getVitesseLineaire() * 2);
				linearVelocityVeryFast = false;
				linearVelocityFast = true;
			}
			if(angularVelocityVeryFast) {
				setVitesseAngulaire(getDeplacement().getVitesseAngulaire() * 0.5);
				angularVelocityVeryFast = false;
				angularVelocityFast = true;
			} else if(!angularVelocityFast) {
				setVitesseAngulaire(getDeplacement().getVitesseAngulaire() * 2);
				angularVelocityVeryFast = false;
				angularVelocityFast = true;
			}
		}
	}
}

void IAWiimote::avancer(SensAvance sens, float magnitude) {
	TypeDeplacement typeDep = sens == SensAvance::Avant ? AVANCER : RECULER;

	if(typeDeplacement != IMMOBILE && typeDeplacement != typeDep) {
		stop();
	}
	if(typeDeplacement == IMMOBILE) {
		getDeplacement().avancerInfini(sens, 1_ms);
		magnitudeOld = magnitude;
	} else if(!linearVelocityFast && magnitude >= 0.8) {
		setVitesseLineaire(getDeplacement().getVitesseLineaire() * 2);
		linearVelocityFast = true;
	} else if(linearVelocityFast && magnitude < 0.8) {
		setVitesseLineaire(getDeplacement().getVitesseLineaire() * 0.5);
		linearVelocityFast = false;
	}

	typeDeplacement = typeDep;
}

void IAWiimote::tourner(SensRotation sens, float magnitude) {
	TypeDeplacement typeDep = sens == SensRotation::Horaire ? TOURNER_DROITE : TOURNER_GAUCHE;

	if(typeDeplacement != IMMOBILE && typeDeplacement != typeDep) {
		stop();
	}
	if(typeDeplacement == IMMOBILE) {
		getDeplacement().tournerRelatif(sens == SensRotation::Trigo ? 179_deg : -179_deg, 1_ms);
		std::cout << "tourner relatif" << std::endl;
		magnitudeOld = magnitude;
	} else if(!angularVelocityFast && magnitude >= 0.8) {
		setVitesseAngulaire(getDeplacement().getVitesseAngulaire() * 2);
		angularVelocityFast = true;
	} else if(angularVelocityFast && magnitude < 0.8) {
		setVitesseAngulaire(getDeplacement().getVitesseAngulaire() * 0.5);
		angularVelocityFast = false;
	}

	typeDeplacement = typeDep;
}

void IAWiimote::stop() {
	std::cout << "stop !" << std::endl;
	getDeplacement().arretUrgence();
	typeDeplacement = IMMOBILE;
}

void IAWiimote::setVitesseLineaire(Speed linearVelocity) {
	if(linearVelocityPushed) {
		getDeplacement().popVitesseLineaire();
	}
	getDeplacement().pushVitesseLineaire(linearVelocity);
	linearVelocityPushed = true;
}

void IAWiimote::setVitesseAngulaire(AngularSpeed angularVelocity) {
	if(angularVelocityPushed) {
		getDeplacement().popVitesseAngulaire();
	}
	getDeplacement().pushVitesseAngulaire(angularVelocity);
	angularVelocityPushed = true;
}

TypeDeplacement IAWiimote::getTypeDeplacement(float angleNunchuk) {
#ifdef NUNCHUK_UTILISER_COURBES
	if(angleNunchuk >= 337.5 || angleNunchuk <= 22.5)
		return AVANCER;
	for(int i = 1; i <= 15; i += 2) {
		if((angleNunchuk >= i * 22.5) && (angleNunchuk <= (i + 2) * 22.5))
			return (TypeDeplacement)((i + 1) / 2);
	}


#else
	if(angleNunchuk >= 315 || angleNunchuk <= 45)
		return AVANCER;
	if(angleNunchuk > 45 && angleNunchuk < 135)
		return TOURNER_DROITE;
	if(angleNunchuk >= 135 && angleNunchuk <= 225)
		return RECULER;
	if(angleNunchuk > 225 && angleNunchuk < 315)
		return TOURNER_GAUCHE;

#endif // NUNCHUK_UTILISER_COURBES
	return IMMOBILE;
}

#undef MANETTE_COUCHEE