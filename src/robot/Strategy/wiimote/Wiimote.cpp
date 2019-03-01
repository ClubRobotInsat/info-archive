//
// Created by louis on 11/09/16.
//

#include "Wiimote.h"
#include <iostream>
#include <unistd.h>
#include <wiic/wiicpp.h>

//---------------- FONCTIONS UTILITAIRES

int getButton(WiimoteButton button) {
	switch(button) {
		case BUTTON_DOWN:
			return CButtons::BUTTON_DOWN;
		case BUTTON_LEFT:
			return CButtons::BUTTON_LEFT;
		case BUTTON_RIGHT:
			return CButtons::BUTTON_RIGHT;
		case BUTTON_UP:
			return CButtons::BUTTON_UP;
		case BUTTON_A:
			return CButtons::BUTTON_A;
		case BUTTON_B:
			return CButtons::BUTTON_B;
		case BUTTON_1:
			return CButtons::BUTTON_ONE;
		case BUTTON_2:
			return CButtons::BUTTON_TWO;
		case BUTTON_HOME:
			return CButtons::BUTTON_HOME;
		case BUTTON_PLUS:
			return CButtons::BUTTON_PLUS;
		case BUTTON_MINUS:
			return CButtons::BUTTON_MINUS;
	}
	return 0;
}


//----------------

WiimoteInputProvider::WiimoteInputProvider() {}

void WiimoteInputProvider::registerEventHandler(WiimoteEventHandler* handler) {
	this->handler = handler;
}

void WiimoteInputProvider::startListening() {
	std::cout << "Connexion d'une wiimote requise :" << std::endl;

	CWii wii;

	// Cherche des wiimotes pendant 5 secondes par bluetooth
	// system("rfkill unblock bluetooth");
	int numFound = wii.Find(5);
	std::cout << "\033[3mSi le bluetooth est desactive (erreur : 'hci_get_route: No such device'), taper la commande : "
	             "\x1B[34m'rfkill unblock bluetooth\x1B[0m'"
	          << std::endl;

	if(numFound == 0) {
		std::cout << "Aucune wiimote détectée" << std::endl;
		return;
	}

	std::vector<CWiimote>& wiimotes = wii.Connect();

	if(wiimotes.size() == 0) {
		std::cout << "Impossible de se connecter à une wiimote" << std::endl;
		return;
	}

	std::cout << "Connexion à la première wiimote trouvée" << std::endl;

	CWiimote& wiimote = wiimotes[0];

	// On indique que la wiimote est connectée par un signal lumineux.
	wiimote.SetLEDs(CWiimote::LED_1);
	wiimote.SetRumbleMode(CWiimote::ON);
	usleep(200000);
	wiimote.SetRumbleMode(CWiimote::OFF);

	bool running = true;

	std::cout << "\nLancement du pilotage\n\n" << std::endl;

	do {
		if(wii.Poll()) {
			WiimoteState state(wiimote);

			switch(wiimote.GetEvent()) {
				case CWiimote::EVENT_EVENT:
					this->handler->processWiimoteInput(state);
					break;
				case CWiimote::EVENT_NUNCHUK_INSERTED:
					std::cout << "Insertion d'un nunchuk détectée" << std::endl;
					break;
				case CWiimote::EVENT_NUNCHUK_REMOVED:
					std::cout << "Déconnexion du nunchuk" << std::endl;
					break;
				case CWiimote::EVENT_DISCONNECT:
				case CWiimote::EVENT_UNEXPECTED_DISCONNECT:
					std::cout << "Deconnexion de la wiimote." << std::endl;
					running = false;
					break;
				default:
					break;
			}
		}
	} while(running);
}

//--------------- WiimoteState

WiimoteState::WiimoteState(CWiimote& wiimote) : wiimote(wiimote) {}

bool WiimoteState::isJustPressed(WiimoteButton button) {
	int wiicButton = getButton(button);

	if(wiicButton != 0) {
		return wiimote.Buttons.isJustPressed(wiicButton);
	}

	return false;
}

bool WiimoteState::isPressed(WiimoteButton button) {
	int wiicButton = getButton(button);

	if(wiicButton != 0) {
		return wiimote.Buttons.isPressed(wiicButton);
	}

	return false;
}

bool WiimoteState::hasNunchuk() {
	return wiimote.ExpansionDevice.GetType() == wiimote.ExpansionDevice.TYPE_NUNCHUK;
}

bool WiimoteState::getNunchukJoystickPosition(float& angle, float& magnitude) {
	if(!hasNunchuk())
		return false;

	CNunchuk& nunchuk = wiimote.ExpansionDevice.Nunchuk;
	nunchuk.Joystick.GetPosition(angle, magnitude);

	return true;
}
