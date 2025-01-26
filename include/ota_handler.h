#ifndef OTA_HANDLER_H
#define OTA_HANDLER_H

#include <Arduino.h>
#include <ArduinoOTA.h>
#include <ESPmDNS.h>
#include <WiFi.h>
#include <Chrono.h>

class OTAHandler {
   public:
    OTAHandler(const char *hostname, const char *credentials[][2], uint8_t num_networks,
      uint16_t timeout_sec = 0, bool print_debug = false);
    ~OTAHandler();
    bool init();
    void run();

   private:
    Chrono m_update_timer;
    Chrono m_timeout;
    uint16_t m_timeout_sec;
    bool can_run = false;
    bool m_print_debug;
    const char *m_hostname;
    const char *(*m_credentials)[2];
    uint8_t m_num_networks;
};

#endif  // OTA_HANDLER_H