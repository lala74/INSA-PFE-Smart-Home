#define Pin_Mouvement 18    //Pin 18
#define Pin_Luminier 34      //Pin 34
#define Pin_Temperature 21   //Pin 21
#define ONBOARD_LED  2

int sensor_luminote = 0;
int compteur_mouv = 0;

//Library
#include "DHT.h"
#include <PubSubClient.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include "time.h"


//WiFi (Connect ESP32 with WiFi)
const char* ssid = "CoCuDo";                 // Your personal network SSID
const char* wifi_password = "quocbaaoo";     // Your personal network password

//MQTT configuration
const char* mqtt_server = "192.168.43.237";  // IP of the MQTT broker
const char* topic = "home/indoor";
const char* mqtt_username = "baoLE"; // MQTT username
const char* mqtt_password = "12345678"; // MQTT password
const char* clientID = "sensorIndoor"; // MQTT client ID

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

void blick_led_n_times(int n) {
  for (int i=0; i < n;++i) {
  digitalWrite(ONBOARD_LED,HIGH);
  delay(100);
  digitalWrite(ONBOARD_LED,LOW);
  delay(100);
  }
}

void MQTT_reconnect() {
  while (!client.connected()){
    Serial.print("Attempting MQTT connection...");
    if (client.connect(clientID, mqtt_username, mqtt_password)) {
      Serial.println("Connected to MQTT Broker!");
      blick_led_n_times(5);
    }
    else {
      Serial.println("Connection to MQTT Broker failed...");
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(1000);
    }
 }
  /// Connect to MQTT  
}

void Wifi_reconnect(){
  while (WiFi.status() != WL_CONNECTED){
    Serial.println("Connecting to ");
    Serial.println(ssid);
  
    // Connect to the WiFi
    WiFi.begin(ssid, wifi_password);
  
    // Wait until the connection has been confirmed before continuing
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
      WiFi.begin(ssid, wifi_password);
    }
    Serial.println("WiFi connected");
    Serial.println("IP: ");
    Serial.println(WiFi.localIP());
  }
}

void reconnect(){
  
    Wifi_reconnect();
    delay(500);
    MQTT_reconnect();

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
    WiFi.begin(ssid, wifi_password);
  }
  Serial.println("WiFi connected");
  Serial.println("IP: ");
  Serial.println(WiFi.localIP());

  while (!Serial) {}

  // Configurer des pins GPIOs
  pinMode(Pin_Mouvement, INPUT);
  pinMode(Pin_Luminier, INPUT);
  pinMode(Pin_Temperature, INPUT);
  pinMode(ONBOARD_LED,OUTPUT);

  Serial.println("Device Started");
  Serial.println("-------------------------------------");

  
  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  //printLocalTime();
  //connect to MQTT
  delay(500);
  MQTT_reconnect();


}

boolean readDateFromSensor(float *temp_h, float *temp_t, int *temp_mov,int *temp_lum) {
  bool check = false;

  //read temperature and humidity by using these functions of lib dht 
  *temp_h = dht.readHumidity();
  *temp_t = dht.readTemperature();
  // if the value digital of Pin Mouvement, detect one mouvement
  if (digitalRead(Pin_Mouvement) == 0) {
    *temp_mov = 1;
  } else {
    *temp_mov = 0;
  }
  // read value analog of Pin_Luminier (value returned by sensor Luminierity)
  *temp_lum = analogRead(Pin_Luminier);

  // uncomment for generating these random value
  /*    *temp_mov = (float )rand();
      *temp_lum = rand() / (float) RAND_MAX * 1000 + 100;
      *temp_h = rand() / (float) RAND_MAX*4+50;
      *temp_t = rand() / (float) RAND_MAX*4+10; 
  */
  // if there are one of the value being equal 0, this reading is ignored
  if (temp_h == 0 || temp_t == 0 || temp_lum == 0){
    return check;
  }
  return !check;
}

// if there is at least one time detecting mouvement, we can indicate that there is monvement dectected.
// make value between 0 and 1000 (0 : darkest and 1000: lightest)
float processingLuminier(float dt){
  return 1000 - (dt / 4095) * 1000;
}

// if there is at least one time detecting mouvement, we can indicate that there is monvement dectected.
float procesingMouvement(float dt){
  if (dt != 0) {
    return 1;  
  } else
  return 0;
}

void makeData(){
  //initialise value for temporary variable
  float temp_h = 0;
  float temp_t = 0;
  int temp_mov = 0;
  int temp_lum = 0;
  int count = 0;

  //reset global value
  hum = 0;
  temp = 0;
  mouv = 0;
  lum = 0;

  //do mesure 5 times
  for (int i = 0; i < 5; i++){
    // if this mesure valid, increase global value
    if (readDateFromSensor(&temp_h,&temp_t,&temp_mov,&temp_lum)){
      hum += temp_h;
      temp += temp_t;
      mouv += temp_mov;
      lum += temp_lum;
      count++;
    }
    if (i < 4){
      delay(3000);
    } else{
      delay(0);
    }
  }

  //calcul moyen of value
  hum /= count;
  temp /= count;
  mouv /= count;
  lum /= count;

  //process mouvemet's et luminier's valeur 
  mouv = procesingMouvement(mouv);
  lum = processingLuminier(lum);

}

//sending data in form Json
void MQTT_Send_data() {

  reconnect();
  
  Serial.println("Sending message to MQTT topic..");
  Serial.println(JSONmessageBuffer);
  
  //Publish data to MQTT broker
  //if (client.publish(topic, String(data).c_str())) {
  if (client.publish(topic, JSONmessageBuffer) == true) {
    Serial.println("Success sending message");
    blick_led_n_times(1);
  }
  else {
    Serial.println("Data failed to send. Reconnecting and trying again");
    MQTT_Send_data();
  }
}


//encode in form Json for sending
void encoding_Json(){
  StaticJsonBuffer<300> JSONbuffer;
  JsonObject& JSONencoder = JSONbuffer.createObject();
  JSONencoder["device"] = "ESP32";
  JSONencoder["sensorType"] = clientID;
  struct tm timeinfo;
  while (!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    reconnect();
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

//make sure that we will do a mesurement at second 0-15-30-45
void makeMesuringCycle(){
  struct tm timeinfo;
  while (!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    reconnect();
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
