//
// Created by louis on 21/12/17.
//

#ifndef ROOT_IWINDOWLISTENER_H
#define ROOT_IWINDOWLISTENER_H

/** Cette interface permet à un module de GUI de transmettre les
 * actions de l'utilisateur aux autres modules du simulateur. */
class IGraphicalUserListener {
public:
	virtual ~IGraphicalUserListener() = default;

	virtual void onExit() = 0;
};


#endif // ROOT_IWINDOWLISTENER_H
