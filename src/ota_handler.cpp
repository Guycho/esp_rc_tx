#include "ota_handler.h"

OTAHandler::OTAHandler(const char *hostname, const char *credentials[][2], uint8_t num_networks,
  uint16_t timeout_sec, bool print_debug) {
    m_hostname = hostname;
    m_credentials = credentials;
    m_num_networks = num_networks;
    m_timeout_sec = timeout_sec;
    m_print_debug = print_debug;

    if (m_print_debug) {
        Serial.println("OTAHandler constructor called");
        Serial.print("Hostname: ");
        Serial.println(m_hostname);
        Serial.print("Number of networks: ");
        Serial.println(m_num_networks);
        Serial.print("Timeout (sec): ");
        Serial.println(m_timeout_sec);
    }
}

OTAHandler::~OTAHandler() {
    if (m_print_debug) {
        Serial.println("OTAHandler destructor called");
    }
}

bool OTAHandler::init() {
    if (m_print_debug) {
        Serial.println("OTAHandler init called");
    }

    uint8_t avail_networks = WiFi.scanNetworks();
    if (m_print_debug) {
        Serial.print("Available networks: ");
        Serial.println(avail_networks);
    }

    const char **m_ssid = new const char *[m_num_networks];
    const char **m_password = new const char *[m_num_networks];

    for (uint8_t j = 0; j < m_num_networks; j++) {
        m_ssid[j] = m_credentials[j][0];
        m_password[j] = m_credentials[j][1];
        bool try_next = false;
        for (uint8_t i = 0; i < avail_networks; i++) {
            if (WiFi.SSID(i) == m_ssid[j]) {
                try_next = true;
                break;
            }
        }
        if (try_next) {
            if (m_print_debug) {
                Serial.print("Trying to connect to ");
                Serial.println(m_ssid[j]);
                WiFi.begin(m_ssid[j], m_password[j]);
                break;
            }
        }
    }

    delay(1000);
    if (WiFi.status() != WL_CONNECTED) {
        if (m_print_debug) {
            Serial.println("Failed to connect to any network");
        }
        return false;
    }

    ArduinoOTA.setHostname(m_hostname);
    ArduinoOTA.begin();
    if (m_print_debug) {
        Serial.println("OTA initialized successfully");
    }
    return true;
}

void OTAHandler::run() { ArduinoOTA.handle(); }