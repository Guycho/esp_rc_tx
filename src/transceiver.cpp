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

RemoteData Transceiver::parse_remote_data(const String& data) {
    JsonDocument m_json_data;
    deserializeJson(m_json_data, data);
    // Extract the bitmask from the JSON document
    uint32_t bitmask = m_json_data["b"];
    RemoteData remote_data;
    // Unpack the bitmask
    remote_data.throttle = (((bitmask >> 0) & 0x1FF) / 2.555) - 100;  // 9 bits for throttle
    remote_data.steering = (((bitmask >> 9) & 0x1FF) / 2.555) - 100;  // 9 bits for steering
    // compensate for resolution loss in the remote
    if (abs(remote_data.throttle) < 0.3) {
        remote_data.throttle = 0;
    }
    if (abs(remote_data.steering) < 0.3) {
        remote_data.steering = 0;
    }
    remote_data.left_arrow = (bitmask >> 18) & 0x1;
    remote_data.right_arrow = (bitmask >> 19) & 0x1;
    remote_data.up_arrow = (bitmask >> 20) & 0x1;
    remote_data.down_arrow = (bitmask >> 21) & 0x1;
    remote_data.sel = (bitmask >> 22) & 0x1;
    remote_data.ch = (bitmask >> 23) & 0x1;
    remote_data.plus = (bitmask >> 24) & 0x1;
    remote_data.minus = (bitmask >> 25) & 0x1;
    remote_data.left_trim_l = (bitmask >> 26) & 0x1;
    remote_data.left_trim_r = (bitmask >> 27) & 0x1;
    remote_data.right_trim_l = (bitmask >> 28) & 0x1;
    remote_data.right_trim_r = (bitmask >> 29) & 0x1;
    remote_data.edge_switch = (bitmask >> 30) & 0x1;
    remote_data.bottom_switch = (bitmask >> 31) & 0x1;
    remote_data.new_data = true;
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
String Transceiver::get_remote_data() { return m_remote_data; }
