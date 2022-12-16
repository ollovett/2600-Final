
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#include <WiFi.h>
#include <PubSubClient.h>

// WiFi
const char *ssid = ""; // keeping private
const char *password = "";  // keeping private

// MQTT Broker
const char *mqtt_broker = "broker.emqx.io";
const char *topic = "esp32/test";
const char *mqtt_username = "emqx";
const char *mqtt_password = "public";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient)

const uint16_t recvPin = 13; // Infrared receiving pin
IRrecv irrecv(recvPin);      // Create a class object used to receive class
decode_results results;       // Create a decoding results class object

//each led pin represents the postion on the tictactoe board
int ledPin1 = 15;
int ledPin2 = 2;
int ledPin3 = 0;
int ledPin4 = 4;
int ledPin5 = 5;
int ledPin6 = 18;
int ledPin7 = 19;
int ledPin8 = 21;
int ledPin9 = 22;

void setup() {
  //set the channel,frequency,esolution_bits
  ledcSetup(0,1000,8);
  ledcSetup(1,1000,8);
  ledcSetup(2,1000,8);
  ledcSetup(3,1000,8);
  ledcSetup(4,1000,8);
  ledcSetup(5,1000,8);
  ledcSetup(6,1000,8);
  ledcSetup(7,1000,8);
  ledcSetup(8,1000,8);

  //attach the channel to GPIO pin
  ledcAttachPin(ledPin1,0);
  ledcAttachPin(ledPin2,1);
  ledcAttachPin(ledPin3,2);
  ledcAttachPin(ledPin4,3);
  ledcAttachPin(ledPin5,4);
  ledcAttachPin(ledPin6,5);
  ledcAttachPin(ledPin7,6);
  ledcAttachPin(ledPin8,7);
  ledcAttachPin(ledPin9,8);
  irrecv.enableIRIn();        // Start the receiver

  // Set software serial baud to 115200;
  Serial.begin(115200);
  // connecting to a WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  //connecting to a mqtt broker
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
  while (!client.connected()) {
      String client_id = "esp32-client-";
      client_id += String(WiFi.macAddress());
      Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
      if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
          Serial.println("Public emqx mqtt broker connected");
      } else {
          Serial.print("failed with state ");
          Serial.print(client.state());
          delay(2000);
      }
  }
  // publish and subscribe
  client.publish(topic, "Hi EMQX I'm ESP32 ^^");
  client.subscribe(topic);
  }
}

void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
      Serial.print((char) payload[i]);
  }
  Serial.println();
  Serial.println("-----------------------");
}

void loop() {
  if (irrecv.decode(&results)) {        // Waiting for decoding
    handleControl(results.value);       // Handle the commands from remote control
    irrecv.resume();                    // Receive the next value
  }
  client.loop();
}

void handleControl(unsigned long value) {
  // Handle the commands
  switch (value) {
    case 0xFF30CF:              // Receive the number '1'
      ledcWrite(0, 255);
      break;
    case 0xFF18E7:              // Receive the number '2'
      ledcWrite(1, 255);
      break;
    case 0xFF7A85:              // Receive the number '3'
      ledcWrite(2, 255);
      break;
    case 0xFF10EF:              // Receive the number '4'
      ledcWrite(3, 255);
      break;
    case 0xFF38C7:              // Receive the number '5'
      ledcWrite(4, 255);
      break;
    case 0xFF5AA5:              // Receive the number '6'
      ledcWrite(5, 255);
      break;
    case 0xFF42BD:              // Receive the number '7'
      ledcWrite(6, 255);  
      break;
    case 0xFF4AB5:              // Receive the number '8'
      ledcWrite(7, 255);  
      break;
    case 0xFF52AD:              // Receive the number '9'
      ledcWrite(8, 255);  
      break;
  }
}
