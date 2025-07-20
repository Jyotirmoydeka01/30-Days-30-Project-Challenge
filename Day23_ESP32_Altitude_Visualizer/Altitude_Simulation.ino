#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>

//-- Replace with your network credentials
const char* ssid = "jyot";
const char* password = "ufju8614";

//-- Web Server on port 80
WebServer server(80);

//-- BMP280 Sensor object
Adafruit_BMP280 bmp;
float seaLevelPressure_hPa = 1013.25;

//-- Store the HTML, CSS, and JavaScript for the webpage in Flash Memory
const char HTML_PROGMEM[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<title>ESP32 Altitude Visualizer (cm)</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>
  body { font-family: Arial, sans-serif; text-align: center; background-color: #282c34; color: white; margin: 0; padding: 0; }
  h1 { margin-top: 20px; }
  #container {
    position: relative;
    width: 80%;
    max-width: 400px;
    height: 70vh;
    background-color: #3c414d;
    border: 2px solid #61dafb;
    border-radius: 10px;
    margin: 20px auto;
    overflow: hidden;
  }
  #altitude-scale {
    position: absolute;
    left: 0;
    top: 0;
    bottom: 0;
    width: 50px;
    border-right: 2px solid #abb2bf;
  }
  .scale-mark {
    position: absolute;
    width: 100%;
    color: #abb2bf;
    font-size: 12px;
    text-align: right;
    padding-right: 5px;
    box-sizing: border-box;
  }
  #altitude-block {
    position: absolute;
    bottom: 10px; /* Start position */
    left: 50%;
    transform: translateX(-50%);
    width: 60px;
    height: 40px;
    background-color: #61dafb;
    border-radius: 5px;
    transition: bottom 0.5s linear;
  }
  #altitude-text {
    font-size: 1.5em;
    font-weight: bold;
    color: #61dafb;
  }
</style>
</head>
<body>

  <h1>ESP32 Altitude Visualizer</h1>
  <p>Relative Altitude: <span id="altitude-text">--</span> cm</p>
  
  <div id="container">
    <div id="altitude-scale"></div>
    <div id="altitude-block"></div>
  </div>

<script>
  let baseAltitude = null;
  const pixelsPerCm = 0.2; // 0.2 pixels per cm → 5 pixels per 25 cm
  const container = document.getElementById('container');
  const scale = document.getElementById('altitude-scale');
  const block = document.getElementById('altitude-block');
  const altitudeText = document.getElementById('altitude-text');
  
  // Create markings on the scale in cm
  function createScale() {
    const containerHeight = container.clientHeight;
    const maxCm = Math.floor(containerHeight / pixelsPerCm);
    for (let i = 0; i <= maxCm; i += 50) { // Mark every 50 cm
        const mark = document.createElement('div');
        mark.className = 'scale-mark';
        mark.style.bottom = (i * pixelsPerCm) + 'px';
        mark.textContent = i + ' cm';
        scale.appendChild(mark);
    }
  }

  // Function to get new data from ESP32
  async function getAltitude() {
    try {
      const response = await fetch('/altitude');
      const altValue = await response.text();
      const altitude = parseFloat(altValue); // already in cm

      if (baseAltitude === null) {
        baseAltitude = altitude; // Set ground level on first successful fetch
      }

      updateVisuals(altitude);
    } catch (error) {
      console.error('Error fetching altitude:', error);
    }
  }

  // Update block position and text
  function updateVisuals(currentAltitude) {
    const altitudeChange = currentAltitude - baseAltitude; // relative cm
    const newBottomPosition = 10 + (altitudeChange * pixelsPerCm);

    block.style.bottom = newBottomPosition + 'px';
    altitudeText.textContent = altitudeChange.toFixed(1);
  }

  // Run on page load
  window.onload = function() {
    createScale();
    setInterval(getAltitude, 1000); // Fetch data every 1 second
  };
</script>

</body>
</html>
)rawliteral";

//-- Handle root URL
void handleRoot() {
  server.send(200, "text/html", HTML_PROGMEM);
}

//-- Handle /altitude data request (send altitude in centimeters)
void handleAltitude() {
  float altitudeCm = bmp.readAltitude(seaLevelPressure_hPa) * 100.0; // m → cm
  server.send(200, "text/plain", String(altitudeCm, 1));
}

//-- Handle 404 Not Found
void handleNotFound() {
  server.send(404, "text/plain", "Not Found");
}

void setup() {
  Serial.begin(115200);

  // Initialize BMP280
  if (!bmp.begin(0x76)) {
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1);
  }

  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Define server routes
  server.on("/", handleRoot);
  server.on("/altitude", handleAltitude);
  server.onNotFound(handleNotFound);

  // Start server
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
