//
// Created by louis on 11/09/16.
//

#ifndef ROOT_WIIMOTEUSER_H
#define ROOT_WIIMOTEUSER_H

#include <memory>

class CWiimote;

class WiimoteInputProvider;
class WiimoteEventHandler;
class WiimoteState;


class WiimoteInputProvider {
public:
	WiimoteInputProvider();

	void registerEventHandler(WiimoteEventHandler* handler);
	/** Boucle principale d'une application utilisant les entrées wiimote. */
	void startListening();

private:
	WiimoteEventHandler* handler;
};

class WiimoteEventHandler {
public:
	virtual void processWiimoteInput(WiimoteState& state) = 0;
	virtual ~WiimoteEventHandler() = default;
};

enum WiimoteButton {
	BUTTON_UP,
	BUTTON_RIGHT,
	BUTTON_DOWN,
	BUTTON_LEFT,
	BUTTON_A,
	BUTTON_B,
	BUTTON_1,
	BUTTON_2,
	BUTTON_HOME,
	BUTTON_PLUS,
	BUTTON_MINUS,
};

/**
 * Classe faisant l'interface entre l'EventHandler et les méthodes
 * de la wiimote telles que Wiimote::isJustPressed(), Wiimote::isPressed()...
 */
class WiimoteState {
public:
	explicit WiimoteState(CWiimote& wiimote);

	bool isJustPressed(WiimoteButton button);
	bool isPressed(WiimoteButton button);
	bool hasNunchuk();

	bool getNunchukJoystickPosition(float& angle, float& magnitude);

private:
	CWiimote& wiimote;
};

#endif // ROOT_WIIMOTEUSER_H
