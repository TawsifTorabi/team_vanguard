//Library Used - Arduino Web Sockets By Gil Maimon
#include <ArduinoWebsockets.h>
//Library - ESP32
#include <WiFi.h>

const char* ssid = "wifi-ssid";
const char* password = "wifi-password";
const char* websockets_server = "192.168.4.1"; // ESP IP address when it creates an AP
const int websockets_port = 81;

using namespace websockets;

WebsocketsServer server;

int motor1Pin1 = 27;
int motor1Pin2 = 26;
int enable1Pin = 14;

int motor2Pin1 = 32;
int motor2Pin2 = 33;
int enable2Pin = 25;

int motor1Speed = 0;
int motor2Speed = 0;

unsigned long lastPingTime = 0;
const unsigned long pingInterval = 5000; // Send ping every 5 seconds

void setup() {
  Serial.begin(115200);
  
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);
  
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(enable2Pin, OUTPUT);

  analogWrite(enable1Pin, motor1Speed);
  analogWrite(enable2Pin, motor2Speed);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Start WebSocket server
  server.listen(websockets_port);

  // Set up ping timer
  lastPingTime = millis();
}

void loop() {
  WebsocketsClient client = server.accept();
  
  if (client.available()) {
    WebsocketsMessage msg = client.readBlocking();
    Serial.println("Data received: " + msg.data());
    handleWebSocketData(msg.data());
  }
  
  // Poll the WebSocket server
  server.poll();
  
  // Send ping frames to keep the connection alive
  unsigned long currentTime = millis();
  if (currentTime - lastPingTime >= pingInterval) {
    client.ping();
    lastPingTime = currentTime;
  }
}

void handleWebSocketData(String data) {
  if (data == "forward") {
    motor1Speed = 150;
    motor2Speed = 150;
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH);
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);
  } else if (data == "backward") {
    motor1Speed = 150;
    motor2Speed = 150;
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH);
  } else if (data == "left") {
    motor1Speed = 100;
    motor2Speed = 100;
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH);
  } else if (data == "right") {
    motor1Speed = 100;
    motor2Speed = 100;
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);
  } else if (data == "stop") {
    motor1Speed = 0;
    motor2Speed = 0;
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, LOW);
  }

  analogWrite(enable1Pin, motor1Speed);
  analogWrite(enable2Pin, motor2Speed);
}
