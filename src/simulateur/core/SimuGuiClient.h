#ifndef ROOT_SIMUGUICLIENT_H
#define ROOT_SIMUGUICLIENT_H

#include "../gui/IGuiClient.h"

class Simulateur;

/** Extension de la classe Simulateur gérant les interactions
 * avec la GUI. */
class SimuGuiClient : public IGuiClient {
public:
	explicit SimuGuiClient(Simulateur& simulator);
	~SimuGuiClient() override;

	void connect(const ConnectionData& connectionData) override;

	void createIAProcess(const IAProcessData& iaProcessData, const ConnectionData& connectionData) override;

private:
	Simulateur& _simu;
};


#endif // ROOT_SIMUGUICLIENT_H
