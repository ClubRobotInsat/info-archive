//
// Created by terae on 3/22/18.
//

#ifndef ROOT_IATESTSERVO_H
#define ROOT_IATESTSERVO_H

#include "../../Commun/Robot.h"
#include "../../Principal/librobot/libRobot.h"

using namespace Commun;

enum class ServoToTest {
	SERVO_1 = 0x03,
	SERVO_2 = 0xFD,

	NBR = 2
};

class IATestServo {
public:
	IATestServo(std::string port, Duration tempo)
	        : _busCAN(std::make_unique<CAN>(std::make_unique<RS232>(port)))
	        , _carteServo(std::make_unique<CarteServosNova2017>(*_busCAN, ID_CARTE_SERVOS))
	        , getPositionServo([](uint8_t servo, uint8_t pos) -> Angle {
		        int raw;
		        std::cout << "Entrez une valeur d'angle : ";
		        std::cin >> raw;
		        Angle value = Angle::makeFromDeg(raw);
		        return value;
	        })
	        , _servos(static_cast<CarteServosNova2017&>(*_carteServo), getPositionServo) {
		_busCAN->setTemporisation(tempo);

		_reception = std::thread(&IATestServo::traiterMessage, this);
	}

	void traiterMessage() {
		setThreadName("TraiterMessage");

		while(true) {
			Trame trame = _busCAN->recevoirTrameBloquant();
			std::cout << "trame reçue : " << trame << std::endl;
		}
	}

	void executer() {
		while(1) {
			int i;
			std::cout << "Numéro du servo à tester [1|2] : ";
			std::cin >> i;
			if(i != 1 && i != 2) {
				std::cerr << "bad value" << std::endl;
				continue;
			}
			if(i == 1)
				_servos.positionnerServoBloquant((uint8_t)enumToInt(ServoToTest::SERVO_1), 0);
			else if(i == 2)
				_servos.positionnerServoBloquant((uint8_t)enumToInt(ServoToTest::SERVO_2), 0);
		}
	}

private:
	const uint8_t ID_CARTE_SERVOS = 2;

	std::unique_ptr<CAN> _busCAN;
	std::unique_ptr<Carte> _carteServo;
	std::function<Angle(uint8_t, uint8_t)> getPositionServo;
	ServoManager _servos;

	std::thread _reception;
};


#endif // ROOT_IATESTSERVO_H
