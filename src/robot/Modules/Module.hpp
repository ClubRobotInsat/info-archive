/**
 * Ce fichier est la base pour créer des modules électroniques
 * Ceux-ci s'appliquent à la stratégie post-2018 dans laquelle tout l'état du robot est regroupé
 *
 * La construction de chaque module spéficique (moteurs, servos, déplacement, capteurs, ...)
 * s'appuie sur la mise en commun avec les élecs d'une structure de donnée JSON qui est
 * utilisée pour la communication des datas entre les deux parties
 *
 * Enfin, la cohésion entre ces modules est gérée par la classe 'ModuleManager'.
 *
 * Pour créer un nouveau module, il faut :
 *  === dans une nouvelle classe '{NOM_DU_MODULE}{ANNEE}.[cpp|h]' ===
 *   - faire hériter le nouveau module de la classe mère 'Module'
 *   - overrider les fonctions 'generate_json' et 'message_processing' pour parser le format JSON
 *   - overrider la fonction 'deactivation'
 *  === rajouter la ligne `#include "{NOM_DU_MODULE}{ANNEE}.h"` dans `ModuleManager.h` et dans le `CMakeLists.txt`
 *  === idéalement, écrire des tests unitaires (`/test/unit-modules.cpp`) et de la doc =)
 *
 * Et c'est tout ! Pour plus de détails, tu peux te référer aux modules déjà codés.
 */

#ifndef _MODULE_H
#define _MODULE_H

#include <Commun.h> // GlobalFrame
#include <Constants.h>
#include <log/Log.h>

#include <atomic>     // atomic
#include <functional> // function
#include <mutex>      // mutex, lock_guard

namespace PhysicalRobot {

	class Module {
	public:
		/// Initialisation du module
		Module(uint8_t id, std::string module_name) : name(module_name), _state_changed(false), _id(id) {}

		virtual ~Module() = default;

		/// Retourne l'ID du module dans le robot
		inline uint8_t get_id() {
			return _id;
		}

		/// Retourne vrai si l'état du module nécessite d'être partagé avec les élecs
		/// i.e. si une modification informatique a été apportée, ou si le timer a expiré
		bool needs_to_be_shared() const {
			return _state_changed || _timer.getElapsedTime() >= GLOBAL_CONSTANTS().get_frame_period();
		}

		/// On a prit en compte la nouvelle trame pour l'envoyer aux élecs
		void reset_state() {
			_state_changed.exchange(false);
			_timer.reset();
		}

		/// Traite les données fournies par le robot (thread-safe)
		// Le gros du travail est à implémenter dans chaque module par l'override de 'message_processing'
		void update(const GlobalFrame& f) {
			lock_variables();
			std::string msg(f.getDonnees(), f.getDonnees() + f.getNbDonnees());
			try {
				message_processing(JSON::parse(msg));
			} catch(nlohmann::detail::parse_error& e) {
				logError("JSON parsing error: ", e.what());
			}
			unlock_variables();
		}

		/// Construit la trame du module (thread-safe)
		// Pour chaque module, il ne faut qu'overrider la fonction qui génère la struct JSON partagée avec Rust
		std::vector<GlobalFrame> make_frames() const {
			std::lock_guard<std::mutex> lk(_mutex_variables);

			std::vector<JSON> v = generate_list_jsons();

			std::vector<GlobalFrame> result;
			for(const JSON& j : v) {
				std::stringstream ss;
				ss << j;
				std::string str = ss.str();
				result.emplace_back(std::vector<uint8_t>(str.cbegin(), str.cend()));
			}

			return result;
		}

		/// Arrêt mécanique du module
		virtual void deactivation() = 0;

		const std::string name;

	protected:
		/// Conversion entre le module C++ et une structure JSON Rust-friendly
		/// Cette fonction est appelée par 'make_frame' et ne doit pas toucher au mutex
		virtual std::vector<JSON> generate_list_jsons() const = 0;

		/// Mise à jour du module à partir d'une structure JSON Rust-friendly entrante
		/// Cette fonction est appelée par 'update' et ne doit pas toucher au mutex
		virtual void message_processing(const JSON&) = 0;

		/// @return true si le JSON contient la liste des champs spécifiés
		bool json_has_fields(const JSON& j, std::vector<std::string> fields) const {
			for(std::string field : fields) {
				if(j.find(field) == j.cend()) {
					logError("JSON doesn't have the field '", field, "': ", j);
					return false;
				}
			}
			return true;
		}

		/// @return true si le JSON contient le champ spécifié
		bool json_has_field(const JSON& j, std::string field) const {
			return json_has_fields(j, std::vector<std::string>{field});
		}

		/// Bloque l'accés aux variables membres
		void lock_variables() {
			_mutex_variables.lock();
		}

		/// Débloque l'accès aux variables membres
		void unlock_variables() {
			_mutex_variables.unlock();
		}

		/// Mutex permettant de bloquer l'accès aux variables membres
		// Cette variable est 'mutable' pour permettre la fonction 'make_frame()' d'être constante malgré l'appel à un
		// 'lock_guard'
		mutable std::mutex _mutex_variables;

		/// Ce booléen passe à vrai si une modification a été apportée entre deux envois de trame
		/// Ça permets à l'`ElecCommunicator` d'envoyer en priorité le nouvel état du robot
		std::atomic_bool _state_changed;

		/// Le timer est réinitialisé à chaque construction de trame, c'est à dire à chaque envoi de l'état du module
		/// Lors de son expiration, l'état est construit et partagé avec l'électronique
		StopWatch _timer;

	private:
		/// ID du module dans le robot, allant de 0 à 15
		uint8_t _id;
	};
} // namespace PhysicalRobot

#endif //_MODULE_H
