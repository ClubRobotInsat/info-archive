#include <resources/EmbeddedFiles.h>
#include "PanelRobotState.h"

PanelRobotState::PanelRobotState() {
    _builder = Gtk::Builder::create_from_string(EmbeddedFiles::readText("glade/state-panel.glade"));
}



void refreshPanel(JSON json){

}

JSON PanelRobotState::generateTestJSON() {
    JSON json;
    json["servo1"] = JSON();
    json["servo2"] = JSON();

    json["servo1"]["blocked"] = true;
    json["servo2"]["blocked"] = false;
    json["servo1"]["mode"] = "Speed";
    json["servo1"]["color"] = "Color";

    return json;
}