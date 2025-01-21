#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

namespace Config {
enum ButtonName {
    LEFT_ARROW = 0,
    RIGHT_ARROW = 1,
    UP_ARROW = 2,
    DOWN_ARROW = 3,
    SEL = 4,
    CH = 5,
    PLUS = 6,
    MINUS = 7,
    LEFT_TRIM_L = 8,
    LEFT_TRIM_R = 9,
    RIGHT_TRIM_L = 10,
    RIGHT_TRIM_R = 11,
    EDGE_SWITCH = 12,
    BOTTOM_SWITCH = 13
};

enum PotentiometerName { THROTTLE = 0, STEERING = 1 };
const uint16_t debounce_delay_ms = 50;
const uint8_t num_buttons = 14;
const uint8_t buttons_pins[num_buttons] = {32, 33, 25, 26, 27, 14, 12, 13, 23, 22, 21, 19, 18, 5};
const bool buttons_on_states[num_buttons] = {false, false, false, false, false, false, false, false,
  false, false, false, false, false, false};
const bool buttons_type_event[num_buttons] = {false, false, false, false, false, false, false,
  false, false, false, true, false, false, false};
// const bool buttons_type_event[num_buttons] = {true, true, true, true, true, true, true, true, true,
//   true, true, true, false, false};
const float potentiometers_dead_band = 5.0;
const double max_output_value = 100.0;
const uint8_t num_potentiometers = 2;
const uint8_t analog_read_resolution = 12;
const uint8_t potentiometers_pins[num_potentiometers] = {34, 35};
const uint16_t potentiometers_min_input_values[num_potentiometers] = {1400, 800};
const uint16_t potentiometers_max_input_values[num_potentiometers] = {4095, 3725};
const bool potentiometers_reverse[num_potentiometers] = {false, false};
const float potentiometers_mid_pct_input_values[num_potentiometers] = {-49.0, 0.0};
namespace Transceiver {
const uint16_t update_delay_ms = 100;
}  // namespace Transceiver
namespace ESPNow {
const uint8_t peer_mac_address[] = {0xA0, 0xDD, 0x6C, 0x04, 0x2D, 0x98};
const bool use_lr = true;
const bool print_debug = false;
}  // namespace ESPNow
}  // namespace Config
#endif  // CONFIG_H