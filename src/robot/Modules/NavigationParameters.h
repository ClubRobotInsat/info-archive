//
// Created by terae on 9/10/18.
//

#ifndef ROOT_MODULENAVIGATIONPARAMETERS_H
#define ROOT_MODULENAVIGATIONPARAMETERS_H

#include "Module.hpp"

namespace PhysicalRobot {

	class NavigationParameters final : public Module {
	public:
		explicit NavigationParameters(uint8_t id) : Module(id, "NavigationParameters") {}

		void set_left_coder_radius(Distance radius);

		void set_right_wheel_coef(float coef);

		void set_inter_axial_length(Distance length);

		Distance get_left_wheel_radius() const;

		Distance get_right_wheel_radius() const;

		float get_right_wheel_coef() const;

		Distance get_inter_axial_length() const;

	protected:
		std::vector<JSON> generate_list_jsons() const override;

		void message_processing(const JSON&) override;

		void deactivation() override;

	private:
		/// Le rayon d'une roue codeuse en dizième de mm
		uint16_t _left_coder_radius = 300;
		/// Coefficient de correction de la roue codeuse droite, pour corriger
		/// le sens et l'écart de diamètre entre les deux roues.
		float _right_wheel_coef = 1.00428;
		/// La distance entre les roues codeuses en mm
		uint16_t _inter_axial_length = 2970;
		/// Le coefficient proportionnel sur la position
		float _pos_kp = 1;
		/// Le coefficient dérivé sur la position
		float _pos_kd = 0;
		/// Le coefficient proportionnel sur l'orientation
		float _orient_kp = 1;
		/// Le coefficient dérivée sur l'orientation
		float _orient_kd = 0;
	};

} // namespace PhysicalRobot

#endif // ROOT_MODULENAVIGATIONPARAMETERS_H
