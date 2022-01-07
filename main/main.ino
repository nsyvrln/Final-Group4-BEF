#include "hc.cpp"
HC hc;
#include "mpu.cpp"
mpu MPU;

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include<Wire.h>

const char* ssid = "Berkany";
const char* password = "Adalet34";

const char* mqtt_server = "54.224.57.65";
long lastMsg = 0;
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  hc.begin(12,13);

  Wire.begin();
  MPU.begin();

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}
float distance;
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  distance = hc.GetDistance();
  if (distance < 2000) {
    String mes = String(distance) + " cm";
    
    client.publish("Group4/distance", mes.c_str());
    Serial.print("Distance: ");
    Serial.println(distance);
  }
  Data SensorData=MPU.getData();
  String Mes="x="+String(SensorData.roll)+" y="+String(SensorData.yaw)+" z="+String(SensorData.pitch);
  Serial.println(Mes);
client.publish("group4/Gyro", Mes.c_str());
    

delay(200);

}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length) {

  Serial.print(topic);

  String messageTemp;

  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }



  if (String(topic) == "group1/ntc") {
  Serial.println("group1/ntc: "+messageTemp);
  }
  else if (String(topic) == "group1/pulse") {
  Serial.println("group1/pulse: "+messageTemp);
  }
  else if (String(topic) == "group2/temp") {
  Serial.println("group2/temp: "+messageTemp);
  }
  else if (String(topic) == "group2/humidity") {
  Serial.println("group2/humidity: "+messageTemp);
  }
   else if (String(topic) == "group2/pressure") {
  Serial.println("group3/humidity: "+messageTemp);
  }
  else if (String(topic) == "group2/color") {
  Serial.println("group3/color: "+messageTemp);
  }
  else if (String(topic) == "group5/Temp") {
  Serial.println("group5/Temp: "+messageTemp);
  }
  else if (String(topic) == "group5/flow") {
  Serial.println("group5/flow: "+messageTemp);
  }
  else if (String(topic) == "group3/temp") {
  Serial.println("group3/temp: "+messageTemp);
  }
  else if (String(topic) == "group3/humidity") {
  Serial.println("group3/humidity: "+messageTemp);
  }
  else if (String(topic) == "group3/color") {
  Serial.println("group3/color: "+messageTemp);
  }
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("gyrohc")) {
      Serial.println("connected");
      // Subscribe
     client.subscribe("group1/ntc");
      client.subscribe("group1/pulse");
      client.subscribe("group2/humidity");
      client.subscribe("group2/temp");
       client.subscribe("group2/pressure");
      client.subscribe("group2/color");
      client.subscribe("group5/Temp");
      client.subscribe("group5/flow");
      client.subscribe("group3/temp");
      client.subscribe("group3/humidity");
      client.subscribe("group3/color");




      
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
