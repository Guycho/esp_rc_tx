#include "bt_app_connector.h"

BTAppConnector::BTAppConnector() {}
BTAppConnector::~BTAppConnector() {}

void BTAppConnector::init(const BTAppConnectorConfig &config) {
    m_bt_serial = config.bt_serial;
    m_transceiver = config.transceiver;
    m_update_delay_ms = 1e3 / config.update_rate_hz;
    m_data_timer.start();
    m_is_connected = false;
    m_bt_serial->begin(config.device_name);
}

void BTAppConnector::run() {
    if (!m_data_timer.hasPassed(m_update_delay_ms, true)) {
        return;
    }
    TelemetryData telemetry_data = m_transceiver->get_telemetry_data();
    JsonDocument m_json_data;
    m_json_data["arm_enabled"] = telemetry_data.arm_state ? "Armed" : "Disarmed";
    m_json_data["steering_mode"] = telemetry_data.steering_mode == 0 ? "Normal" : "Gyro";
    m_json_data["drive_mode"] = telemetry_data.drive_mode == 0   ? "AWD"
                                : telemetry_data.drive_mode == 1 ? "CS"
                                                                 : "RWD";
    m_json_data["battery_status"] = telemetry_data.battery_status == 0   ? "Normal"
                                    : telemetry_data.battery_status == 1 ? "Low"
                                    : telemetry_data.battery_status == 2 ? "Critical"
                                                                      : "USB";
    for (uint8_t i = 0; i < 4; ++i) {
        m_json_data["motor_rpm"][i] = telemetry_data.motors_rpm[i];
        m_json_data["motor_throttle"][i] = abs(telemetry_data.motors_throttle[i]);
    }
    m_json_data["battery_voltage"] = telemetry_data.battery_voltage;
    for (uint8_t i = 0; i < 2; ++i) {
        m_json_data["steering_values"][i] = telemetry_data.steering_values[i];
    }
    m_json_data["g_force_x"] = telemetry_data.g_force_x;
    m_json_data["g_force_y"] = telemetry_data.g_force_y;
    m_json_data["rotational_rate"] = telemetry_data.rotation_rate_z;
    String json;
    serializeJson(m_json_data, json);
    m_bt_serial->println(json);
}
