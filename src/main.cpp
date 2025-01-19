#include <Arduino.h>
#include <Chrono.h>
#include <ESP_now_handler.h>

#include "config.h"
#include "input.h"
#include "transceiver.h"
#include "pin_reader.h"
#include "pot_reader.h"

PinReader pin_readers[Config::num_buttons];
PotReader pot_readers[Config::num_potentiometers];

ESPNowHandler esp_now_handler(Config::ESPNow::peer_mac_address, Config::ESPNow::use_lr,
  Config::ESPNow::print_debug);
Transceiver transceiver;

void setup() {
    for (uint8_t i = 0; i < Config::num_buttons; i++) {
        PinReaderConfig pin_reader_config;
        pin_reader_config.pin = Config::buttons_pins[i];
        pin_reader_config.on_state = Config::buttons_on_states[i];
        pin_readers[i].init(pin_reader_config);
    }

    for (uint8_t i = 0; i < Config::num_potentiometers; i++) {
        PotReaderConfig pot_reader_config;
        pot_reader_config.pin = Config::potentiometers_pins[i];
        pot_reader_config.min_input_value = Config::potentiometers_min_input_values[i];
        pot_reader_config.max_input_value = Config::potentiometers_max_input_values[i];
        pot_reader_config.reverse = Config::potentiometers_reverse[i];
        pot_readers[i].init(pot_reader_config);
    }
    
    Serial.begin(9600);

    esp_now_handler.init();

    TransceiverConfig transceiver_config;
    transceiver_config.update_delay_ms = Config::Transceiver::update_delay_ms;
    transceiver_config.esp_now_handler = &esp_now_handler;
    transceiver.init(transceiver_config);
}

void loop() {}
