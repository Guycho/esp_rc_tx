#ifndef PIN_READER_H
#define PIN_READER_H

#include <Arduino.h>
#include <Chrono.h>
#include <config.h>

struct PinReaderConfig {
    uint8_t pin;
    bool on_state;
    bool type_event;
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
    uint16_t m_debounce_delay_ms;

    uint8_t m_pin;
    bool m_on_state;
    bool m_state;
    bool m_type_event;
    bool m_new_event;
    bool m_was_false;
};
#endif  // PIN_READER_H
