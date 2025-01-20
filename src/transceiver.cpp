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
    m_input_controller_data = m_input_controller->get_data();
    send_data();
    m_remote_data = m_esp_now_handler->get_data();
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
    bitmask |= (scaled_throttle << 0);   // 10 bits for throttle
    bitmask |= (scaled_steering << 9);  // 10 bits for steering
    bitmask |= (m_input_controller_data.left_arrow << 19);
    bitmask |= (m_input_controller_data.right_arrow << 20);
    bitmask |= (m_input_controller_data.up_arrow << 21);
    bitmask |= (m_input_controller_data.down_arrow << 22);
    bitmask |= (m_input_controller_data.sel << 23);
    bitmask |= (m_input_controller_data.ch << 24);
    bitmask |= (m_input_controller_data.plus << 25);
    bitmask |= (m_input_controller_data.minus << 26);
    bitmask |= (m_input_controller_data.left_trim_l << 27);
    bitmask |= (m_input_controller_data.left_trim_r << 28);
    bitmask |= (m_input_controller_data.right_trim_l << 29);
    bitmask |= (m_input_controller_data.right_trim_r << 30);
    bitmask |= (m_input_controller_data.edge_switch << 31);

    // Add the bitmask to the JSON document
    m_json_data["b"] = bitmask;
    serializeJson(m_json_data, json);

    // Calculate checksum (XOR of all bytes)
    uint8_t checksum = 0;
    for (size_t i = 0; i < json.length(); ++i) {
        checksum ^= json[i];
    }
    // Add checksum to the JSON document
    m_json_data["c"] = checksum;
    // Serialize JSON again to include the checksum
    serializeJson(m_json_data, json);
    m_esp_now_handler->send_data(json);
}

String Transceiver::get_remote_data() { return m_remote_data; }
