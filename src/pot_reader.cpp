#include "pot_reader.h"

PotReader::PotReader() {}
PotReader::~PotReader() {}

void PotReader::init(const PotReaderConfig &config) {
    m_pin = config.pin;
    m_reverse = config.reverse;
    m_dead_band = Config::potentiometers_dead_band;
    m_min_input_value = config.min_input_value;
    m_max_input_value = config.max_input_value;
    m_mid_pct_input_value = config.mid_pct_input_value;
    m_min_output_value = -Config::max_output_value;
    m_max_output_value = Config::max_output_value;
    analogSetWidth(Config::analog_read_resolution);
    analogReadResolution(Config::analog_read_resolution);
    adcAttachPin(m_pin);
    m_value = 0;
}

void PotReader::run() {
    float temp_value =
      Calcs::constrain_float(analogRead(m_pin), m_min_input_value, m_max_input_value);
    temp_value = m_reverse ? Calcs::map_float(temp_value, m_max_input_value, m_min_input_value,
                               m_min_output_value, m_max_output_value)
                           : Calcs::map_float(temp_value, m_min_input_value, m_max_input_value,
                               m_min_output_value, m_max_output_value);
    float mapped_value = temp_value > m_mid_pct_input_value
                             ? Calcs::map_float(temp_value, m_mid_pct_input_value, m_max_output_value,
                                                0, m_max_output_value)
                             : Calcs::map_float(temp_value, m_min_output_value, m_mid_pct_input_value,
                                                m_min_output_value, 0);
    m_value = Calcs::calc_dead_band(mapped_value, m_max_output_value, m_dead_band);
}
float PotReader::get_value() { return m_value; }
