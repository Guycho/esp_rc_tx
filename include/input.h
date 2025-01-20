#ifndef INPUT_H
#define INPUT_H

#include <Arduino.h>
#include <Chrono.h>
#include "pin_reader.h"
#include "pot_reader.h"

struct InputControllerData {
    float throttle;
    float steering;
    bool left_arrow;
    bool right_arrow;
    bool up_arrow;
    bool down_arrow;
    bool sel;
    bool ch;
    bool plus;
    bool minus;
    bool left_trim_l;
    bool left_trim_r;
    bool right_trim_l;
    bool right_trim_r;
    bool edge_switch;
};

struct InputControllerConfig {

};

#endif  // INPUT_H