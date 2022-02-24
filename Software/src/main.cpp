#include <Arduino.h>
#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>
#include <ESP8266WiFi.h>
#include <espnow.h>
#include <EspnowConnectionManager.h>
#include <ESP8266_ISR_Timer.h>

/* InfluxDB Connection Parameters */
#define WIFI_SSID ""
#define WIFI_PASSWORD ""
/* InfluxDB Connection Parameters */
#define INFLUXDB_URL ""
// InfluxDB v2 server or cloud API authentication token (Use: InfluxDB UI -> Data -> Tokens -> <select token>)
#define INFLUXDB_TOKEN ""
// InfluxDB v2 organization id (Use: InfluxDB UI -> User -> About -> Common Ids )
#define INFLUXDB_ORG ""
// InfluxDB v2 bucket name (Use: InfluxDB UI ->  Data -> Buckets)
#define INFLUXDB_BUCKET ""
#define TZ_INFO ""

uint8_t broadcastAddress1[] = {};

InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);
unsigned long previousMillis = 0;

typedef struct dataPayload
{
  uint32_t light = 0;
  uint8_t rssi = 255;
  tm time;
  bool connection = false;
};
struct dataPayload data;
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus)
{
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0)
  {
    Serial.println("Delivery success");
  }
  else
  {
    Serial.println("Delivery fail");
  }
}

void setup()
{
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.println(WiFi.status());
    delay(2500);
  }
}

void loop()
{
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= 60000)
  {
    previousMillis = currentMillis;
    String query = "from(bucket: \"MESH\") |> range(start: -10m) |> filter(fn: (r) => r._measurement == \"Environment\") |> filter(fn: (r) => r.UID == \"4100\") |> filter(fn: (r) => r._field == \"Light\") |> last()";
    FluxQueryResult result = client.query(query);
    while (result.next())
    {
      data.light = result.getValueByName("_value").getDouble();
      Serial.println((String)data.light);
    }
    data.connection = true;
    data.rssi = WiFi.RSSI();
  }
  if (data.light <= 30 && data.light > 2)
  {
    for (int i = 0; i < 10; i++)
    {
      analogWrite(4, random(80, 120));
      analogWrite(5, random(200, 254));
      delay(random(50, 120));
    }
  }
  else{
    analogWrite(4, 0);
  }
}