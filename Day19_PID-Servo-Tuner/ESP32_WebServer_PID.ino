#include <WiFi.h>
#include <WebServer.h>

// Wi-Fi Credentials
const char* ssid = "jyot";
const char* password = "ufju8614";

// PID Values
float Kp = 1.0, Ki = 0.0, Kd = 0.0;

// Create Web Server on port 80
WebServer server(80);

// HTML Page Template
const char* htmlPage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>PID Tuner</title>
  <style>
    body { font-family: sans-serif; padding: 20px; background: #f0f0f0; }
    input { width: 80px; padding: 5px; margin: 5px; }
    button { padding: 10px 20px; }
  </style>
</head>
<body>
  <h2>PID Tuning Interface</h2>
  <form action="/set" method="GET">
    <label>Kp: </label><input type="number" step="0.01" name="kp" value="%KP%"><br>
    <label>Ki: </label><input type="number" step="0.01" name="ki" value="%KI%"><br>
    <label>Kd: </label><input type="number" step="0.01" name="kd" value="%KD%"><br><br>
    <button type="submit">Send to Arduino</button>
  </form>
</body>
</html>
)rawliteral";

// Function to inject values into HTML
String processor(const String& var) {
  if (var == "KP") return String(Kp, 2);
  if (var == "KI") return String(Ki, 2);
  if (var == "KD") return String(Kd, 2);
  return "";
}

void handleRoot() {
  String html = htmlPage;
  html.replace("%KP%", String(Kp, 2));
  html.replace("%KI%", String(Ki, 2));
  html.replace("%KD%", String(Kd, 2));
  server.send(200, "text/html", html);
}

void handleSet() {
  if (server.hasArg("kp")) Kp = server.arg("kp").toFloat();
  if (server.hasArg("ki")) Ki = server.arg("ki").toFloat();
  if (server.hasArg("kd")) Kd = server.arg("kd").toFloat();

  // Send to Nano over Serial
  Serial.printf("%.2f,%.2f,%.2f\n", Kp, Ki, Kd);

  // Redirect back to root page
  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {
  Serial.begin(9600);  // Serial to Nano
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected.");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/set", handleSet);
  server.begin();
}

void loop() {
  server.handleClient();
}
