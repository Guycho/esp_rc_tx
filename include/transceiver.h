#ifndef TRANSCEIVER_H
#define TRANSCEIVER_H

#include <ArduinoJson.h>
#include <Chrono.h>
#include <WiFi.h>

#include <ESP_now_handler.h>
#include "config.h"
#include "input.h"

struct TransceiverConfig {
    InputController *input_controller;
    ESPNowHandler *esp_now_handler;

    uint16_t update_delay_ms;
};

struct RemoteData {
    bool arm_state;
    uint8_t steering_mode;
    uint8_t drive_mode;
    uint8_t battery_status;
};

class Transceiver {
   public:
    Transceiver();
    ~Transceiver();

    void init(const TransceiverConfig &config);
    void run();

   private:
    ESPNowHandler *m_esp_now_handler;
    InputController *m_input_controller;

    void send_data();
    String get_remote_data();
    bool verify_checksum(const String &data);
    RemoteData parse_remote_data(const String &data);

    Chrono m_data_timer;

    InputControllerData m_input_controller_data;
    RemoteData m_remote_data;
    String m_remote_data;
    uint16_t m_update_delay_ms;
    uint8_t m_data_index = 0;
};

#endif  // ESP32SERVER_HPP