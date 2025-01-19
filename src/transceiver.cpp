#include "transceiver.h"

Transceiver::Transceiver() {}

Transceiver::~Transceiver() {}

void Transceiver::init(const TransceiverConfig& config) {
    m_esp_now_handler = config.esp_now_handler;
    m_update_delay_ms = config.update_delay_ms;
    m_data_timer.start();
}

void Transceiver::update_data() {
    if (!m_data_timer.hasPassed(m_update_delay_ms, true)) {
        return;
    }
    m_input_data = get_input_data();
    m_remote_data = m_esp_now_handler->get_data();
    Serial.println(m_remote_data);
}

void Transceiver::send_data() {
    JsonDocument m_json_data;
    String json;
    serializeJson(m_json_data, json);
    m_esp_now_handler->send_data(json);
}

String Transceiver::get_remote_data() { return m_remote_data; }

float Transceiver::two_decimals(float value) { return round(value * 100) / 100; }
