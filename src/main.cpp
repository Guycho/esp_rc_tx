#include <Arduino.h>
#include <Chrono.h>
#include <ESP_now_handler.h>
#include <OTAHandler.h>
#include <TelnetStream.h>

#include "config.h"
#include "input.h"
#include "pin_reader.h"
#include "pot_reader.h"
#include "transceiver.h"

PinReader pin_readers[Config::num_buttons];
PotReader pot_readers[Config::num_potentiometers];
InputController input_controller;
ESPNowHandler esp_now_handler(Config::ESPNow::peer_mac_address, Config::ESPNow::use_lr,
  Config::ESPNow::print_debug);
OTAHandler ota_handler(Config::OTAHandler::hostname, Config::OTAHandler::credentials,
  Config::OTAHandler::num_networks, Config::OTAHandler::timeout_sec,
  Config::OTAHandler::print_debug);
Transceiver transceiver;

void setup() {
    Serial.begin(9600);
    InputControllerConfig input_controller_config;
    for (uint8_t i = 0; i < Config::num_buttons; i++) {
        PinReaderConfig pin_reader_config = {.pin = Config::buttons_pins[i],
          .on_state = Config::buttons_on_states[i],
          .type_event = Config::buttons_type_event[i]};
        pin_readers[i].init(pin_reader_config);
        input_controller_config.buttons[i] = &pin_readers[i];
    }

    for (uint8_t i = 0; i < Config::num_potentiometers; i++) {
        PotReaderConfig pot_reader_config = {.pin = Config::potentiometers_pins[i],
          .min_input_value = Config::potentiometers_min_input_values[i],
          .max_input_value = Config::potentiometers_max_input_values[i],
          .mid_pct_input_value = Config::potentiometers_mid_pct_input_values[i],
          .reverse = Config::potentiometers_reverse[i]};
        pot_readers[i].init(pot_reader_config);
        input_controller_config.potentiometers[i] = &pot_readers[i];
    }
    input_controller.init(input_controller_config);

    esp_now_handler.init();

    TransceiverConfig transceiver_config = {.input_controller = &input_controller,
      .esp_now_handler = &esp_now_handler,
      .update_delay_ms = Config::Transceiver::update_delay_ms};
    transceiver.init(transceiver_config);

    ota_handler.init();

    TelnetStream.begin();
}
void loop() {
    transceiver.run();
    ota_handler.run();
}
