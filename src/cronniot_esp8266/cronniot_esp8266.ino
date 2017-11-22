/**
    Cronniot_ESP8266
    Purpose: Comunicate the ESP8266 controller with Cronniot IOT Service

    @author Cronniot
    @version 1.0 07/29/17
    @licence MIT
*/

#include <PubSubClient.h>
#include <ESP8266WiFi.h>

#define MQTT_SERVER "cronniot.com"
#define MQTT_PORT 8883

void callback(char* topic, byte* payload, unsigned int length);

typedef struct { 
  String deviceId;
  uint8_t pin;
} pinDevice;

//######## Information to fill ################

const char* ssid = "wifi_name";
const char* password = "wifi_password";          
const char* controllerId = "controller_id";           //ESP8266 identifier
const char* secretKey = "secret_key";                 //user secret code
const pinDevice digitPins[] { {"device_id", 10} };    //definition of pins to use: device Id / pin number

//###################################

WiFiClientSecure wifiClient;
PubSubClient client(MQTT_SERVER, MQTT_PORT, callback, wifiClient);

void setup() {
  Serial.begin(115200);
  for(uint8_t i = 0; i < sizeof(digitPins)/sizeof(pinDevice); ++i) {
      pinMode(digitPins[i].pin, OUTPUT);
      digitalWrite(digitPins[i].pin, LOW);
  }
  delay(100);
  WiFi.begin(ssid, password);
  reconnect();
  delay(2000);
}

void loop() {
  if (!client.connected() && WiFi.status() == 3) { reconnect(); }
  client.loop();
  delay(10);
}

void callback(char* topic, byte* payload, unsigned int length) {  
  String msDeviceId;
  String msSecretKey;
  bool readDevice = true;

  for(int i = 2; i < length; i++) {
      if(payload[i] == '_') { 
        readDevice = false;
        continue;
      }
      if(readDevice) { msDeviceId += (char)payload[i]; }
      else { msSecretKey += (char)payload[i]; }
  }

  char* outputQueue = new char[length + 3];
  strcpy(outputQueue, msDeviceId.c_str());
  strcat(outputQueue, "_");
  strcat(outputQueue, msSecretKey.c_str());
  strcat(outputQueue, "_out");
  
  if(msSecretKey != secretKey) {
    client.publish(outputQueue, "-1");
    return;
  }
  
  uint8_t pin = 0;
  uint8_t a = 0;
  const uint8_t arrLength = sizeof(digitPins)/sizeof(pinDevice);

  while(pin == 0 && a < arrLength) {
    if(digitPins[a].deviceId == msDeviceId) { pin = digitPins[a].pin; }
    else { a++; }      
  }

  if(pin == 0) {
    client.publish(outputQueue, "-1");
    return;
  }
  if(payload[0] == '1'){
    digitalWrite(pin, HIGH);             //turn the device pin 'on'
    client.publish(outputQueue, "1");
  }
  else if (payload[0] == '0'){
    digitalWrite(pin, LOW);              //turn the device pin 'off'
    client.publish(outputQueue, "0");
  }
  else if (payload[0] == 'S'){
    client.publish(outputQueue, String(digitalRead(pin)).c_str());
  }
}

/**
    Connect to the WIFI network and to the MQTT server
*/
void reconnect() {
  if(WiFi.status() != WL_CONNECTED) {
    Serial.print("Connecting to ");
    Serial.println(ssid);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");  
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }

  if(WiFi.status() == WL_CONNECTED){
    while (!client.connected()) {
      Serial.print("Attempting MQTT connection...");      
      uint8_t mac[6];
      WiFi.macAddress(mac);
      if (client.connect((char*) macToStr(mac).c_str(), "cronniotUser", "58e66fb21d07c414d6944677%6&")) {
        Serial.print("\tMTQQ Connected");
        client.subscribe(controllerId);
      }
      else { Serial.println("\tFailed."); abort(); }
    }
  }
}

/**
    Generate unique name from MAC address
    
    @param numeric array of mac address
    @return a String mac address
*/
String macToStr(const uint8_t* mac) {
  String result;
  for (uint8_t i = 0; i < 6; ++i) {
    result += String(mac[i], 16);
    if (i < 5) { result += ':'; }
  }
  return "esp8266-" + result;
}

