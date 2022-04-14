#include <ESP8266WiFi.h> 
#include <PubSubClient.h>                                              
#include <SoftwareSerial.h>

#define WIFI_SSID "DE LA VICTORIA NETWORK-2.4GHZ"                                          
#define WIFI_PASSWORD "Isuzutrooper_grv208" 

SoftwareSerial mySerial (D6, D5); //RX, TX pins
SoftwareSerial mySerial2 (D8, D7); //RX, TX pins
//SoftwareSerial mySerial2 (D4, D3); //RX, TX pins
// Initializes the espClient. You should change the espClient name if you have multiple ESPs running in your home automation system
WiFiClient espClient;
PubSubClient espclient(espClient);

const char* mqtt_server = "test.mosquitto.org";


int period = 500;
unsigned long time_now = 0;
unsigned long delay_sched = 0;

String timeStatus;

int ultrasonicResult;
String ultrasonicRead;
int ultrasonicInt  = 0;
int percentage = 0;

int ultrasonicResultY;
String ultrasonicReadY;
int ultrasonicIntY = 0; 
int percentageY  = 0;

int ultrasonicResultZ;
String ultrasonicReadZ;
int ultrasonicIntZ = 0; 
int percentageZ = 0;

int timeResult;
String timeRead;
int timeInt;

String dateRead;

String feed1_0, feed1_1, feed1_2, feed2_0, feed2_1, feed2_2, feed1;
String wash1_0, wash1_1, wash1_2, wash2_0, wash2_1, wash2_2;

bool manual_feed_0p5KG, manual_feed_1KG, manual_feed_1p5KG;
bool scheduled_feed_0p5KG, scheduled_feed_1KG, scheduled_feed_1p5KG;

String timeTest = "7:30 PM";

bool x = false;

String check;

String msg1, msg2;

// feed 1 voids
void feed_1_on_0p5kg () {
  mySerial.write('a'); // Write integer 111 to PIC
  espclient.publish("cage_1/feed_1/switch", "OFF");
}

void feed_1_on_1kg () {
  mySerial.write('b'); // Write integer 112 to PIC
  espclient.publish("cage_1/feed_1/switch", "OFF");
}

void feed_1_on_1p5kg () {
  mySerial.write('c'); // Write integer 113 to PIC
  espclient.publish("cage_1/feed_1/switch", "OFF");
}

//feed 2 voids

void feed_2_on_0p5kg () {
  mySerial.write('d'); // Write integer 211 to PIC
  espclient.publish("cage_2/feed_2/switch", "OFF");
}

void feed_2_on_1kg () {
  mySerial.write('e'); // Write integer 212 to PIC
  espclient.publish("cage_2/feed_2/switch", "OFF");
}

void feed_2_on_1p5kg () {
  mySerial.write('f'); // Write integer 213 to PIC
  espclient.publish("cage_2/feed_2/switch", "OFF");
}

void wash_1 () {
  mySerial.write('g');
  espclient.publish("cage_1/wash_1/switch", "OFF");
}

void wash_2 () {
  mySerial.write('h');
  espclient.publish("cage_2/wash_2/switch", "OFF");
}

// This functions is executed when some device publishes a message to a topic that your NodeMCU is subscribed to

