#include "transceiver.h"

Transceiver::Transceiver() {}

Transceiver::~Transceiver() {}

void Transceiver::init(const TransceiverConfig& config) {
    m_esp_now_handler = config.esp_now_handler;
    m_update_delay_ms = config.update_delay_ms;
    m_input_controller = config.input_controller;
    m_data_timer.start();
}

void Transceiver::run() {
    if (!m_data_timer.hasPassed(m_update_delay_ms, true)) {
        return;
    }
    m_remote_data = m_esp_now_handler->get_data();
    if (m_remote_data.length() != 0 && Calcs::verify_checksum(m_remote_data)) {
        m_telemetry_data = parse_remote_data(m_remote_data);
    }
    m_input_controller_data = m_input_controller->get_data();
    send_data();
}

TelemetryData Transceiver::get_telemetry_data() { return m_telemetry_data; }

TelemetryData Transceiver::parse_remote_data(const String& data) {
    JsonDocument m_json_data;
    deserializeJson(m_json_data, data);
    TelemetryData telemetry_data;
    const size_t data_size = sizeof(typeof(telemetry_data)) * 8;
    std::bitset<data_size> bitmask;
    uint8_t num_of_bitmasks = data_size / (sizeof(uint32_t) * 8);
    uint32_t bitmasks[num_of_bitmasks];
    for (uint8_t i = 0; i < num_of_bitmasks; ++i) {
        bitmasks[i] = m_json_data["b"][i];
        for (uint8_t j = 0; j < 32 && (i * 32 + j) < bitmask.size(); ++j) {
            bitmask[i * 32 + j] = (bitmasks[i] & (1UL << j));
        }
    }
    memcpy(&telemetry_data, &bitmask, sizeof(typeof(telemetry_data)));
    return telemetry_data;
}

void Transceiver::send_data() {
    JsonDocument m_json_data;
    String json;
    const size_t data_size = sizeof(typeof(m_input_controller_data)) * 8;
    std::bitset<data_size> bitmask;
    memcpy(&bitmask, &m_input_controller_data, sizeof(typeof(m_input_controller_data)));

    uint8_t num_of_bitmasks = data_size / (sizeof(uint32_t) * 8);

    uint32_t bitmasks[num_of_bitmasks];
    for (uint8_t i = 0; i < num_of_bitmasks; ++i) {
        bitmasks[i] = 0;
        for (uint8_t j = 0; j < 32 && (i * 32 + j) < bitmask.size(); ++j) {
            if (bitmask[i * 32 + j]) {
                bitmasks[i] |= (1UL << j);
            }
        }
        m_json_data["b"][i] = bitmasks[i];
    }
    serializeJson(m_json_data, json);

    m_json_data["c"] = Calcs::calc_checksum(json);
    serializeJson(m_json_data, json);
    m_esp_now_handler->send_data(json);
}