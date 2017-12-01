//
//  ConstantesRobot.h
//  Club Robot
//
//  Created by Rémi on 07/06/2015.
//

#ifndef Club_Robot_Constantes_h
#define Club_Robot_Constantes_h

#include <Units.h>

#include "MathToolbox/MathToolbox.h"

namespace Commun {
	struct ConstantesRobot {
		virtual Duration getTimeoutMoveDefault() const = 0;
		virtual Duration getTimeoutOpponentBlockingDefault() const = 0;
		virtual Speed getLinearSpeedDefault() const = 0;
		virtual AngularSpeed getAngularSpeedDefault() const = 0;
		virtual Distance getLinearPrecisionDefault() const = 0;
		virtual Angle getAngularPrecisionDefault() const = 0;

		virtual Distance getRayRotation() const = 0;
		virtual Vector3m getRobotSize() const = 0;
		virtual Angle getOffsetAngleAdv() const = 0;

		virtual uint16_t getPortTCPIPDefault() const = 0;

		virtual int getIDRobot() const = 0;
	};

	struct ConstantesCommunes {
		virtual int getNombreBlocageLogicielMax() const = 0;
		virtual int getNombreBlocagePhysiqueMax() const = 0;

		virtual Duration getMatchDuration() const = 0;

		virtual Vector2m getTableSize() const = 0;
		virtual Vector2u16 getTailleGrille() const = 0;
	};
}

#endif
