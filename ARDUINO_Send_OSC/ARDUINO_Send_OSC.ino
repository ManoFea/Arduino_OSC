#include <WiFi.h>
#include <OSCMessage.h>

const char *ssid = "grogu-fi";  //--YOUR WIFI
const char *password = "uF0undM3"; //--YOUR WIFI PASSWORD
IPAddress ipAddress(192, 168, 2, 17);  //--YOUR TARGET IP ADDRESS
int port = 8000;

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

  // Start the UDP connection
  Udp.begin(7777);  // Set the desired OSC server port
}

void loop() {
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
  Udp.beginPacket(ipAddress, port); // Replace with your computer's IP address
  msg.send(Udp);
  Udp.endPacket();

  // Print the sensor value to Serial for debugging
  Serial.print("Sensor Value: ");
  Serial.println(sensorValue);

  delay(100); // Adjust the delay as needed
}
