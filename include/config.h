#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

namespace Config {
enum ButtonName {
    EDGE_SWITCH = 0,
    SEL = 1,
    CH = 2,
    PLUS = 3,
    MINUS = 4,
    BOTTOM_SWITCH = 5,
    RIGHT_TRIM_R = 6,
    RIGHT_TRIM_L = 7,
    LEFT_TRIM_R = 8,
    LEFT_TRIM_L = 9,
    UP_ARROW = 10,
    RIGHT_ARROW = 11,
    DOWN_ARROW = 12,
    LEFT_ARROW = 13
};
enum PotentiometerName { THROTTLE = 0, STEERING = 1 };
const uint16_t debounce_delay_ms = 50;
const uint8_t num_buttons = 14;
const uint8_t buttons_pins[num_buttons] = {32, 33, 25, 26, 27, 14, 12, 13, 23, 22, 21, 19, 18, 5};
const bool buttons_on_states[num_buttons] = {false, false, false, false, false, false, false, false,
  false, false, false, false, false, false};
const bool buttons_type_event[num_buttons] = {false, true, true, true, true, false, true, true,
  true, true, false, false, false, false};
const float potentiometers_dead_band = 5.0;
const double max_output_value = 100.0;
const uint8_t num_potentiometers = 2;
const uint8_t analog_read_resolution = 12;
const uint8_t potentiometers_pins[num_potentiometers] = {34, 35};
const uint16_t potentiometers_min_input_values[num_potentiometers] = {1400, 310};
const uint16_t potentiometers_max_input_values[num_potentiometers] = {4095, 3800};
const bool potentiometers_reverse[num_potentiometers] = {false, false};
const float potentiometers_mid_pct_input_values[num_potentiometers] = {-49.0, -8.0};
namespace Transceiver {
const uint16_t update_delay_ms = 25;
}  // namespace Transceiver
namespace ESPNow {
const uint8_t peer_mac_address[] = {0xA0, 0xDD, 0x6C, 0x04, 0x2D, 0x98};
const bool use_lr = false;
const bool print_debug = false;
}  // namespace ESPNow
namespace OTAHandler {
const uint8_t num_networks = 3;
const uint16_t timeout_sec = 60;
const bool print_debug = false;
extern const char *credentials[][2];
extern const char *hostname;
}  // namespace OTAHandler
}  // namespace Config
#endif  // CONFIG_H