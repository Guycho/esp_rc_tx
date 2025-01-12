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
    m_input_controller_data = get_input_data();
    m_remote_data = m_esp_now_handler->get_data();
    Serial.println(m_remote_data);
}

void Transceiver::send_data() {
    JsonDocument m_json_data;
    String json;
    if (m_input_controller_data.new_data) {
        m_data_index++;
    }
    m_json_data["t"] = two_decimals(m_input_controller_data.throttle);
    m_json_data["s"] = two_decimals(m_input_controller_data.steering);
    m_json_data["a"] = m_input_controller_data.arm_toggle;
    m_json_data["sm"] = m_input_controller_data.steering_mode_toggle;
    m_json_data["dm"] = m_input_controller_data.drive_mode_toggle;
    m_json_data["rr"]  = m_input_controller_data.lock_rear_right;
    m_json_data["rl"] = m_input_controller_data.lock_rear_left;
    m_json_data["wn"] = m_input_controller_data.write_to_nvm;
    m_json_data["tr"] = m_input_controller_data.trim_r;
    m_json_data["tl"] = m_input_controller_data.trim_l;
    m_json_data["tt"] = m_input_controller_data.trim_throttle;
    m_json_data["ts"] = m_input_controller_data.trim_steering;
    m_json_data["tdr"] = m_input_controller_data.trim_direction_r;
    m_json_data["tdl"] = m_input_controller_data.trim_direction_l;
    m_json_data["tdf"] = m_input_controller_data.trim_direction_f;
    m_json_data["tdb"] = m_input_controller_data.trim_direction_b;
    m_json_data["rt"] = m_input_controller_data.reset_trim;
    m_json_data["nd"] = m_data_index;
    serializeJson(m_json_data, json);
    m_esp_now_handler->send_data(json);
}

String Transceiver::get_remote_data() { return m_remote_data; }

float Transceiver::two_decimals(float value) { return round(value * 100) / 100; }
