#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

const char *ssid = "ssid";
const char *password = "password";

int motor1Pin1 = 27;
int motor1Pin2 = 26;
int enable1Pin = 14;

int motor2Pin1 = 32;
int motor2Pin2 = 33;
int enable2Pin = 25;

int motor1Speed = 0;
int motor2Speed = 0;

AsyncWebServer server(80);
AsyncWebSocket ws("/remote");

unsigned long lastMessageTime = 0;
const unsigned long messageTimeout = 500; // 2 seconds

void stopMotors() {
  motor1Speed = 0;
  motor2Speed = 0;
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
  analogWrite(enable1Pin, motor1Speed);
  analogWrite(enable2Pin, motor2Speed);
}

void notifyClients() {
  ws.textAll(String("Notification"));
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  lastMessageTime = millis(); // Reset the timeout timer
  AwsFrameInfo *info = (AwsFrameInfo *)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    if (strcmp((char *)data, "forward") == 0) {
      motor1Speed = 150;
      motor2Speed = 150;
      digitalWrite(motor1Pin1, LOW);
      digitalWrite(motor1Pin2, HIGH);
      digitalWrite(motor2Pin1, HIGH);
      digitalWrite(motor2Pin2, LOW);
      notifyClients();
    } else if (strcmp((char *)data, "backward") == 0) {
      motor1Speed = 150;
      motor2Speed = 150;
      digitalWrite(motor1Pin1, HIGH);
      digitalWrite(motor1Pin2, LOW);
      digitalWrite(motor2Pin1, LOW);
      digitalWrite(motor2Pin2, HIGH);
      notifyClients();
    } else if (strcmp((char *)data, "left") == 0) {
      motor1Speed = 100;
      motor2Speed = 100;
      digitalWrite(motor1Pin1, LOW);
      digitalWrite(motor1Pin2, HIGH);
      digitalWrite(motor2Pin1, LOW);
      digitalWrite(motor2Pin2, HIGH);
      notifyClients();
    } else if (strcmp((char *)data, "right") == 0) {
      motor1Speed = 100;
      motor2Speed = 100;
      digitalWrite(motor1Pin1, HIGH);
      digitalWrite(motor1Pin2, LOW);
      digitalWrite(motor2Pin1, HIGH);
      digitalWrite(motor2Pin2, LOW);
      notifyClients();
    } else if (strcmp((char *)data, "stop") == 0) {
      stopMotors();
      notifyClients();
    } else if (strcmp((char *)data, "ping") == 0) {
      ws.textAll("pong");
    }

    analogWrite(enable1Pin, motor1Speed);
    analogWrite(enable2Pin, motor2Speed);
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

void setup() {
  Serial.begin(115200);

  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);

  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(enable2Pin, OUTPUT);

  stopMotors(); // Initialize motors to a stopped state

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  initWebSocket();

  server.begin();

  Serial.println("Configuring Watch Dog Timer...");
}

void loop() {
  ws.cleanupClients();

  // Check if the timeout has been reached and stop motors if needed
  if (millis() - lastMessageTime >= messageTimeout) {
    stopMotors();
  }
}
