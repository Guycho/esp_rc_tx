#include "pin_reader.h"

PinReader::PinReader() {}
PinReader::~PinReader() {}

void PinReader::init(const PinReaderConfig &config) {
    m_pin = config.pin;
    m_on_state = config.on_state;
    m_type_event = config.type_event;
    m_debounce_delay_ms = Config::debounce_delay_ms;
    if (config.on_state) {
        pinMode(m_pin, INPUT_PULLDOWN);
    } else {
        pinMode(m_pin, INPUT_PULLUP);
    }
    m_debounce_timer.start();
    m_state = digitalRead(m_pin);
}

void PinReader::run() {
    bool temp_state = digitalRead(m_pin) == m_on_state;
    if (!m_type_event) {
        if (m_debounce_timer.hasPassed(m_debounce_delay_ms)) {
            m_state = temp_state;
        }
        if (temp_state == m_state) {
            m_debounce_timer.restart();
        }
    } else {
        if (m_was_false && temp_state) {
            m_new_event = true;
        }
        if (!temp_state) {
            m_was_false = true;
        }
        if (m_new_event) {
            m_state = true;
        } else {
            m_state = false;
        }
    }
}

bool PinReader::get_state() {
    run();
    if (m_state) {
        m_new_event = false;
        m_was_false = false;
    }
    return m_state;
}
