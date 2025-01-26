#ifndef OTA_HANDLER_H
#define OTA_HANDLER_H

#include <Arduino.h>
#include <ArduinoOTA.h>
#include <ESPmDNS.h>
#include <WiFi.h>

class OTAHandler {
   public:
    OTAHandler(const char *hostname, const char *credentials[][2], uint8_t num_networks);
    ~OTAHandler();
    bool init();
    void run();

   private:
    const char *m_hostname;
    const char *(*m_credentials)[2];
    const char **m_ssid;
    const char **m_password;
    uint8_t m_num_networks;
};

#endif  // OTA_HANDLER_H