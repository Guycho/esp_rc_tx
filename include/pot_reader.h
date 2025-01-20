#ifndef POT_READER_H
#define POT_READER_H

#include <Arduino.h>
#include <Chrono.h>
#include "config.h"
#include "utils.h"
struct PotReaderConfig {
    uint8_t pin;
    uint16_t min_input_value;
    uint16_t max_input_value;
    float mid_pct_input_value;
    bool reverse;
};

class PotReader {
   public:
    PotReader();
    ~PotReader();

    void init(const PotReaderConfig &config);
    void run();
    bool get_value();
   private:
    uint8_t m_pin;
    bool m_reverse;
    float m_value;
    float m_dead_band;
    float m_min_input_value;
    float m_max_input_value;
    float m_mid_pct_input_value;
    float m_min_output_value;
    float m_max_output_value;

};
#endif  // PIN_READER_H
