#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>


namespace Config {
const uint16_t debounce_delay_ms = 50;
const uint8_t num_buttons = 13;
const uint8_t buttons_pins[num_buttons] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
const bool buttons_on_states[num_buttons] = {false, false, false, false, false, false, false, false,
  false, false, false, false, false};
const float potentiometers_dead_band = 5.0;
const double max_output_value = 100.0;
const uint8_t num_potentiometers = 2;
const uint8_t potentiometers_pins[num_potentiometers] = {A0, A1};
const uint16_t potentiometers_min_input_values[num_potentiometers] = {0.0, 0.0};
const uint16_t potentiometers_max_input_values[num_potentiometers] = {1023.0, 1023.0};
const bool potentiometers_reverse[num_potentiometers] = {false, false};
namespace Transceiver {
extern const uint16_t update_delay_ms;
}  // namespace Transceiver
namespace ESPNow {
extern const uint8_t peer_mac_address[];
extern const bool use_lr;
extern const bool print_debug;
}  // namespace ESPNow
}  // namespace Config

#endif  // CONFIG_H