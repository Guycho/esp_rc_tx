#ifndef INPUT_H
#define INPUT_H

#include <Arduino.h>
#include <Chrono.h>
#include <data_types.h>

#include "config.h"
#include "pin_reader.h"
#include "pot_reader.h"

struct InputControllerConfig {
    PinReader *buttons[Config::num_buttons];
    PotReader *potentiometers[Config::num_potentiometers];
};

using namespace DataTypes;

class InputController {
   public:
    InputController();
    ~InputController();

    void init(const InputControllerConfig &config);
    void run();
    RemoteControllerData get_data();

   private:
    PinReader *m_buttons[Config::num_buttons];
    PotReader *m_potentiometers[Config::num_potentiometers];

    RemoteControllerData m_data;
};

#endif  // INPUT_H