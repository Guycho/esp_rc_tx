#include "config.h"

namespace Config {
namespace Server {
const char *ssid = "Guycho";
const char *password = "0544904809";
// const char *ssid = "OmerRobotics";
// const char *password = "qazwsx123";
}  // namespace Server
namespace Transceiver {
const uint16_t update_delay_ms = 100;
}  // namespace Transceiver
namespace PS4Controller {
const char *mac = "A0:DD:6C:03:9A:EE";
const float dead_band = 5;
}  // namespace PS4Controller
namespace ESPNow {
const uint8_t peer_mac_address[] = {0xA0, 0xDD, 0x6C, 0x04, 0x2D, 0x98};
}  // namespace ESPNow
}  // namespace Config
