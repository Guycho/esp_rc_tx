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
    if (!(m_remote_data.length() == 0 || !verify_checksum(m_remote_data))) {
        m_telemetry_data = parse_remote_data(m_remote_data);
    }
    m_input_controller_data = m_input_controller->get_data();
    send_data();
}

TelemetryData Transceiver::get_telemetry_data() { return m_telemetry_data; }

bool Transceiver::verify_checksum(const String& data) {
    JsonDocument m_json_data;
    deserializeJson(m_json_data, data);
    // Extract the checksum from the JSON document
    uint8_t received_checksum = m_json_data["c"];

    // Calculate the checksum (XOR of all bytes except the checksum itself)
    uint8_t calculated_checksum = 0;
    for (size_t i = 0; i < data.length(); ++i) {
        if (data[i] == ',') break;  // Stop before the checksum field
        calculated_checksum ^= data[i];
    }
    calculated_checksum ^= '}';
    if (received_checksum != calculated_checksum) {
        return false;  // Checksum mismatch;
    }
    return true;
}

TelemetryData Transceiver::parse_remote_data(const String& data) {
    JsonDocument m_json_data;
    deserializeJson(m_json_data, data);
    // Extract the bitmask from the JSON document
    uint32_t bitmask = m_json_data["b"];
    TelemetryData remote_data;
    // Unpack the bitmask
    remote_data.arm_state = (bitmask >> 0) & 0x1;
    remote_data.steering_mode = (bitmask >> 1) & 0x3;
    remote_data.drive_mode = (bitmask >> 3) & 0x3;
    remote_data.battery_status = (bitmask >> 5) & 0x3;
    return remote_data;
}

void Transceiver::send_data() {
    JsonDocument m_json_data;
    String json;
    uint16_t scaled_throttle =
      static_cast<uint16_t>((m_input_controller_data.throttle + 100) * 2.555);
    uint16_t scaled_steering =
      static_cast<uint16_t>((m_input_controller_data.steering + 100) * 2.555);

    // Create a bitmask for the boolean values and scaled throttle/steering
    uint32_t bitmask = 0;
    bitmask |= (scaled_throttle << 0);   // 9 bits for throttle
    bitmask |= (scaled_steering << 9);  // 9 bits for steering
    bitmask |= (m_input_controller_data.left_arrow << 18);
    bitmask |= (m_input_controller_data.right_arrow << 19);
    bitmask |= (m_input_controller_data.up_arrow << 20);
    bitmask |= (m_input_controller_data.down_arrow << 21);
    bitmask |= (m_input_controller_data.sel << 22);
    bitmask |= (m_input_controller_data.ch << 23);
    bitmask |= (m_input_controller_data.plus << 24);
    bitmask |= (m_input_controller_data.minus << 25);
    bitmask |= (m_input_controller_data.left_trim_l << 26);
    bitmask |= (m_input_controller_data.left_trim_r << 27);
    bitmask |= (m_input_controller_data.right_trim_l << 28);
    bitmask |= (m_input_controller_data.right_trim_r << 29);
    bitmask |= (m_input_controller_data.edge_switch << 30);
    bitmask |= (m_input_controller_data.bottom_switch << 31);

    m_json_data["b"] = bitmask;
    serializeJson(m_json_data, json);

    uint8_t checksum = 0;
    for (char c : json) {
        checksum ^= c;
    }
    m_json_data["c"] = checksum;
    serializeJson(m_json_data, json);
    m_esp_now_handler->send_data(json);
}
