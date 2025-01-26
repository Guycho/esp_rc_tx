#include "ota_handler.h"

OTAHandler::OTAHandler(const char *hostname, const char *credentials[][2], uint8_t num_networks) {
    m_hostname = hostname;
    m_credentials = credentials;
    m_num_networks = num_networks;
}
OTAHandler::~OTAHandler() {}

bool OTAHandler::init() {
    uint8_t avail_networks = WiFi.scanNetworks();
    const char **m_ssid = new const char *[m_num_networks];
    const char **m_password = new const char *[m_num_networks];
    for (uint8_t j = 0; j < m_num_networks; j++) {
        m_ssid[j] = m_credentials[j][0];
        m_password[j] = m_credentials[j][1];
        for (uint8_t i = 0; i < avail_networks; i++) {
            if (WiFi.SSID(i) == m_ssid[j]) {
                WiFi.begin(m_ssid[j], m_password[j]);
                break;
            }
        }
    }
    delay(1000);
    if (WiFi.status() != WL_CONNECTED) {
        return false;
    }
    ArduinoOTA.setHostname(m_hostname);
    ArduinoOTA.begin();
    return true;
}

void OTAHandler::run() { ArduinoOTA.handle(); }