void callback(String topic, byte* message, unsigned int length) {
  //Serial.print("Message arrived on topic: ");
  //Serial.print(topic);
  //Serial.print(". Message: ");
  String messageInfo;
  
  for (int i = 0; i < length; i++) {
    //Serial.print((char)message[i]);
    messageInfo += (char)message[i];
  }
 //Serial.println();

  // CAGE 1 refresh
  if (topic=="cage_1/refresh") {
  Serial.print("------");
    if (messageInfo == "ON") {
      mySerial.write('s');
      Serial.print("refresh1");
    }
  }

  if (topic=="cage_2/refresh") {
    if (messageInfo == "ON") {
      mySerial.write('r');
      Serial.print("refresh2");
    }
  }

  // CAGE 1 Feed
  if(topic=="cage_1/feed_1"){
    if((messageInfo == "ON 0.5 KG") || (messageInfo == "0.5 KG ON")){
      msg1 =  "Manual feed 0.5KG at " + dateRead + ", " + timeRead;
      feed_1_on_0p5kg ();
      Serial.println("Cage 1 Manual feed 0.5 KG");
      }  
    if((messageInfo == "ON1 0.5 KG") || (messageInfo == "0.5 KG ON1")){
      msg1 = "Scheduled feed 0.5KG at " + dateRead + ", " + timeRead;
      feed_1_on_0p5kg ();
      Serial.println("Cage 1 Scheduled feed 0.5KG");
      }    
    if((messageInfo == "ON 1 KG") || (messageInfo == "1 KG ON")){
      msg1 = "Manual feed 1KG at " + dateRead + ", " + timeRead;
      feed_1_on_1kg ();
      Serial.println("Cage 1 Manual feed 1KG");
    }
    if((messageInfo == "ON1 1 KG") || (messageInfo == "1 KG ON1")){
      msg1 = "Scheduled feed 1KG at " + dateRead + ", " + timeRead;
      feed_1_on_1kg ();
      Serial.println("Cage 1 Scheduled feed 1KG");
    }
    if((messageInfo == "ON 1.5 KG") || (messageInfo == "1.5 KG ON")){
      msg1 = "Manual feed 1.5KG at " + dateRead + ", " + timeRead;
      feed_1_on_1p5kg ();
      Serial.println("Cage 1 Manual feed 1.5KG");
    }
    if((messageInfo == "ON1 1.5 KG") || (messageInfo == "1.5 KG ON1")){
      msg1 = "Scheduled feed 1.5KG at " + dateRead + ", " + timeRead;
      feed_1_on_1p5kg ();
      Serial.println("Cage 1 Scheduled feed 1.5KG");
    }
  }

  // CAGE 2 Feed
  if(topic=="cage_2/feed_2"){
    if((messageInfo == "ON 0.5 KG") || (messageInfo == "0.5 KG ON")){
      msg2 =  "Manual feed 0.5KG at " + dateRead + ", " + timeRead;
      feed_2_on_0p5kg ();
      Serial.println("Cage 2 Manual feed 0.5 KG");
      }  
    if((messageInfo == "ON1 0.5 KG") || (messageInfo == "0.5 KG ON1")){
      msg2 = "Scheduled feed 0.5KG at " + dateRead + ", " + timeRead;
      feed_2_on_0p5kg ();
      Serial.println("Cage 2 Scheduled feed 0.5KG");
      }    
    if((messageInfo == "ON 1 KG") || (messageInfo == "1 KG ON")){
      msg2 = "Manual feed 1KG at " + dateRead + ", " + timeRead;
      feed_2_on_1kg ();
      Serial.println("Cage 2 Manual feed 1KG");
    }
    if((messageInfo == "ON1 1 KG") || (messageInfo == "1 KG ON1")){
      msg2 = "Scheduled feed 1KG at " + dateRead + ", " + timeRead;
      feed_2_on_1kg ();
      Serial.println("Cage 2 Scheduled feed 1KG");
    }
    if((messageInfo == "ON 1.5 KG") || (messageInfo == "1.5 KG ON")){
      msg2 = "Manual feed 1.5KG at " + dateRead + ", " + timeRead;
      feed_2_on_1p5kg ();
      Serial.println("Cage 2 Manual feed 1.5KG");
    }
    if((messageInfo == "ON1 1.5 KG") || (messageInfo == "1.5 KG ON1")){
      msg2 = "Scheduled feed 1.5KG at " + dateRead + ", " + timeRead;
      feed_2_on_1p5kg ();
      Serial.println("Cage 2 Scheduled feed 1.5KG");
    }
  }

  // CAGE 1 Wash

  if (topic=="cage_1/wash_1") {
    if (messageInfo == "ON") {
      wash_1();
      Serial.println("Wash 1");
    }
  }

  if (topic=="cage_2/wash_2") {
    if (messageInfo == "ON") {
      wash_2();
      Serial.println("Wash 2");
    }
  }

  //CAGE 1 sched

  if(topic=="cage1_feed_sched/0"){
      feed1_0= messageInfo;
  }

  if(topic=="cage1_feed_sched/1"){
    feed1_1 = messageInfo;
  }

  if(topic=="cage1_feed_sched/2"){
    feed1_2 = messageInfo;
  }

  //CAGE 2 sched

  if(topic=="cage2_feed_sched/0"){
    feed2_0 = messageInfo;
  }

  if(topic=="cage2_feed_sched/1"){
    feed2_1 = messageInfo;
  }

  if(topic=="cage2_feed_sched/2"){
    feed2_2 = messageInfo; 
  }

  //cage 1 wash sched
  if(topic=="cage1_wash_sched/0"){
    wash1_0 = messageInfo;
  }

  if(topic=="cage1_wash_sched/1"){
    wash1_1 = messageInfo;
  }

  if(topic=="cage1_wash_sched/2"){
    wash1_2 = messageInfo;
  }

  // Cage 2 wash sched
  if(topic=="cage2_wash_sched/0"){
    wash2_0 = messageInfo;
  }

  if(topic=="cage2_wash_sched/1"){
    wash2_1 = messageInfo;
  }

  if(topic=="cage2_wash_sched/2"){
    wash2_2 = messageInfo;
  }
  //Serial.println();
}

