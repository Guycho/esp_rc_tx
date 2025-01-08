#include "ESP32_server.h"

ESP32Server::ESP32Server() {}

ESP32Server::~ESP32Server() {}

void ESP32Server::init(const ESP32ServerConfig& config) {

    // Scan for available networks
    Serial.println("Scanning for available networks...");
    int numNetworks = WiFi.scanNetworks();
    if (numNetworks == -1) {
        Serial.println("Failed to scan networks");
        return;
    }

    Serial.printf("Found %d networks:\n", numNetworks);
    for (int i = 0; i < numNetworks; ++i) {
        Serial.printf("%d: %s, Signal Strength: %d dBm \n", i + 1,
          WiFi.SSID(i).c_str(), WiFi.RSSI(i));
    }

    // Connect to the specified network in station mode
    WiFi.begin(config.ssid, config.password);
    Serial.print("Connecting to ");
    Serial.println(config.ssid);

    // Wait for connection
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(1000);
        Serial.print(".");
        attempts++;
    }
    Serial.println();

    if (WiFi.status() == WL_CONNECTED) {
        Serial.print("Connected to ");
        Serial.println(config.ssid);
        IPAddress IP = WiFi.localIP();
        Serial.print("Station IP address: ");
        Serial.println(IP);

        // Set up the web server
        m_server.on("/", HTTP_GET, std::bind(&ESP32Server::handle_root, this));
        m_server.on("/data", HTTP_GET, std::bind(&ESP32Server::handle_data, this));
        m_server.onNotFound(std::bind(&ESP32Server::handle_not_found, this));
        m_server.begin();
        Serial.println("Server started");
    } else {
        Serial.println("Failed to connect to Wi-Fi");
    }
}

void ESP32Server::handle_root() { m_server.send(200, "text/plain", "ESP32 Server"); }

void ESP32Server::handle_data() { m_server.send(200, "application/json", m_json); }

void ESP32Server::update_data(String json) { m_json = json; }
void ESP32Server::handle_not_found() { m_server.send(404, "text/plain", "File Not Found"); }

void ESP32Server::handle_client() { m_server.handleClient(); }
