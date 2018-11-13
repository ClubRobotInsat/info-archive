#include "SimuServos.h"

SimuServos::SimuServos(uint8_t id)
        : Module(id, servo_read_frame, servo_write_frame) {

}

uint8_t SimuServos::get_frame_size() const {
    return get_size_servo_frame(get_nbr_servos());
}

uint8_t SimuServos::get_nbr_servos() const {
    return 5; // TODO
}

void SimuServos::deactivation() {

}

SharedServos2019 SimuServos::generate_shared() const {
    SharedServos2019 s = {};
    /*s.nb_servos = get_nbr_servos();
    uint8_t count = 0;
    for(uint8_t index = 0; index < ID_MAX_SERVOS; ++index) {
        if(_servos[index]) {
            s.servos[count].id = _servos[index]->id;

            auto angle_to_uint16t = [count, &s](Angle angle) -> uint16_t {
                uint16_t pos = static_cast<uint16_t>((angle.toMinusPiPi().toDeg() + 166.7) * 1023 / 333.4);
                if(pos < 21 || pos > 1002) {
                    logWarn("Angle demandé en-dehors de l'intervalle [-159.8°; 159.8°] pour le servo n°",
                            static_cast<int>(s.servos[count].id));
                }
                pos = static_cast<uint16_t>(pos < 21 ? 21 : (pos > 1023 ? 1023 : pos));
                return pos;
            };

            s.servos[count].position = angle_to_uint16t(_servos[index]->position);
            if(_servos[index]->command_type == Servo::CommandType::POSITION) {
                s.servos[count].command = angle_to_uint16t(std::get<Angle>(_servos[index]->command));
            } else {
                s.servos[count].command = std::get<uint16_t>(_servos[index]->command);
            }
            s.servos[count].command = _servos[index]->command_type;
            s.servos[count].blocked = _servos[index]->blocked;
            s.servos[count].color = _servos[index]->color;
            s.servos[count].blocking_mode = _servos[index]->blocking_mode;
            count++;
        }
    }

    s.parsing_failed = 0;*/
    return s;
}

void SimuServos::message_processing(const SharedServos2019 &) {
    std::cout << "Trame recue" << std::endl;
}