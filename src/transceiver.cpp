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
    // Extract the bitmask from the JSON document
    uint32_t bitmask_value = m_json_data["b"];
    TelemetryData telemetry_data;
    const uint8_t bitmask_size = sizeof(typeof(telemetry_data));
    std::bitset<bitmask_size> bitmask(bitmask_value);
    uint8_t* data_ptr = reinterpret_cast<uint8_t*>(&telemetry_data);
    for (size_t i = 0; i < sizeof(TelemetryData); ++i) {
        std::bitset<8> byte((bitmask >> (i * 8)).to_ulong() & 0xFF);
        data_ptr[i] = static_cast<uint8_t>(byte.to_ulong());
    }
    return telemetry_data;
}

void Transceiver::send_data() {
    JsonDocument m_json_data;
    String json;

    const uint8_t bitmask_size = sizeof(typeof(m_input_controller_data));
    std::bitset<bitmask_size> bitmask;

    uint8_t* data_ptr = reinterpret_cast<uint8_t*>(&m_input_controller_data);
    for (size_t i = 0; i < sizeof(typeof(m_input_controller_data)); ++i) {
        std::bitset<8> byte(data_ptr[i]);
        bitmask |= (std::bitset<bitmask_size>(byte.to_ulong()) << (i * 8));
    }

    m_json_data["b"] = bitmask.to_ullong();

    serializeJson(m_json_data, json);

    m_json_data["c"] = Calcs::calc_checksum(json);
    serializeJson(m_json_data, json);
    m_esp_now_handler->send_data(json);
}
