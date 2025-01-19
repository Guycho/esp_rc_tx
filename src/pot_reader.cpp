#include "pot_reader.h"

PotReader::PotReader(){}
PotReader::~PotReader(){}

void PotReader::init(const PotReaderConfig &config){
    m_pin = config.pin;
    m_reverse = config.reverse;
    m_dead_band = Config::dead_band;
    m_min_input_value = config.min_input_value;
    m_max_input_value = config.max_input_value;
    m_min_output_value = -Config::max_output_value;
    m_max_output_value = Config::max_output_value;    
    pinMode(m_pin, INPUT);
    m_value = 0;
}

void PotReader::run(){
    float temp_value = Utils::constrain_float(analogRead(m_pin), m_min_input_value, m_max_input_value);
    temp_value = Utils::map_float(temp_value, m_min_input_value, m_max_input_value, m_min_output_value, m_max_output_value);
    int8_t sign = temp_value > 0 ? 1 : -1;
    if (abs(temp_value) < m_dead_band){
        m_value = 0;
        return;
    }
    float abs_temp = Utils::map_float(abs(temp_value), m_dead_band, Config::max_output_value, 0, Config::max_output_value);
    float temp_out_value = sign * abs_temp;
    m_value = Utils::constrain_float(temp_out_value, m_min_output_value, m_max_output_value);
    m_value = map(m_value, m_min_input_value, m_max_input_value, m_min_output_value, m_max_output_value);
    m_value = constrain(m_value, m_min_output_value, m_max_output_value);
    if (m_reverse){
        m_value = -m_value;
    }
}

bool PotReader::get_value(){return m_value;}
