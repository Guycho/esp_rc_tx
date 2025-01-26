#include <Arduino.h>
#include <Chrono.h>
#include <ESP_now_handler.h>

#include "config.h"
#include "input.h"
#include "ota_handler.h"
#include "pin_reader.h"
#include "pot_reader.h"
#include "transceiver.h"

PinReader pin_readers[Config::num_buttons];
PotReader pot_readers[Config::num_potentiometers];
InputController input_controller;
ESPNowHandler esp_now_handler(Config::ESPNow::peer_mac_address, Config::ESPNow::use_lr,
  Config::ESPNow::print_debug);
OTAHandler ota_handler(Config::OTAHandler::hostname, Config::OTAHandler::credentials, Config::OTAHandler::num_networks);
Transceiver transceiver;

void setup() {
    Serial.begin(9600);

    for (uint8_t i = 0; i < Config::num_buttons; i++) {
        PinReaderConfig pin_reader_config;
        pin_reader_config.pin = Config::buttons_pins[i];
        pin_reader_config.on_state = Config::buttons_on_states[i];
        pin_reader_config.type_event = Config::buttons_type_event[i];
        pin_readers[i].init(pin_reader_config);
    }

    for (uint8_t i = 0; i < Config::num_potentiometers; i++) {
        PotReaderConfig pot_reader_config;
        pot_reader_config.pin = Config::potentiometers_pins[i];
        pot_reader_config.min_input_value = Config::potentiometers_min_input_values[i];
        pot_reader_config.max_input_value = Config::potentiometers_max_input_values[i];
        pot_reader_config.mid_pct_input_value = Config::potentiometers_mid_pct_input_values[i];
        pot_reader_config.reverse = Config::potentiometers_reverse[i];
        pot_readers[i].init(pot_reader_config);
    }

    InputControllerConfig input_controller_config;
    for (uint8_t i = 0; i < Config::num_buttons; i++) {
        input_controller_config.buttons[i] = &pin_readers[i];
    }
    for (uint8_t i = 0; i < Config::num_potentiometers; i++) {
        input_controller_config.potentiometers[i] = &pot_readers[i];
    }
    input_controller.init(input_controller_config);

    esp_now_handler.init();

    TransceiverConfig transceiver_config;
    transceiver_config.update_delay_ms = Config::Transceiver::update_delay_ms;
    transceiver_config.esp_now_handler = &esp_now_handler;
    transceiver_config.input_controller = &input_controller;
    transceiver.init(transceiver_config);
}

void loop() {
    transceiver.run();
}
