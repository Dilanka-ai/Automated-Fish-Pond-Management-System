#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif

#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>

/****  sensor Settings *******/
#include "RTClib.h"

RTC_DS1307 rtc;


char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

int motor = 27;

///////////////////////////// TDS & Temparature//////////////
#include <OneWire.h>
#include <DallasTemperature.h>

#define Temp_pin 13

OneWire oneWire(Temp_pin);

DallasTemperature sensors(&oneWire);

int R1 = 1000;
int EC_Read = 12;
int ECPower = 14;

float Temp_C;
float Temp_F;
float Temp1_Value = 0;
float Temp_Coef = 0.019;
float Calibration_PPM = 1080 ;
float K = 0.29;
float PPM_Con = 0.5;
/////////////////////////////////////////////////////////////////////////////////////
float CalibrationEC = (Calibration_PPM * 2) / 1000;
float Temperature;
float EC;
float EC_at_25;
int ppm;
float A_to_D = 0;
float Vin = 5;
float Vdrop = 0;
float R_Water;
float Value = 0;

///////////Water Level////////////////////
#include <Wire.h>
#define echoPin 4
#define trigPin 2
long duration, distance;
int water_level;

////////////Ph////////////////////////
const int pH_sensor_pin = 25 ;
float phValue;

/**** LED Settings *******/
const int water_out_motor = 15; //Set water_out_motor pin as GPI15
const int water_in_motor = 5; //Set water_in_motor pin as GPI05

/********ph value adjust********///
int ph_adjust=0;


///////////mode/////////////
int mode_change = 0;

/****** WiFi Connection Details *******/
const char* ssid = "Dilanka";
const char* password = "aththanayaka";

/******* MQTT Broker Connection Details *******/
const char* mqtt_server = "8d7e7602bb974d3d8783679376b850ea.s1.eu.hivemq.cloud";
const char* mqtt_username = "dilankahashan";
const char* mqtt_password = "Amdh1997";
const int mqtt_port = 8883;

/**** Secure WiFi Connectivity Initialisation *****/
WiFiClientSecure espClient;

/**** MQTT Client Initialisation Using WiFi Connection *****/
PubSubClient client(espClient);

unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];

/****** root certificate *********/

static const char *root_ca PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw
TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh
cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4
WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu
ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY
MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc
h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+
0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U
A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW
T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH
B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC
B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv
KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn
OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn
jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw
qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI
rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV
HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq
hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL
ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ
3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK
NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5
ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur
TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC
jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc
oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq
4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA
mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d
emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=
-----END CERTIFICATE-----
)EOF";

/************* Connect to WiFi ***********/
void setup_wifi() {
  delay(10);
  Serial.print("\nConnecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("\nWiFi connected\nIP address: ");
  Serial.println(WiFi.localIP());
}

/************* Connect to MQTT Broker ***********/
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client-";   // Create a random client ID
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("connected");

      client.subscribe("feed");   // subscribe the topics here
      client.subscribe("water_out"); 
      client.subscribe("water_in"); 
      client.subscribe("ph_adjust_add"); 
      client.subscribe("ph_adjust_dec"); 
       client.subscribe("mode_Ec");

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");   // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

/***** Call back Method for Receiving MQTT messages and Switching LED ****/

void callback(char* topic, byte* payload, unsigned int length) {
  String incommingMessage = "";
  for (int i = 0; i < length; i++) incommingMessage+=(char)payload[i];

  Serial.println("Message arrived ["+String(topic)+"]"+incommingMessage);

  //--- check the incomming message
    if( strcmp(topic,"feed") == 0){
     if (incommingMessage.equals("1")){ 
     
   digitalWrite(motor, HIGH); // For only motor 
   Serial.println("Motor is on");
  delay(5000);
  digitalWrite(motor, LOW);   
    }
     digitalWrite(motor, LOW);     // Turn the motor off
  }
    if( strcmp(topic,"water_out") == 0){
     if (incommingMessage.equals("1")){ 
     
   digitalWrite(water_out_motor, HIGH); // For only motor 
   Serial.println("water out Motor is on");
  delay(5000);
  digitalWrite(water_out_motor, LOW);   
    }
     digitalWrite(water_out_motor, LOW);     // Turn the motor off
  }
    if( strcmp(topic,"water_in") == 0){
     if (incommingMessage.equals("1")){ 
     
   digitalWrite(water_in_motor, HIGH); // For only motor 
   Serial.println("water in Motor is on");
  delay(5000);
  digitalWrite(water_in_motor, LOW);   
    }
     digitalWrite(motor, LOW);     // Turn the motor off
  }
     if( strcmp(topic,"ph_adjust_add") == 0){
     if (incommingMessage.equals("1")){ 
     
  ph_adjust=ph_adjust+1;
  Serial.println("Add a ph");
  }
  }
  
       if( strcmp(topic,"ph_adjust_dec") == 0){
     if (incommingMessage.equals("1")){ 
     
  ph_adjust=ph_adjust-1;
  Serial.println("Dec a ph");
  
  }}
     if( strcmp(topic,"mode_Ec") == 0){
     if (incommingMessage.equals("1")){ 
      
mode_change = 1;

  }}
       if( strcmp(topic,"mode_Ec") == 0){
     if (incommingMessage.equals("1")){ 
      
mode_change = 0;

  }}
}

/**** Method for Publishing MQTT Messages **********/
void publishMessage(const char* topic, String payload , boolean retained){
  if (client.publish(topic, payload.c_str(), true))
      Serial.println("Message publised ["+String(topic)+"]: "+payload);
}

/**** Application Initialisation Function******/
void setup() {

  pinMode(water_in_motor, OUTPUT); //set up
  pinMode(water_out_motor, OUTPUT); //set up
  
  Serial.begin(9600);
  while (!Serial) delay(1);
  setup_wifi();

  #ifdef ESP8266
    espClient.setInsecure();
  #else
    espClient.setCACert(root_ca);      // enable this line and the the "certificate" code for secure connection
  #endif

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

    if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);

  }

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  pinMode(motor, OUTPUT); // Feeder motor

  //digitalWrite(motor, HIGH); // For only motor testing
  //delay(10000);
  //digitalWrite(motor, LOW);

  sensors.begin(); //// TDS& Temp
  pinMode(EC_Read, INPUT);
  pinMode(ECPower, INPUT);
  pinMode(Temp_pin, INPUT);

  ///////////////////////Water Level//////////////////////////
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
//////////////////////////////PH Sensor///////////////////////
pinMode(pH_sensor_pin, INPUT);

}

