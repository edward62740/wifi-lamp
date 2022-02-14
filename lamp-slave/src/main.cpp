#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>
#include <EspnowConnectionManager.h>

String hostname = "BLED MASTER";

void OnDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len)
{
}

void setup()
{

  if (esp_now_init() != 0)
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
  yield();
}

void loop()
{
  // put your main code here, to run repeatedly:
}