// This functions reconnects your ESP8266 to your MQTT broker
// Change the function below if you want to subscribe to more topics with your ESP8266 
void reconnect() {
  // Loop until we're reconnected
  while (!espclient.connected()) {
    Serial.print("Attempting MQTT connection...");
    
    if (espclient.connect("ESP8266Client2")) {
      Serial.println("connected");  
      // Subscribe or resubscribe to a topic
      // You can subscribe to more topics (to control more LEDs in this example)
      espclient.subscribe("cage_1/feed_1");
      espclient.subscribe("cage_1/feed_1/switch");
      espclient.subscribe("cage_1/weight_1");
      espclient.subscribe("cage_1/logs");

      espclient.subscribe("cage_2/feed_2");
      espclient.subscribe("cage_2/feed_2/switch");
      espclient.subscribe("cage_2/weight_2");
      espclient.subscribe("cage_2/logs");

      espclient.subscribe("water_tank");
      espclient.subscribe("feed_tank_1");
      espclient.subscribe("feed_tank_2");

      espclient.subscribe("cage1_feed_sched/0");
      espclient.subscribe("cage1_feed_sched/1");
      espclient.subscribe("cage1_feed_sched/2");

      espclient.subscribe("cage2_feed_sched/0");
      espclient.subscribe("cage2_feed_sched/1");
      espclient.subscribe("cage2_feed_sched/2");

      espclient.subscribe("cage_1/wash_1");
      espclient.subscribe("cage_2/wash_2");

      espclient.subscribe("cage_1/wash_1/switch");
      espclient.subscribe("cage_2/wash_2/switch");

      espclient.subscribe("cage1_wash_sched/0");
      espclient.subscribe("cage1_wash_sched/1");
      espclient.subscribe("cage1_wash_sched/2");

      espclient.subscribe("cage2_wash_sched/0");
      espclient.subscribe("cage2_wash_sched/1");
      espclient.subscribe("cage2_wash_sched/2");

      espclient.subscribe("cage_1/refresh");
      espclient.subscribe("cage_2/refresh");
    } else {
      Serial.print("failed, rc=");
      Serial.print(espclient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
                                                              
void setup() 
{
  Serial.begin(9600);
  mySerial.begin(9600);
  mySerial2.begin(9600);
  
  delay(1000);                   
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                               
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
 
  pinMode(LED_BUILTIN, OUTPUT);
  espclient.setServer(mqtt_server, 1883);
  espclient.setCallback(callback);

}
 
void loop() 
{

if (mySerial2.read() == 't'){
      timeRead = mySerial2.readStringUntil('\n');
      Serial.println();
      Serial.print("time: ");
      Serial.print(timeRead); 
  }

if (mySerial2.read() == 'd'){
      dateRead = mySerial2.readStringUntil('\n');
      Serial.println();
      Serial.print("date: ");
      Serial.print(dateRead);
  }     

if (mySerial2.read() == 'x') {
      ultrasonicRead = mySerial2.readStringUntil('\r');
      ultrasonicInt = ultrasonicRead.toInt();
      Serial.println();
      Serial.print("Water Tank : ");
      Serial.print(ultrasonicInt);
      espclient.publish("water_tank", String(ultrasonicInt).c_str());
  }  

  if (Serial.read() == 'z') {
      ultrasonicReadZ = Serial.readStringUntil('\r');
      ultrasonicIntZ = ultrasonicReadZ.toInt();
      Serial.println();
      Serial.print("Feed Tank 2: ");
      Serial.print(ultrasonicIntZ);
      espclient.publish("feed_tank_2", String(ultrasonicIntZ).c_str());
      espclient.publish("cage_2/logs", msg2.c_str());
  }

if (mySerial.read() == 'y'){
      ultrasonicReadY = mySerial.readStringUntil('\r');
      ultrasonicIntY = ultrasonicReadY.toInt();
      Serial.println();
      Serial.print("Feed Tank 1: ");
      Serial.print(ultrasonicIntY);
      espclient.publish("feed_tank_1", String(ultrasonicIntY).c_str());
      espclient.publish("cage_1/logs", msg1.c_str());
  }

  if ((timeRead == feed1_0) || (timeRead == feed1_1) || (timeRead == feed1_2)) {
    Serial.println("feed 1 scheduled successful!");
    espclient.publish("cage_1/feed_1/switch", "ON1");
    if (timeRead == feed1_0) {feed1_0 = " ";}
    if (timeRead == feed1_1) {feed1_1 = " ";}
    if (timeRead == feed1_2) {feed1_2 = " ";}
  }

  if ((timeRead == feed2_0) || (timeRead == feed2_1) || (timeRead == feed2_2)) {
    Serial.println("feed 2 scheduled successful!");
    espclient.publish("cage_2/feed_2/switch", "ON1");
    if (timeRead == feed2_0) {feed2_0 = " ";}
    if (timeRead == feed2_1) {feed2_1 = " ";}
    if (timeRead == feed2_2) {feed2_2 = " ";}
  }

  if ((timeRead == wash1_0) || (timeRead == wash1_1) || (timeRead == wash1_2)) {
    Serial.println("wash 1 scheduled successful!");
    espclient.publish("cage_1/wash_1/switch", "ON");
    if (timeRead == wash1_0) {wash1_0 = " ";}
    if (timeRead == wash1_1) {wash1_1 = " ";}
    if (timeRead == wash1_2) {wash1_2 = " ";}
  }

  if ((timeRead == wash2_0) || (timeRead == wash2_1) || (timeRead == wash2_2)) {
    Serial.println("wash 2 scheduled successful!");
    espclient.publish("cage_2/wash_2/switch", "ON");
    if (timeRead == wash2_0) {wash2_0 = " ";}
    if (timeRead == wash2_1) {wash2_1 = " ";}
    if (timeRead == wash2_2) {wash2_2 = " ";}
  }

  if(millis() >= delay_sched + 500){
      delay_sched += 500;

      mySerial.write('o');
  }

  //Serial.println(feed1);

 //Ultrasonic Sensor Water Tank (Slave 1 PIC)

      /*Serial.print("Firebase time: ");
      Serial.print(feed1_0);
      Serial.println(feed1_0.length());*/
/*
  if (timeRead == feed1_0) {
      //Serial.println();
      espclient.publish("cage_1/feed_1/OFF", "ON");
      //espclient.publish("cage_1/feed_1/OFF", "OFF");
      }*/

 if (!espclient.connected()) {
    reconnect();
  }
  if(!espclient.loop())
    espclient.connect("ESP8266Client2");
    
}