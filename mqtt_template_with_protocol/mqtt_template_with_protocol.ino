#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
 
// Connect to the WiFi
const char* ssid = //Wifi Username;
const char* password = //Wifi Password;
const char* mqtt_server = //IP Address;

bool state;

// Creating wifi, mqtt class instance
WiFiClient espClient;
PubSubClient client(espClient);


// Creating the callback function for handeling requests
void callback(char* topic, byte* payload, unsigned int length) {
   Serial.print("Message arrived [");
   Serial.print(topic);
   Serial.print("] ");
   String data="";
   if(length != 6){
    Serial.println("data not sufficient");
    }
   for (int i=0;i<length;i++) {
      char receivedChar = (char)payload[i];
      data += receivedChar;
  }
  Serial.println(data);
  control(data);
}


// Helper function
void num_to_state(int num){
  if(num > 0){
    state = HIGH;
    }
  else{
    state = LOW;
    }
}


//things to execute in this function
void control(String text){
  // Declaring the required variables for the communication
  String pinT;
  String voltT;
  int ad = 0;
  
  // Setting the length of the code recived
  int len = 6;
  
  // Variables for decoding the recived code
  int pinI;
  int voltI;

  // Decoding the text to get the specific values
  for (int i=0; i<2;i++){
      pinT += text[i];
      }
  for (int i=3; i<=len; i++){
    voltT += text[i];
    }
  ad = text[2] - '0';
  
  // Printing the data for debgging purpose
  Serial.println("");
  Serial.println("..............................................");
  Serial.println("");
  Serial.print("code recived: ");
  Serial.println(text);
  Serial.print("the pin number selected: ");
  Serial.println(pinT);
  Serial.print("digital or analog (0 is digital, 1 is analog): ");
  Serial.println(ad);
  Serial.print("the voltage to the selected pin: ");
  Serial.println(voltT);


  // Converting string version of volt and pin to int
  pinI = pinT.toInt();
  voltI = voltT.toInt();

  // Output in the pin
  if(ad == 1){
    analogWrite(pinI, voltI);
    }
  else{
    num_to_state(voltI);
    Serial.print("state of variable: ");
    Serial.println(state);
    digitalWrite(pinI,state);
    }

  // Console art
  Serial.println("");
  Serial.println("..............................................");
  Serial.println("");

  // Clearing the variables
  text = "";
  pinT = "";
  voltT = "";
  ad = 0;
}

void reconnect() {
   // Loop until we're reconnected
   while (!client.connected()) {
   Serial.print("Attempting MQTT connection...");
   // Attempt to connect
   if (client.connect("ESP8266 Client","home","kmpkokia1")) {
      Serial.println("connected");
      // ... and subscribe to topic
      client.subscribe("test");
   } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
   }
}
 
void setup(){
  Serial.begin(115200);
  Serial.println();

  // Setting pins on the board to output
  Serial.println("setting the pins up");
  for(int i=0;i>13;i++){
    pinMode(i,OUTPUT);
    }

  // Attempting to connect to the network
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
   Serial.println(" connected");

   // Setting the callback for request
   client.setServer(mqtt_server, 1883);
   client.setCallback(callback);
}
 
void loop()
{
    // Reconnect if connection is lost
   if (!client.connected()) {
      reconnect();
   }
   client.loop();
}
