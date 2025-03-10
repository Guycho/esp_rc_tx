#include <Arduino.h>
#include <Chrono.h>
#include <ESP_now_handler.h>

#include "bt_app_connector.h"
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
Transceiver transceiver;
BluetoothSerial bt_serial;
BTAppConnector bt_app_connector;

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

    BTAppConnectorConfig bt_app_connector_config = {.transceiver = &transceiver,
      .bt_serial = &bt_serial,
      .device_name = Config::BTAppConnector::device_name,
      .update_rate_hz = Config::BTAppConnector::update_rate_hz};

    bt_app_connector.init(bt_app_connector_config);
}
void loop() {
    transceiver.run();
    bt_app_connector.run();
}