/******** Main Function *************/
void loop() {

  if (!client.connected()) reconnect(); // check if client is connected
  client.loop();


/********************Sensor settings**************/
if(mode_change==0){
 DateTime now = rtc.now();

  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" (");
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
  Serial.print(") ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.print(" , ");
  //delay(1000);

  int H = now.hour(); //Assign time to variables
  int M = now.minute();
  int S = now.second();

  if (H == 8 & M == 0 & S == 0) { /// 1st time feed
    digitalWrite(motor, HIGH);
    delay(10000);
    digitalWrite(motor, LOW);
  }
  if (H == 12 & M == 0 & S == 0) {//2nd time feed
    digitalWrite(motor, HIGH);
    delay(10000);
    digitalWrite(motor, LOW);
  }
   if (H == 18 & M == 0 & S == 0) {//3rd time feed
    digitalWrite(motor, HIGH);
    delay(10000);
    digitalWrite(motor, LOW);
  }
  else {
    digitalWrite(motor, LOW);
  }


  GetEC(); 
  delay(1000);


  ////////////////////////Water Level///////////////////////////////////
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  
  distance = duration / 58.2;
  int disp = int(distance);
  int water_level = map(disp, 30,10, 0, 100);

  Serial.print("Distance: ");
  Serial.print(water_level);
  Serial.print(" cm");

  ////////////////////////PH Sensor/////////////////
long sensor_value = analogRead(pH_sensor_pin); 
float phValue =  map(sensor_value,0,4100,1,14);
phValue=phValue+ph_adjust;
Serial.print(" ,PH : ");
Serial.println(sensor_value);
delay(1000);
}

if(mode_change==0){
  GetEC(); 
  delay(1000);

}
/***************Data Publishing************/

  DynamicJsonDocument doc(1024);

  doc["deviceId"] = "ESP32-1";
  doc["siteId"] = "FPMS";
  doc["tds"] = EC_at_25;
  doc["temperature"] = Temperature;
  doc["ph"] = phValue;
  doc["water_level"] = water_level;

  
  char mqtt_message[128];
  serializeJson(doc, mqtt_message);

  publishMessage("esp32_data", mqtt_message, true);
  delay(5000);

}

void GetEC(){
  sensors.requestTemperatures();
  Temp1_Value = sensors.getTempCByIndex(0);
  Temperature = Temp1_Value;

  digitalWrite(EC_Read, HIGH);
  A_to_D = analogRead( ECPower);
  delay(1000);
  digitalWrite(ECPower, LOW);
  Vdrop = 3.25+(Vin * A_to_D) / 1024.0;
  R_Water = (Vdrop * R1) / (Vin - Vdrop);
  EC = 1000 / (R_Water * K);
  EC_at_25 = EC / (1 + Temp_Coef * (Temperature - 25.0));
  EC_at_25 = EC_at_25*100;
  ppm = ((EC_at_25)* PPM_Con);
  ppm = (ppm/1000);
  
  Serial.print(" EC: ");
  Serial.print(EC_at_25);
  Serial.print("Analog: ");
  Serial.print(A_to_D);
  Serial.print(" microSiemens(uS/cm) ");
  Serial.print(Temperature);
  Serial.println(" *C ");
}
