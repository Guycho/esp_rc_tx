#ifndef TRANSCEIVER_H
#define TRANSCEIVER_H

#include <ArduinoJson.h>
#include <Chrono.h>
#include <WiFi.h>

#include "ESP_now_handler.h"
#include "config.h"
#include "input.h"

struct TransceiverConfig {
    uint16_t update_delay_ms;
    ESPNowHandler *esp_now_handler;
};

class Transceiver {
   public:
    Transceiver();
    ~Transceiver();

    void init(const TransceiverConfig &config);
    void update_data();
    void send_data();
    String get_remote_data();

   private:
    Chrono m_data_timer;
    ESPNowHandler *m_esp_now_handler;
    InputControllerData m_input_controller_data;
    float two_decimals(float value);
    String m_remote_data;
    uint16_t m_update_delay_ms;
};

#endif  // ESP32SERVER_HPP