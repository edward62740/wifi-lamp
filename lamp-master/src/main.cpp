#include <Arduino.h>
#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>
#include <ESP8266WiFi.h>
#include <espnow.h>
#include <EspnowConnectionManager.h>


/* InfluxDB Connection Parameters */
#define WIFI_SSID "OPERATIONALLY READY <80 DAYS"
#define WIFI_PASSWORD "2e9fc16a"
#define INFLUXDB_URL "https://us-west-2-1.aws.cloud2.influxdata.com"
// #define INFLUXDB_TOKEN "YdMA18IpTRQhPhF7AEeaMAZy-pZpEPqTCUNu_qUsmEL_O1T1UxwaWuf2SURzfXBaLG4yOFyfjtNu-AVtDIDVDw==" test
#define INFLUXDB_TOKEN "qXo1pHPocwaFZ1ZSSlDllihJ6owI5WyU7dgHJd9o9q5TnKgLjpAYIToSQPaH4OJxc87ZG5DV55_I_0LGOJMB6A=="
#define INFLUXDB_ORG "theprovidentinquisition@gmail.com"
#define INFLUXDB_BUCKET "MESH"
#define TZ_INFO "CET-1CEST,M3.5.0,M10.5.0/3"

uint8_t broadcastAddress1[] = {0x3C, 0x71, 0xBF, 0xC3, 0xBF, 0xB0};
uint8_t broadcastAddress2[] = {0x24, 0x0A, 0xC4, 0xAE, 0xAE, 0x44};
uint8_t broadcastAddress3[] = {0x80, 0x7D, 0x3A, 0x58, 0xB4, 0xB0};

String hostname = "BLED MASTER";
InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);

void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}

void setup()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
  WiFi.setHostname(hostname.c_str());
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.println(WiFi.status());
    WiFi.disconnect();
    delay(5);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  }
  timeSync(TZ_INFO, "pool.ntp.org", "time.nis.gov");
  client.setHTTPOptions(HTTPOptions().httpReadTimeout(200));
  client.setHTTPOptions(HTTPOptions().connectionReuse(true));
  // Check server connection
  if (client.validateConnection())
  {
    Serial.println(client.getServerUrl());
  }
  else
  {
    Serial.println(client.getLastErrorMessage());
  }

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  esp_now_add_peer(broadcastAddress1, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}

void loop()
{
  // put your main code here, to run repeatedly:
}