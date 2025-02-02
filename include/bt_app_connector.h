#ifndef BT_APP_CONNECTOR_H
#define BT_APP_CONNECTOR_H

#include <Arduino.h>
#include <BluetoothSerial.h>
#include <Chrono.h>

#include "transceiver.h"

struct BTAppConnectorConfig {
    Transceiver *transceiver;
    BluetoothSerial *bt_serial;
    String device_name;
    uint8_t update_rate_hz;
};

class BTAppConnector {
   public:
    BTAppConnector();
    ~BTAppConnector();

    void init(const BTAppConnectorConfig &config);
    void run();

   private:
    Chrono m_data_timer;
    BluetoothSerial *m_bt_serial;
    Transceiver *m_transceiver;
    bool m_is_connected;
    uint16_t m_update_delay_ms;
    float m_max_g_force = 5.0;
    uint16_t m_max_rpm = 5000;
};

#endif  // BT_APP_CONNECTOR_H