#include <WiFi.h>
#include <OSCMessage.h>

const char *ssid = "grogu-fi";       // Your WiFi SSID
const char *password = "uF0undM3";   // Your WiFi password
IPAddress localIP(192, 168, 2, 17);  // Arduino's IP address
int sendPort = 8000;                  // Port to send OSC messages
int receivePort = 9999;               // Port to receive OSC messages

const int analogPin0 = A0;
const int digiPin4 = 4;
const int digiPin5 = 5;
const int digiPin6 = 6;
const int digiPin7 = 7;
const int digiPin8 = 8;

WiFiUDP Udp;

void setup() {
  Serial.begin(115200);
  delay(10);

  // Connect to Wi-Fi
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("Local IP Address: ");
  Serial.println(WiFi.localIP());

  // Start the UDP connection
  Udp.begin(receivePort);
  // Serial.printf("UDP server started at port %d\n", receivePort);
}

void loop() {
  // Check for incoming OSC messages
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    OSCMessage msgIn;
    while (packetSize--) {
      msgIn.fill(Udp.read());
    }

    if (!msgIn.hasError()) {
      // Get the OSC address and arguments
      const char *address = msgIn.getAddress();
      Serial.print("Received OSC message with address: ");
      Serial.println(address);

      // Print out the arguments
      for (int i = 0; i < msgIn.size(); i++) {
        if (msgIn.isInt(i)) {
          Serial.print("Arg ");
          Serial.print(i);
          Serial.print(": ");
          Serial.println(msgIn.getInt(i));
        } else if (msgIn.isFloat(i)) {
          Serial.print("Arg ");
          Serial.print(i);
          Serial.print(": ");
          Serial.println(msgIn.getFloat(i));
        }
      }
    } else {
      Serial.println("Error parsing OSC message");
    }
  }

  // Read the analog sensor value
  int sensorValue = analogRead(analogPin0);
  int digi1 = digitalRead(digiPin4);
  int digi2 = digitalRead(digiPin5);
  int digi3 = digitalRead(digiPin6);
  int digi4 = digitalRead(digiPin7);
  int digi5 = digitalRead(digiPin8);

  // Create an OSC message
  OSCMessage msg("/arduinoSend");
  msg.add(sensorValue);
  msg.add(digi1);
  msg.add(digi2);
  msg.add(digi3);
  msg.add(digi4);
  msg.add(digi5);

  // Send the OSC message
  Udp.beginPacket(localIP, sendPort);
  msg.send(Udp);
  Udp.endPacket();

  // Print the sensor value to Serial for debugging
  Serial.print("Sensor Value: ");
  Serial.println(sensorValue);

  delay(50); // Adjust the delay as needed
}
