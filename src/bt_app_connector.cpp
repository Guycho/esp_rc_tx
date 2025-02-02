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
    // m_json_data["throttle"] = control_data.throttle;
    // m_json_data["steering"] = control_data.steering;
    // m_json_data["arm_enabled"] = control_data.arm_enabled ? "Armed" : "Disarmed";
    // m_json_data["steering_mode"] = control_data.steering_mode == 0 ? "Normal" : "Gyro";
    // m_json_data["drive_mode"] = control_data.drive_mode == 0   ? "AWD"
    //                             : control_data.drive_mode == 1 ? "CS"
    //                                                            : "RWD";
    // m_json_data["motor1_rpm"] = control_data.mavlink_data.four_motor_speed.motor1_rpm;
    // m_json_data["motor1_throttle"] = abs(control_data.wheels_mixer_data.motor_speed[0]);
    // m_json_data["motor2_rpm"] = control_data.mavlink_data.four_motor_speed.motor2_rpm;
    // m_json_data["motor2_throttle"] = abs(control_data.wheels_mixer_data.motor_speed[1]);
    // m_json_data["motor3_rpm"] = control_data.mavlink_data.four_motor_speed.motor3_rpm;
    // m_json_data["motor3_throttle"] = abs(control_data.wheels_mixer_data.motor_speed[2]);
    // m_json_data["motor4_rpm"] = control_data.mavlink_data.four_motor_speed.motor4_rpm;
    // m_json_data["motor4_throttle"] = abs(control_data.wheels_mixer_data.motor_speed[3]);
    // m_json_data["battery_voltage"] = control_data.mavlink_data.battery_voltage;
    // m_json_data["right_steering"] = control_data.steering_mixer_data.motor_speed[0];
    // m_json_data["left_steering"] = control_data.steering_mixer_data.motor_speed[1];
    // m_json_data["g_force_x"] = control_data.mavlink_data.inertial_data.acceleration.x;
    // m_json_data["g_force_y"] = control_data.mavlink_data.inertial_data.acceleration.y;
    // m_json_data["max_g_force"] = m_max_g_force;
    // m_json_data["rotational_rate"] = control_data.mavlink_data.inertial_data.gyro.z;
    m_json_data["throttle"] = 0;
    m_json_data["steering"] = 0;
    m_json_data["arm_enabled"] = 0;
    m_json_data["steering_mode"] = 0;
    m_json_data["drive_mode"] = 0;
    m_json_data["motor1_rpm"] = 0;
    m_json_data["motor1_throttle"] = 0;
    m_json_data["motor2_rpm"] = 0;
    m_json_data["motor2_throttle"] = 0;
    m_json_data["motor3_rpm"] = 0;
    m_json_data["motor3_throttle"] = 0;
    m_json_data["motor4_rpm"] = 0;
    m_json_data["motor4_throttle"] = 0;
    m_json_data["battery_voltage"] = 0;
    m_json_data["right_steering"] = 0;
    m_json_data["left_steering"] = 0;
    m_json_data["g_force_x"] = 0;
    m_json_data["g_force_y"] = 0;
    m_json_data["max_g_force"] = 0;
    m_json_data["rotational_rate"] = 0;
}
