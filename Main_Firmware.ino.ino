#include <WiFi.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>

// Motor PINs
#define ENA 2
#define IN1 4
#define IN2 5
#define IN3 18
#define IN4 19
#define ENB 21

// WiFi credentials
const char* ssid = "realme narzo 30";
const char* password = "Raam@546";

// WebSocket server on port 81
WebSocketsServer webSocket = WebSocketsServer(81);

// Car control variables
bool forward = false;
bool backward = false;
bool left = false;
bool right = false;
int speed = 200; // Default speed (0-255)

void setup() {
  Serial.begin(115200);
  
  // Initialize motor pins
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);
  
  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  Serial.println();
  Serial.println("WiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  // Start WebSocket server
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  Serial.println("WebSocket server started on port 81");
  
  // Stop car initially
  carStop();
}

void loop() {
  webSocket.loop();
  smartcar();
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      // Stop car when disconnected for safety
      forward = backward = left = right = false;
      break;
      
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d\n", num, ip[0], ip[1], ip[2], ip[3]);
        
        // Send welcome message
        webSocket.sendTXT(num, "{\"status\":\"connected\",\"message\":\"Car ready!\"}");
      }
      break;
      
    case WStype_TEXT:
      {
        Serial.printf("[%u] Received: %s\n", num, payload);
        
        // Parse JSON command
        DynamicJsonDocument doc(1024);
        deserializeJson(doc, payload);
        
        String command = doc["command"];
        
        if (command == "move") {
          forward = doc["forward"];
          backward = doc["backward"];
          left = doc["left"];
          right = doc["right"];
          
          Serial.printf("Move command - F:%d B:%d L:%d R:%d\n", forward, backward, left, right);
        }
        else if (command == "speed") {
          speed = doc["value"];
          speed = constrain(speed, 0, 255); // Ensure speed is in valid range
          Serial.printf("Speed set to: %d\n", speed);
        }
        else if (command == "stop") {
          forward = backward = left = right = false;
          Serial.println("Emergency stop!");
        }
        
        // Send acknowledgment
        String response = "{\"status\":\"ok\",\"speed\":" + String(speed) + "}";
        webSocket.sendTXT(num, response);
      }
      break;
      
    default:
      break;
  }
}

void smartcar() {
  if (forward && !backward) {
    if (left && !right) {
      carForwardLeft();
      Serial.println("Forward Left");
    }
    else if (right && !left) {
      carForwardRight();
      Serial.println("Forward Right");
    }
    else {
      carForward();
      Serial.println("Forward");
    }
  }
  else if (backward && !forward) {
    if (left && !right) {
      carBackwardLeft();
      Serial.println("Backward Left");
    }
    else if (right && !left) {
      carBackwardRight();
      Serial.println("Backward Right");
    }
    else {
      carBackward();
      Serial.println("Backward");
    }
  } 
  else if (left && !right && !forward && !backward) {
    carTurnLeft();
    Serial.println("Turn Left");
  }
  else if (right && !left && !forward && !backward) {
    carTurnRight();
    Serial.println("Turn Right");
  }
  else {
    carStop();
  }
}

void carForward() {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void carBackward() {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void carTurnLeft() {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void carTurnRight() {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void carForwardLeft() {
  analogWrite(ENA, speed/2);  // Reduce left motor speed
  analogWrite(ENB, speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void carForwardRight() {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed/2);  // Reduce right motor speed
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void carBackwardLeft() {
  analogWrite(ENA, speed/2);  // Reduce left motor speed
  analogWrite(ENB, speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void carBackwardRight() {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed/2);  // Reduce right motor speed
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void carStop() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

