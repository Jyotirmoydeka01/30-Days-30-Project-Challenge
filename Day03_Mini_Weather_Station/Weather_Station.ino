#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>


float temp;
float pressure;

// Your WiFi credentials
const char* ssid = "jyot";
const char* password = "ufju8614";

// I2C pins for BMP280
#define BMP_SDA D2
#define BMP_SCL D1

Adafruit_BMP280 bmp; // I2C interface
ESP8266WebServer server(80); // Web server on port 80

void setup() {
  Serial.begin(115200);
  Wire.begin(BMP_SDA, BMP_SCL);

  if (!bmp.begin(0x76)) {
    Serial.println("BMP280 not found!");
    while (1);
  }

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Main page
  server.on("/", []() {
    String html = R"rawliteral(
    <!DOCTYPE html>
    <html>
    <head>
      <meta charset="UTF-8">
      <title>IoT Monitor</title>
      <style>
        body { font-family: Arial; text-align: center; padding-top: 50px; }
        h2 { color: #333; }
        .data { font-size: 1.5em; }
      </style>
      <script>
        function fetchData() {
          fetch('/data')
            .then(response => response.json())
            .then(data => {
              document.getElementById('temp').innerText = data.temp + ' °C';
              document.getElementById('press').innerText = data.press + ' hPa';
            });
        }
        setInterval(fetchData, 4000); // update every 4 sec
        window.onload = fetchData;
      </script>
    </head>
    <body>
      <h2>🌡️ IoT Temperature Monitor</h2>
      <p class='data'>Temperature: <span id='temp'>--</span></p>
      <p class='data'>Pressure: <span id='press'>--</span></p>
    </body>
    </html>
    )rawliteral";

    server.send(200, "text/html", html);
  });

  // JSON data route
  server.on("/data", []() {
     temp = bmp.readTemperature();
     pressure = bmp.readPressure() / 100.0F;

    String json = "{\"temp\":";
    json += temp;
    json += ", \"press\":";
    json += pressure;
    json += "}";

    server.send(200, "application/json", json);
  });

  server.begin();
  Serial.println("Web server started.");
}

void loop() {
  server.handleClient();
  Serial.println(temp);
  Serial.println(pressure);
}

  
