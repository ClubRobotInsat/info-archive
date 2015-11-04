//
//  ConstantesRobot.h
//  Club Robot
//
//  Created by Rémi on 07/06/2015.
//

#ifndef Club_Robot_Constantes_h
#define Club_Robot_Constantes_h

#include <Units/Units.h>
#include <MathToolbox/MathToolbox.h>

namespace Commun {
	struct ConstantesRobot {
		virtual Duree getTimeoutDeplacementDefaut() const = 0;
		virtual Duree getTimeoutBlocageAdversaireDefaut() const = 0;
		virtual VitesseLineaire getVitesseLineaireDefaut() const = 0;
		virtual VitesseAngulaire getVitesseAngulaireDefaut() const = 0;
		virtual Distance getPrecisionLineaireDefaut() const = 0;
		virtual Angle getPrecisionAngulaireDefaut() const = 0;

		virtual Distance getRayonRotation() const = 0;
		virtual Vector3m getTailleRobot() const = 0;
		virtual Angle getOffsetAngleAdv() const = 0;

		virtual uint16_t getPortTCPIPDefaut() const = 0;

		virtual int getIDRobot() const = 0;
	};

	struct ConstantesCommunes {
		virtual int getNombreBlocageLogicielMax() const = 0;
		virtual int getNombreBlocagePhysiqueMax() const = 0;

		virtual Duree getDureeMatch() const = 0;

		virtual Vector2m getTailleTable() const = 0;
		virtual Vector2u16 getTailleGrille() const = 0;
	};
}

#endif
