//AsyncTCP, WebServer library from me-no-dev
//ESP32Servo from kevin harrington, jhon k. bennett
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESP32Servo.h> 

const char *ssid = "inferno";
const char *password = "dAN_BR0WNinferno";

int led1 = 26;
int led2 = 32;
int led3 = 33;

int servo1pin = 13;

Servo servo1;

int servo1pos = 90;

AsyncWebServer server(80);
AsyncWebSocket ws("/remote");

unsigned long lastMessageTime = 0;
const unsigned long messageTimeout = 500; // 2 seconds

void notifyClients() {
  ws.textAll(String("Notification"));
}


//Switch ON or OFF LED
void toggleLED(int pin) {
  int pinState = digitalRead(pin);
  pinState = !pinState; // Toggle the state (LOW to HIGH or HIGH to LOW)
  digitalWrite(pin, pinState); // Set the LED to the new state
}


void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
    lastMessageTime = millis(); // Reset the timeout timer
    AwsFrameInfo *info = (AwsFrameInfo *)arg;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
        data[len] = 0;
        const char *message = (char *)data;
        Serial.println(message);

        if (strncmp(message, "servo:", 6) == 0) {
            // Parse the position for servo
            char position[10];
            int servoValue;
            if (sscanf(message + 6, "%9[^,],value:%d", position, &servoValue) == 2) {
                servo1.write(servoValue);
                notifyClients();
            }
        } else if (strncmp(message, "led:", 4) == 0) {
            // Parse the LED number
            int ledNo;
            if (sscanf(message + 4, "%d", &ledNo) == 1) {
                toggleLED(ledNo);
                notifyClients();
            }
        } else if (strcmp(message, "ping") == 0) {
            ws.textAll("pong");
        }
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
  
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);

	// Allow allocation of all timers
	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
  servo1.setPeriodHertz(50);// Standard 50hz servo
  servo1.attach(servo1pin, 500, 2400);   // attaches the servo on pin 18 to the servo object
                                         // using SG90 servo min/max of 500us and 2400us
                                         // for MG995 large servo, use 1000us and 2000us,
                                         // which are the defaults, so this line could be
                                         // "myservo.attach(servoPin);"

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
  //if (millis() - lastMessageTime >= messageTimeout) {
  //  stopMotors();
  //}
}
