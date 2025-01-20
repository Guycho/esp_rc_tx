#ifndef INPUT_H
#define INPUT_H

#include <Arduino.h>
#include <Chrono.h>
#include "config.h"
#include "pin_reader.h"
#include "pot_reader.h"

struct InputControllerData {
    float throttle = 0;
    float steering = 0;
    bool left_arrow = 0;
    bool right_arrow = 0;
    bool up_arrow = 0;
    bool down_arrow = 0;
    bool sel = 0;
    bool ch = 0;
    bool plus = 0;
    bool minus = 0;
    bool left_trim_l = 0;
    bool left_trim_r = 0;
    bool right_trim_l = 0;
    bool right_trim_r = 0;
    bool edge_switch = 0;
    bool bottom_switch = 0;
};

struct InputControllerConfig {
    PinReader *buttons[Config::num_buttons];
    PotReader *potentiometers[Config::num_potentiometers];
};

class InputController {
   public:
    InputController();
    ~InputController();

    void init(const InputControllerConfig &config);
    void run();
    InputControllerData get_data();

   private:
    PinReader *m_buttons[Config::num_buttons];
    PotReader *m_potentiometers[Config::num_potentiometers];

    InputControllerData m_data;
};

#endif  // INPUT_H