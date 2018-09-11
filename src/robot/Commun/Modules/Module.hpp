/**
 * Ce fichier est la base pour créer des modules électroniques
 * Ceux-ci s'appliquent à la stratégie post-2018 dans laquelle tout l'état du robot est regroupé
 *
 * La construction de chaque module spéficique (moteurs, servos, déplacement, capteurs, ...)
 * s'appuie sur une structure C partagée avec le code Rust et dont les fonctions de conversion
 * struct / trame sont entreposées dans une librairie C pour être utilisées par les deux parties.
 *
 * Enfin, la cohésion entre ces modules est gérée par la classe 'ModuleManager'.
 *
 * Pour créer un nouveau module, il faut :
 *  === dans les fichiers 'SharedWithRust.[c|h]' ===
 *   - créer une nouvelle structure en C partagée avec la partie électronique
 *   - écrire les fonctions de parsing struct / trame en C
 *  === dans une nouvelle classe 'Module{NOM_DU_MODULE}{ANNEE}.[cpp|h]' ===
 *   - faire hériter le nouveau module de la classe mère 'Module' en donnant les paramètres C
 *   - overrider les fonctions 'generate_shared' et 'message_processing' (utilisation de la struct)
 *   - définir la taille d'une trame en overridant 'get_frame_size' (/!\ fonction très importante)
 *
 * Et c'est tout ! Pour plus de détails, tu peux te référer aux modules déjà codés.
 */

#ifndef _MODULE_H
#define _MODULE_H

#include "../Communication/CAN.h"
#include <Commun.h>
#include <functional>
#include <mutex>

namespace Commun {

	// Cette classe permets de stocker des 'Modules' non-templates
	class BaseModule {
	public:
		explicit BaseModule(uint8_t id) : _id(id) {}

		virtual ~BaseModule() = default;

		/// Retourne l'ID du module dans le robot
		inline uint8_t get_id() {
			return _id;
		}

		/// Construit la trame du module (thread-safe)
		virtual GlobalFrame make_frame() const = 0;

		/// Traite les données fournies par le robot (thread-safe)
		virtual void update(const GlobalFrame&) = 0;

		/// Retourne la taille d'une trame
		inline virtual uint8_t get_frame_size() const = 0;

	protected:
		/// Bloque l'accés aux variables membres
		void lockVariables() {
			_mutexVariables.lock();
		}

		/// Débloque l'accès aux variables membres
		void unlockVariables() {
			_mutexVariables.unlock();
		}

		/// Mutex permettant de bloquer l'accès aux variables membres
		// Cette variable est 'mutable' pour permettre la fonction 'make_frame()' d'être constante malgré l'appel à un
		// 'lock_guard'
		mutable std::mutex _mutexVariables;

	private:
		/// ID du module dans le robot
		uint8_t _id;
	};

	/// Abstraction qui gère le lien entre les trames et les fonctions en C
	template <typename SharedStruct>
	class Module : public BaseModule {
	public:
		/// Initialisation du module
		Module(uint8_t id,
		       SharedStruct (*read_function)(const uint8_t*, uint8_t),
		       uint8_t (*write_function)(uint8_t*, uint8_t, const SharedStruct*))
		        : BaseModule(id), _wrapper(read_function, write_function) {}

		~Module() override = default;

		/// Traite les données fournies par le robot
		// Le gros du travail est à implémenter dans chaque module par l'override de 'message_processing'
		void update(const GlobalFrame& f) final {
			lockVariables();
			message_processing(_wrapper.read_frame(f.getDonnees(), f.getNbDonnees()));
			unlockVariables();
		}

		/// Construit la trame du module
		// Pour chaque module, il ne faut qu'overrider la fonction qui génère la struct partagée avec Rust
		// ainsi que la fonction C qui convertit cette struct en une trame binaire
		GlobalFrame make_frame() const final {
			GlobalFrame f;
			uint8_t buf[get_frame_size()];

			std::lock_guard<std::mutex> lk(_mutexVariables);
			SharedStruct s = generate_shared();
			uint8_t size = _wrapper.write_frame(buf, get_frame_size(), &s);

			if(size != get_frame_size()) {
				throw std::runtime_error("Bad frame generation - no matching sizes.");
			}
			f.addBytes(get_frame_size(), buf);
			return f;
		}

	protected:
		/// Conversion entre le module C++ et une structure Rust-friendly
		/// Cette fonction est appelée par 'make_frame' et ne doit pas toucher au mutex
		virtual SharedStruct generate_shared() const = 0;

		/// Mise à jour du module à partir d'une structure Rust-friendly entrante
		/// Cette fonction est appelée par 'update' et ne doit pas toucher au mutex
		virtual void message_processing(const SharedStruct&) = 0;

	private:
		/// Classe helper pour intégrer les fonctions codées en C
		struct FunctorWrapper {
			FunctorWrapper(SharedStruct (*read_function)(const uint8_t*, uint8_t),
			               uint8_t (*write_function)(uint8_t*, uint8_t, const SharedStruct*))
			        : read_frame(read_function), write_frame(write_function) {}

			std::function<SharedStruct(const uint8_t*, uint8_t)> read_frame;
			std::function<uint8_t(uint8_t*, uint8_t, const SharedStruct*)> write_frame;
		};

		FunctorWrapper _wrapper;
	};
}

#endif //_MODULE_H