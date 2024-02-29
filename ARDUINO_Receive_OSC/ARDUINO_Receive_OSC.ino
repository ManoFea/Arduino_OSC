#include <WiFi.h>
#include <OSCMessage.h>

const char *ssid = "grogu-fi";  //--YOUR WIFI
const char *password = "uF0undM3"; //--YOUR WIFI PASSWORD
const int localPort = 9999;


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
  Udp.begin(localPort);
  Serial.println(WiFi.localIP());
  // Serial.printf("UDP server started at port %d\n", localPort);
  
}

void loop() {
  // Check if data has been received
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
      for (int i = 0; i < msgIn.size()+1; i++) {
        if (msgIn.isInt(i)) {
          
          Serial.print("Arg");
          Serial.print(i);
          Serial.print(": ");
          Serial.println(msgIn.getInt(i));
          


        } else if (msgIn.isFloat(i)) {
          Serial.print("Arg");
          Serial.print(i);
          Serial.print(": ");
          Serial.println(msgIn.getFloat(i));
        }
      }



    } else {
      Serial.println("Error parsing OSC message");
    }
  }
  delay(10);
}
