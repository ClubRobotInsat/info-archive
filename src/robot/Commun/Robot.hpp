#include <csignal>
#include <cstdlib>
#include <stdexcept>
#include <string>

namespace Commun {

	/// Retourne un pointeur vers la carte d'ID donne
	template <typename _Strategie, typename _IDCartes, template <_IDCartes> class _CarteInfo, typename Contacteurs>
	Carte& RobotAnnee<_Strategie, _IDCartes, _CarteInfo, Contacteurs>::getCarte(_IDCartes id) {
		auto it = _idMap.find(enumToInt(id));

		if(it != _idMap.end()) {
			return this->getCarteWithID(it->second);
		}

		logError("Demande de la carte ", id, ", cette carte n'existe pas !\n");
		throw std::runtime_error("La carte n'existe pas !");
	}

	template <typename _Strategie, typename _IDCartes, template <_IDCartes> class _CarteInfo, typename Contacteurs>
	bool RobotAnnee<_Strategie, _IDCartes, _CarteInfo, Contacteurs>::hasCarte(_IDCartes id) {
		auto it = _idMap.find(enumToInt(id));
		return it != _idMap.end();
	}

	template <typename _Strategie, typename _IDCartes, template <_IDCartes> class _CarteInfo, typename Contacteurs>
	void RobotAnnee<_Strategie, _IDCartes, _CarteInfo, Contacteurs>::synchroniserCAN() {
		this->getCarte<_IDCartes::CAN_USB>().reinitialiserNumSequence();

		/*while(!this->getCarte<_IDCartes::CAN_USB>().isInitialiseNumSequence()) {
		    sleep(50_ms);
		    logWarn("La carte CAN-USB ne s'est pas synchronisée");
		}*/
		this->getCAN().reinitialiserNumSequence();
		this->getCAN().reinitialiserFenetre();
		logSuccess("Le CAN est synchronisé, on est partis !");
	}

	template <typename _Strategie, typename _IDCartes, template <_IDCartes> class _CarteInfo, typename Contacteurs>
	bool RobotAnnee<_Strategie, _IDCartes, _CarteInfo, Contacteurs>::pinguerCartes() {
		bool allOK = true;
		int i = 0;
		for(auto e : getEnumValues<_IDCartes>()) {

			if(this->hasCarte(e) && this->getCarte(e).pingPong()) {
				logDebug("[", e, "] OK !");
				i++;
			} else {
				logError("[", e, "] ERROR no PONG !");
				allOK = false;
			}
		}
		if(allOK || i >= 7) {
			return true;
		} else {
			logError("Une ou plusieurs cartes n'ont pas ping correctement !");
			return false;
		}
	}

	template <typename _Strategie, typename _IDCartes, template <_IDCartes> class _CarteInfo, typename Contacteurs>
	void RobotAnnee<_Strategie, _IDCartes, _CarteInfo, Contacteurs>::pinguerCAN() {
		this->getCarte<_IDCartes::CAN_USB>().envoyerPing();

		do {
			logInfo("Attente du pong de la carte CAN-USB");
			sleep(50_ms);
		} while(!this->getCarte<_IDCartes::CAN_USB>().verifierPong());

		logInfo("La carte CAN-USB a pongué");
	}

	// Attend la tirette au départ
	template <typename _Strategie, typename _IDCartes, template <_IDCartes> class _CarteInfo, typename Contacteurs>
	void RobotAnnee<_Strategie, _IDCartes, _CarteInfo, Contacteurs>::attenteTirette() {
		int etatTirette = 0;
		miseEnPlaceTirette();

		while(etatTirette < Robot::NB_RETRY_TIRETTE) {
			if(tiretteTiree()) {
				++etatTirette;
				logDebug5(etatTirette);
			} else
				etatTirette = 0;
			sleep(100_ms);
		}
	}

	// Utilitaire : attente de mise de la tirette
	template <typename _Strategie, typename _IDCartes, template <_IDCartes> class _CarteInfo, typename Contacteurs>
	void RobotAnnee<_Strategie, _IDCartes, _CarteInfo, Contacteurs>::miseEnPlaceTirette() {
		logInfo("Attente que l'on mette la tirette");

		while(this->getCarte<_IDCartes::IO>().actualiserPuisLireUnCapteur(enumToInt(Contacteurs::TIRETTE))) {
			sleep(100_ms);
		}

		logInfo("Tirette mise, tirer sur la tirette plz !");
	}

	// Utilitaire : vérification de tirette tirée
	template <typename _Strategie, typename _IDCartes, template <_IDCartes> class _CarteInfo, typename Contacteurs>
	bool RobotAnnee<_Strategie, _IDCartes, _CarteInfo, Contacteurs>::tiretteTiree() {
		if(this->getCarte<_IDCartes::IO>().actualiserPuisLireUnCapteur(enumToInt(Contacteurs::TIRETTE))) {
			logDebug6("Tirette detectée comme tirée");
			return true;
		} else {
			return false;
		}
	}

	template <typename _Strategie, typename _IDCartes, template <_IDCartes> class _CarteInfo, typename Contacteurs>
	void RobotAnnee<_Strategie, _IDCartes, _CarteInfo, Contacteurs>::setEnvoiPositionAdversaireAuto(bool value) {
		if(value)
			this->getCarte<_IDCartes::EVITEMENT>().activerEnvoiMesureAdvAuto();
		else
			this->getCarte<_IDCartes::EVITEMENT>().desactiverEnvoiMesureAdvAuto();
	}

	template <typename _Strategie, typename _IDCartes, template <_IDCartes> class _CarteInfo, typename Contacteurs>
	Angle RobotAnnee<_Strategie, _IDCartes, _CarteInfo, Contacteurs>::getAngleAdversaireDetecte() {
		Angle angleAdv = this->getCarte<_IDCartes::EVITEMENT>().getAngle();
		if(abs(angleAdv) > 0.001_rad) {
			return (angleAdv + this->constantes().getOffsetAngleAdv());
		} else {
			return 0_rad;
		}
	}

	template <typename _Strategie, typename _IDCartes, template <_IDCartes> class _CarteInfo, typename Contacteurs>
	template <_IDCartes ID, typename... T>
	void RobotAnnee<_Strategie, _IDCartes, _CarteInfo, Contacteurs>::assignerCarteHelper(T... params) {
		_idMap[enumToInt(ID)] = _CarteInfo<ID>::idCarte;

		this->ajouterCarte(std::make_unique<typename _CarteInfo<ID>::typeCarte>(this->getCAN(), _CarteInfo<ID>::idCarte, params...));
	}
}
