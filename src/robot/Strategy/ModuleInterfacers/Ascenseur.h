//
//  Ascenseur.h
//  Club Robot
//
//  Created by Rémi on 03/03/2015.
//

#ifndef __Club_Robot__Ascenseur__
#define __Club_Robot__Ascenseur__

#include "../../Cartes/CarteAsservissement.h"
#include "Commun.h"

class CarteAsservissement;

namespace Commun {

	class Ascenseur {
	public:
		static const Duration TIMEOUT;
		/**
		 * Correspond à l'offset de l'ascenseur = ce qui est rajouté de base à tous ses déplacements
		 * Dans l'idée si on utilise cet offset c'est qu'on veut déposer sur la zone rouge au milieu de la table
		 */
		static const Angle OFFSET;
		/**
		 * Correspond à la hauteur d'un élément porté par les ascenseurs en roulant
		 */
		static const Distance HAUTEUR_AVANCE;

	public:
		/**
		 * Construit l'ascenseur. Les angles des positions doivent être dans l'ordre croissant
		 * i.e., on commence en position 0, incrémenter va aller en position 1, puis décrémenter va aller en position 0.
		 */
		Ascenseur(CarteAsservissement& carteAsservissement, std::vector<Angle> const& positions);
		virtual ~Ascenseur() = default;

		/**
		 * Balance une exception en cas de dépassement de bornes.
		 */
		ResultatAction incrementer();
		ResultatAction decrementer();

		std::size_t position() const {
			return _position;
		}

		bool estEnMouvement() {
			bool enMouvement = true;
			if(_mutexEnMouvement.try_lock()) {
				_mutexEnMouvement.unlock();
				enMouvement = false;
			}
			return enMouvement;
		}

		ResultatAction initialiser(SensRotation sens);
		void setVitesse(AngularSpeed v);

		void setOffset(Angle offset) {
			_offset = offset;
		}

		ResultatAction allerAngleBloquant(Angle angle);
		Angle getAngleBloquant();

	protected:
		ResultatAction positionnerAscBloquant(std::size_t pos);
		ResultatAction positionnerAsc(std::size_t pos);
		ResultatAction attendreAscAPos();
		bool verifierPositionnerAsc();
		bool verifierAscBloque();

		std::size_t _position = 0;

		CarteAsservissement& _carteAsservissement;
		std::vector<Angle> const _positions;
		std::mutex _mutexEnMouvement;
		Angle _offset;
	};
} // namespace Commun

#endif /* defined(__Club_Robot__Ascenseur__) */
