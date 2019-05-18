//
// Created by louis on 07/02/16.
//

#ifndef ROOT_SIMULATEUR_H
#define ROOT_SIMULATEUR_H

#define TARGET_SIMULATEUR

#include <atomic>

#include "../gui/IGuiClient.h"
#include "../gui/IGuiContext.h"
#include "SimuRobot.h"
#include "World.h"

enum SIMU_YEAR { MOON = 2017, CITY = 2018 };

class Simulateur : public IGuiClient {
public:
	/**
	 * Obtient un pointeur vers l'unique instance du simulateur
	 */
	static Simulateur& getInstance() {
		if(_instance == nullptr) {
			_instance = std::make_unique<Simulateur>();
		}
		return *_instance;
	}

	/**
	 * Création du simulateur : instance + serveur + monde
	 */
	Simulateur();

	/**
	 * Destrution du simulateur : fin du serveur + suppression de tous les objets
	 */
	~Simulateur();

	/**
	 * Execute la boucle principale du simulateur. Cette méthode
	 * ne retourne pas tant que le simulateur ne s'est pas fermé.
	 * */
	void start();


	/**
	 * Change de fichier JSON cible pour load toute la table et ajouter le robot.
	 * Nécessite un reset du simulateur pour être appliqué.
	 */
	void setJSONFile(const std::string&);

	/** Active ou désactive l'ajout de l'environnement dans le simulateur. Si l'environnement
	 * est désactivé, le robot évolue dans un espace vide.
	 *
	 * Ce paramètre prend effet lors d'un reset du simulateur. */
	void setWorldEnabled(bool enabled) {
		_enableWorld = enabled;
	}

	/**
	 * Change la couleur du robot. Ce paramètre sera appliqué lors d'un reset du
	 * simulateur.
	 */
	void setRobotColor(Constants::RobotColor color) {
		_robotColor = color;
	}

	/**
	 * Change le nom du robot. Ce paramètre est appliqué lors d'un reset du simulateur.
	 * Si le nom du robot vaut "off", alors aucun robot ne sera ajouté au simulateur.
	 */
	void setRobotName(const std::string& name) {
		_robotName = name;
	}

	/**
	 * Active ou désactive les collisions. Ce paramètre prend effet immédiatement.
	 * */
	void setPhysicsEnabled(bool enabled) override;


	/**
	 * Obtient un pointeur sur le monde du simu
	 */
	World* getWorld() {
		return &_theWorld;
	}

	/**
	 * Envoie une demande d'arrêt au simulateur. Celui-ci effectue
	 * les dernières opérations avant l'arrêt, puis s'arrête.
	 * */
	void requestStop();

	/**
	 * Mise à jour du monde et de l'état du robot
	 * @param time 10_ms is good
	 */
	void update(Duration time);


	/**
	 * Réinitialisation du simulateur : objets + robot
	 */
	void resetWorld();

	/**
	 * Réinitialisation du simulateur avec les données de reset.
	 * */
	void resetWorld(const ResetData& resetData) override;

	ResetData getResetData() override;

	Simu::SimuRobot& getRobot() {
		return *_robot;
	}

	void sendTextMessage(const std::string& message);

	void connect(const ConnectionData& connectionData) override;

	void createIAProcess(const IAProcessData& iaProcessData, const ConnectionData& connectionData) override;

	void testNavigationForward(Distance distance) override;

	void testNavigationTurn(Angle angle) override;

	std::vector<std::string> getRobotColors() const override;

private:
	/// Unique instance du simulateur
	static std::unique_ptr<Simulateur> _instance;

	std::unique_ptr<IGraphicalContext> _graphicalCtx;
	std::unique_ptr<IPhysicalContext> _physicalCtx;
	std::unique_ptr<IGuiContext> _guiCtx;

	friend class SimuGuiClient;

	/// Le monde dans lequel on stocke tous les objets
	World _theWorld;

	std::unique_ptr<Simu::SimuRobot> _robot;

	std::atomic_bool _simuAlive = false;


	// Parametres du simulateur

	/// Nom du robot actuel
	std::string _robotName = "off";
	/// Couleur du robot actuel
	Constants::RobotColor _robotColor = Constants::RobotColor::Undef;

	/// Indique si la physique est activée.
	bool _enablePhysics = true;

	bool _enableWorld = true;

	std::string _json_file = "";


	/**
	 * Initialise la simulation et place tous les objets de l'année en cours
	 */
	void initWorld();

	/**
	 * On arrête la simulation : suppression du robot et de tous les objets
	 */
	void destroyWorld();

	/**
	 * On ajoute notre robot sur la table
	 * @param color couleur du robot, détermine sa position initiale et sa couleur d'affichage
	 */
	void addRobot(std::string name, Constants::RobotColor color);
};

#endif // ROOT_SIMULATEUR_H
