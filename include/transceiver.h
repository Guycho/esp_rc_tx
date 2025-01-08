#ifndef TRANSCEIVER_H
#define TRANSCEIVER_H

#include <ArduinoJson.h>
#include <WiFi.h>
#include <Chrono.h>
#include "ESP_now_handler.h"


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

   private:
    float two_decimals(float value);
    Chrono m_data_timer;
    ESPNowHandler *m_esp_now_handler;

    uint16_t m_update_delay_ms;
};

#endif  // ESP32SERVER_HPP