#include "pin_reader.h"


PinReader::PinReader(){}
PinReader::~PinReader(){}

void PinReader::init(const PinReaderConfig &config){
    m_pin = config.pin;
    m_on_state = config.on_state;
    m_debounce_delay_ms = Config::debounce_delay_ms;
    if (config.on_state) {
        pinMode(m_pin, INPUT_PULLDOWN);
    } else {
        pinMode(m_pin, INPUT_PULLUP);
    }
    m_debounce_timer.start();
    m_state = digitalRead(m_pin);
}

void PinReader::run(){
    bool temp_state = digitalRead(m_pin);

    if (m_debounce_timer.hasPassed(m_debounce_delay_ms)) {
        m_state = temp_state;
    }
    if (temp_state == m_state) {
        m_debounce_timer.restart();
    }
}

bool PinReader::get_state(){
    return m_state == m_on_state;
}
