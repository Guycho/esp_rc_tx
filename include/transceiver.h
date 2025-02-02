#ifndef TRANSCEIVER_H
#define TRANSCEIVER_H

#include <ArduinoJson.h>
#include <Chrono.h>
#include <ESP_now_handler.h>
#include <WiFi.h>
#include <utils.h>

#include <bitset>

#include "config.h"
#include "input.h"

struct TransceiverConfig {
    InputController *input_controller;
    ESPNowHandler *esp_now_handler;

    uint16_t update_delay_ms;
};

class Transceiver {
   public:
    Transceiver();
    ~Transceiver();

    void init(const TransceiverConfig &config);
    void run();
    TelemetryData get_telemetry_data();

   private:
    ESPNowHandler *m_esp_now_handler;
    InputController *m_input_controller;

    void send_data();
    bool verify_checksum(const String &data);
    TelemetryData parse_remote_data(const String &data);
    Chrono m_data_timer;

    RemoteControllerData m_input_controller_data;
    TelemetryData m_telemetry_data;
    String m_remote_data;
    uint16_t m_update_delay_ms;
    uint8_t m_data_index = 0;
};

#endif  // ESP32SERVER_HPP