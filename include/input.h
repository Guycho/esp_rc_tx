#ifndef INPUT_CONTROLLER_H
#define INPUT_CONTROLLER_H

#include <PS4Controller.h>

#include "esp_bt_device.h"
#include "esp_bt_main.h"
#include "esp_err.h"
#include "esp_gap_bt_api.h"
#include "utils.h"

struct InputControllerData {
    float throttle;
    float steering;
    uint8_t arm_toggle;
    uint8_t steering_mode_toggle;
    uint8_t drive_mode_toggle;
    uint8_t lock_rear_right;
    uint8_t lock_rear_left;
    uint8_t write_to_nvm;
    uint8_t trim_r;
    uint8_t trim_l;
    uint8_t trim_throttle;
    uint8_t trim_steering;
    uint8_t trim_direction_r;
    uint8_t trim_direction_l;
    uint8_t trim_direction_f;
    uint8_t trim_direction_b;
    uint8_t reset_trim;
    uint8_t new_data;
};

struct InputControllerConfig {
    const char* mac;
    float dead_band;
};

void init_ps4(const InputControllerConfig config);
InputControllerData get_input_data();

#endif  // INPUT_CONTROLLER_H