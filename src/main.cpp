#include <Chrono.h>

#include "ESP_now_handler.h"
#include "config.h"
#include "input.h"
#include "transceiver.h"

ESPNowHandler esp_now_handler(Config::ESPNow::peer_mac_address);
Transceiver transceiver;

void setup() {
    Serial.begin(9600);

    esp_now_handler.init();

    InputControllerConfig input_config;
    input_config.mac = Config::PS4Controller::mac;
    input_config.dead_band = Config::PS4Controller::dead_band;
    init_ps4(input_config);

    TransceiverConfig transceiver_config;
    transceiver_config.update_delay_ms = Config::Transceiver::update_delay_ms;
    transceiver_config.esp_now_handler = &esp_now_handler;
    transceiver.init(transceiver_config);
}

void loop() {
}