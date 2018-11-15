#include "SimuLed.h"

#include <log/Log.h>

SimuLed::SimuLed(uint8_t id)
        : Module(id, led_read_frame, led_write_frame),
          _on_off{false} {

}

uint8_t SimuLed::get_frame_size() const {
    return get_size_led_frame();
}

void SimuLed::deactivation() {

}

SharedLED2019 SimuLed::generate_shared() const {
    SharedLED2019 shared_struct{};
    return shared_struct;
}

void SimuLed::message_processing(const SharedLED2019 &shared_struct) {
    if (shared_struct.parsing_failed != 0) {
        logWarn("Bad frame received by LED module");
        return;
    }

    auto new_on_off = static_cast<bool>(shared_struct.on_off);

    if (new_on_off != _on_off) {
        logDebug("LED state changed to ", new_on_off);
        _on_off = new_on_off;
    }
    _state_changed.exchange(true);
}
