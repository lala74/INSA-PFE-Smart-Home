#define Pin_Mouvement 18    //Pin 18
#define Pin_Luminier 19      //Pin 19
#define Pin_Temperature 21   //Pin 21

int sensor_luminote = 0;
int compteur_mouv = 0;

//Library
#include "DHT.h"
#include <PubSubClient.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include "time.h"


//WiFi (Connect ESP32 with WiFi)
const char* ssid = "Lala";                 // Your personal network SSID
const char* wifi_password = "minhduc12";     // Your personal network password

//MQTT configuration
const char* mqtt_server = "172.20.10.11";  // IP of the MQTT broker
const char* topic = "home/outdoor";
const char* mqtt_username = "baoLE"; // MQTT username
const char* mqtt_password = "12345678"; // MQTT password
const char* clientID = "sensorOutdoor"; // MQTT client ID

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;

char JSONmessageBuffer[300];

//variable global
float hum;
float temp;
float mouv;
float lum;


DHT dht(Pin_Temperature, DHT22);
// Initialise the WiFi and MQTT Client objects
WiFiClient wifiClient;
// 8883 is the listener port for the Broker
PubSubClient client(mqtt_server, 1883, wifiClient);

void MQTT_reconnect() {
  while (!client.connected()){
    Serial.print("Attempting MQTT connection...");
    if (client.connect(clientID, mqtt_username, mqtt_password)) {
      Serial.println("Connected to MQTT Broker!");
    }
    else {
      Serial.println("Connection to MQTT Broker failed...");
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
  /// Connect to MQTT
  
}


void setup() {
  Serial.begin(115200);
  dht.begin();
  Serial.println("Connecting to ");
  Serial.println(ssid);

  // Connect to the WiFi
  WiFi.begin(ssid, wifi_password);

  // Wait until the connection has been confirmed before continuing
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.println("IP: ");
  Serial.println(WiFi.localIP());

  while (!Serial) {}

  // Configurer des pins GPIOs
  pinMode(Pin_Mouvement, INPUT);
  pinMode(Pin_Luminier, INPUT);
  pinMode(Pin_Temperature, INPUT);

  Serial.println("Device Started");
  Serial.println("-------------------------------------");

  
  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  //printLocalTime();
  MQTT_reconnect();
  client.setServer(mqtt_server, 1883);

}

boolean readDateFromSensor(float *temp_h, float *temp_t, int *temp_mov,int *temp_lum) {
  bool check = false;
  //*temp_h = dht.readHumidity();
  //*temp_t = dht.readTemperature();

  *temp_h = rand() / (float) RAND_MAX*4+50;
  *temp_t = rand() / (float) RAND_MAX*4+10;

  if (digitalRead(Pin_Mouvement) == 0) {
    *temp_mov = 1;
  } else {
    *temp_mov = 0;
  }
  //*temp_lum = analogRead(Pin_Luminier);
  *temp_lum = rand() / (float) RAND_MAX * 1000 + 100;
  if (temp_h == 0 || temp_t == 0){
    return check;
  }
  return !check;
}

void makeData(){
  float temp_h = 0;
  float temp_t = 0;
  int temp_mov = 0;
  int temp_lum = 0;
  int count = 0;

  hum = 0;
  temp = 0;
  mouv = 0;
  lum = 0;

  for (int i = 0; i < 5; i++){
    if (readDateFromSensor(&temp_h,&temp_t,&temp_mov,&temp_lum)){
      hum += temp_h;
      temp += temp_t;
      mouv += temp_mov;
      lum += temp_lum;
      count++;
    }
    delay(3000);
  }

  hum /= count;
  temp /= count;
  mouv /= count;
  lum /= count;

}

void MQTT_Send_data() {
  // MQTT can only transmit strings so we convert float to strings

  if (!client.connected()) {
    MQTT_reconnect();
  }
  //client.loop();

  Serial.println("Sending message to MQTT topic..");
  Serial.println(JSONmessageBuffer);
  
  //Publish data to MQTT broker
  //if (client.publish(topic, String(data).c_str())) {
  if (client.publish(topic, JSONmessageBuffer) == true) {
    Serial.println("Success sending message");
  }
  else {
    Serial.println("data failed to send. Reconnecting to MQTT Broker and trying again");
  }
}

void encoding_Json(){
  StaticJsonBuffer<300> JSONbuffer;
  JsonObject& JSONencoder = JSONbuffer.createObject();
  JSONencoder["device"] = "ESP32";
  JSONencoder["sensorType"] = clientID;
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  char timestamp[30];
  strftime(timestamp,30, "%y-%m-%d %H:%M:%S", &timeinfo);
  
  JSONencoder["timestamp"] = timestamp;
  
  JSONencoder["temperature"] = temp;
  JSONencoder["humidity"] = hum;
  JSONencoder["mouvement"] = mouv;
  JSONencoder["luminosity"] = lum;

  JSONencoder.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
}

void makeMesuringCycle(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  char timestamp[30];
  strftime(timestamp,30, "%S", &timeinfo);
  int timeToi = atoi(timestamp);
  if (timeToi == 0 || timeToi ==30 || timeToi== 45 || timeToi == 15){
    makeData();
    encoding_Json();
    MQTT_Send_data();
  }
}

void loop() {

  makeMesuringCycle();

 
}
