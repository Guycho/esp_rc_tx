#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>


namespace Config {
const uint8_t num_wheels = 4;
const uint8_t num_steering = 2;
namespace Transceiver {
extern const uint16_t update_delay_ms;
}  // namespace Transceiver
namespace PS4Controller {
extern const char *mac;
extern const float dead_band;
}  // namespace PS4Controller
namespace ESPNow {
extern const uint8_t peer_mac_address[];
}  // namespace ESPNow
}  // namespace Config
#endif  // CONFIG_H