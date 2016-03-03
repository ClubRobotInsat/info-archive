L'IA envoie ses ordres sous forme de trames vers les cartes (réelles ou simu).
Dans le cas réel (pour tester par USB), on envoie les ordres sur RS232.
Pour tester l'IA dans le simu, on envoie les ordres par TCP.

Le simulateur contient un server (Communicateur) qui va écouter les trames fournies par l'IA.
Communicateur a plusieurs implémentations (TCP/IP etc...) qui sont instanciées en fonction des arguments passés.

Communicateur::Communicateur(const string& port, Robot& robot)

Communicateur::threadReception()
	C'est là que se fait la reception des messages.
	Une trame est créée et envoyée à Robot::onRecu (simulateur/Robot.hpp)
Robot::onRecu(Trame const& trame, bool isAck)
	Ici, la trame est transférée à la carte qui correspond.


Création du robot et du communicateur :
Monde2015::Monde2015()
	création du robot
Robot.hpp: RobotAnnee::RobotAnnee() :
	création du communicateur

IDeplacement, CarteDeplacement2009, Robot 
