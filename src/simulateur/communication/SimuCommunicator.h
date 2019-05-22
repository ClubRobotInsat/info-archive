#ifndef ROOT_SIMUCOMMUNICATOR_H
#define ROOT_SIMUCOMMUNICATOR_H

#include <memory>
#include <thread>

#include "../../robot/Communication/Protocol.h"
#include "../../robot/Modules/ModuleManager.h"

class SimuCommunicator {
public:
	explicit SimuCommunicator(const std::shared_ptr<PhysicalRobot::ModuleManager>& moduleMgr);

	~SimuCommunicator();

	/** Change le protocole actuel de la communication, et
	 * reboot la communication. */
	void connect(std::unique_ptr<Communication::Protocol>&& protocol);

	void disconnect();

private:
	std::unique_ptr<Communication::Protocol> _protocol;

	/// Le module manager qui contient les différents modules
	std::shared_ptr<PhysicalRobot::ModuleManager> _moduleMgr;

	std::atomic_bool _connected;
	std::thread _communicationThread;


	void communicationThread();
};

#endif // ROOT_SIMUCOMMUNICATOR_H
