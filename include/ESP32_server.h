#ifndef ESP32_SERVER_H
#define ESP32_SERVER_H

#include <ArduinoJson.h>
#include <WebServer.h>
#include <WiFi.h>

struct ESP32ServerConfig {
    const char* ssid;
    const char* password;
};

class ESP32Server {
   public:
    ESP32Server();
    ~ESP32Server();

    void init(const ESP32ServerConfig& config);
    void handle_client();
    void update_data(String json);

   private:
    WebServer m_server;
    
    void handle_root();
    void handle_data();
    void handle_not_found();

    String m_json;
    uint16_t m_update_delay_ms;
};

#endif  // ESP32SERVER_HPP