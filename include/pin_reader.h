#ifndef PIN_READER_H
#define PIN_READER_H

#include <Arduino.h>
#include <Chrono.h>
#include <config.h>

struct PinReaderConfig {
    uint8_t pin;
    bool on_state;
};

class PinReader {
   public:
    PinReader();
    ~PinReader();

    void init(const PinReaderConfig &config);
    void run();
    bool get_state();
   private:
    Chrono m_debounce_timer;
    uint8_t m_pin;
    uint16_t m_debounce_delay_ms;
    bool m_on_state;
};
#endif  // PIN_READER_H
