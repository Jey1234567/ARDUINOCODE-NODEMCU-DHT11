#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <DHT.h>

// Define DHT settings
#define DHTPIN D2          // Use the correct GPIO pin (D2 = GPIO4)
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// WiFi credentials
const char* ssid = "YOUR WIFI NAME";
const char* password = "YOUR WIFI PASSWORD";

//Replace with IP/port and API end post
const char* serverUrl = "http://192.168.xxx.xx:xxxx/api/temperature/temperaturepost";

//For online version
//const char* serverUrl ="https://...."

void setup() {
  Serial.begin(115200);
  dht.begin();
  WiFi.begin(ssid, password);

  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected!");
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  
  if (isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    delay(5000);
    return;
  }
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  if (WiFi.status() == WL_CONNECTED) {
    //For Local Version
    WiFiClient client;
   

    //For Online Version
    //WiFiClientSecure client; uncomment this
    // Disable SSL certificate verification
    //client.setInsecure(); uncomment this

    HTTPClient http;
    http.begin(client, serverUrl);
    http.addHeader("Content-Type", "application/json");

    // JSON in the form{ "temp": 25.6, 
    //                   "humid": 52}
    String json = "{\"temp\":" + String(temperature, 2) + ",\"humid\":" + String(humidity, 2) + "}";
    Serial.println(json); // log before POST


    int httpResponseCode = http.POST(json);
    Serial.print("POST Response: ");
    Serial.println(httpResponseCode);

    http.end();
  } else {
    Serial.println("WiFi disconnected.");
  }

  delay(5000); // send every 5 seconds
}
