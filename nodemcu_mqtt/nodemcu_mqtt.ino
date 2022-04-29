#include <ESP8266WiFi.h>

#include <PubSubClient.h>

#include <SoftwareSerial.h>

#define WIFI_SSID "DE LA VICTORIA NETWORK-2.4GHZ"
#define WIFI_PASSWORD "Isuzutrooper_grv208"

SoftwareSerial mySerial(D6, D5); //RX, TX pins
SoftwareSerial mySerial2(D8, D7); //RX, TX pins
//SoftwareSerial mySerial2 (D4, D3); //RX, TX pins
// Initializes the espClient. You should change the espClient name if you have multiple ESPs running in your home automation system
WiFiClient espClient;
PubSubClient espclient(espClient);

const char * mqtt_server = "test.mosquitto.org";

int period = 500;
unsigned long time_now = 0;
unsigned long time_now2 = 0;
unsigned long delay_sched = 0;
unsigned long delay1 = 0;
unsigned long delay2 = 0;
unsigned long success_delay = 0;
unsigned long failed_delay = 0;

int ultrasonicResult;
String ultrasonicRead;
int ultrasonicInt = 0;

int ultrasonicResultY;
String ultrasonicReadY;
int ultrasonicIntY = 0;

int ultrasonicResultZ;
String ultrasonicReadZ;
int ultrasonicIntZ = 0;

int timeResult;
String timeRead;
int timeInt;

String dateRead;

String feed1_0, feed1_1, feed1_2, feed2_0, feed2_1, feed2_2, feed1;
String wash1_0, wash1_1, wash1_2, wash2_0, wash2_1, wash2_2;

String bath1_0, bath1_1, bath1_2, bath2_0, bath2_1, bath2_2;

String weight_1_msg_info;
String weight_2_msg_info;

bool feed_1_manual_indicator = false;
bool feed_2_manual_indicator = false;

bool feed_1_scheduled_indicator = false;
bool feed_2_scheduled_indicator = false;

bool wash_1_manual_indicator = false;
bool wash_2_manual_indicator = false;

bool wash_1_scheduled_indicator = false;
bool wash_2_scheduled_indicator = false;

bool bath_1_manual_indicator = false;
bool bath_2_manual_indicator = false;

bool bath_1_scheduled_indicator = false;
bool bath_2_scheduled_indicator = false;

bool feed_1_scheduled_warning = false;
bool feed_2_scheduled_warning = false;

bool wash_1_sensor_indicator = false;
bool wash_2_sensor_indicator = false;

String msg1, msg2, msg1_failed, msg2_failed, msg1_success, msg2_success;
String msg1_detected, msg2_detected;
String water_tank_msg;

String feed_1_msg_info;
String feed_2_msg_info;

int prevUltrasonic;

bool x = true;
bool y = true;
bool a = true;
bool b = true;
bool m = true;
bool n = true;

unsigned long thermal_delay;
unsigned long time_delay;
unsigned long water_tank_delay;

// feed 1 voids
void feed_1_on_0p5kg() {
  mySerial.write('a'); // Write integer 111 to PIC
  mySerial.write('o');
  espclient.publish("cage_1/feed_1/switch", "OFF");
}

void feed_1_on_1kg() {
  mySerial.write('b'); // Write integer 112 to PIC
  mySerial.write('o');
  espclient.publish("cage_1/feed_1/switch", "OFF");
}

void feed_1_on_1p5kg() {
  mySerial.write('c'); // Write integer 113 to PIC
  mySerial.write('o');
  espclient.publish("cage_1/feed_1/switch", "OFF");
}

//feed 2 voids

void feed_2_on_0p5kg() {
  mySerial.write('A'); // Write integer 211 to PIC
  mySerial.write('o');
  espclient.publish("cage_2/feed_2/switch", "OFF");
}

void feed_2_on_1kg() {
  mySerial.write('B'); // Write integer 212 to PIC
  mySerial.write('o');
  espclient.publish("cage_2/feed_2/switch", "OFF");
}

void feed_2_on_1p5kg() {
  mySerial.write('C'); // Write integer 213 to PIC
  mySerial.write('o');
  espclient.publish("cage_2/feed_2/switch", "OFF");
}

void wash_1() {
  mySerial.write('w');
  mySerial.write('o');
  espclient.publish("cage_1/wash_1/switch", "OFF");
}

void wash_2() {
  mySerial.write('W');
  mySerial.write('o');
  espclient.publish("cage_2/wash_2/switch", "OFF");
}

void bath_1() {
  mySerial.write('v');
  mySerial.write('o');
  espclient.publish("cage_1/bath_1/switch", "OFF");
}

void bath_2() {
  mySerial.write('V');
  mySerial.write('o');
  espclient.publish("cage_2/bath_2/switch", "OFF");
}

// This functions is executed when some device publishes a message to a topic that your NodeMCU is subscribed to

void callback(String topic, byte * message, unsigned int length) {
  //Serial.print("Message arrived on topic: ");
  //Serial.print(topic);
  //Serial.print(". Message: ");
  String messageInfo;

  for (int i = 0; i < length; i++) {
    //Serial.print((char)message[i]);
    messageInfo += (char) message[i];
  }
  //Serial.println();

  // CAGE 1 refresh
  if (topic == "cage_1/refresh") {
    if (messageInfo == "ON") {
      mySerial.write('r');
      mySerial.write('o');
      espclient.publish("cage_1/refresh/switch", "OFF");
      Serial.print("refresh1");
    }
  }

  if (topic == "cage_2/refresh") {
    if (messageInfo == "ON") {
      mySerial.write('R');
      mySerial.write('o');
      espclient.publish("cage_2/refresh/switch", "OFF");
      Serial.print("refresh2");
    }
  }

  // CAGE 1 Feed
  if (topic == "cage_1/feed_1") {
    if ((messageInfo == "ON 0.5 KG") || (messageInfo == "0.5 KG ON")) {
      feed_1_manual_indicator = true;
      msg1 = "Feed 0.5KG|Manual|" + timeRead + "|" + dateRead;
      msg1_success = msg1 + "|Successful";
      feed_1_on_0p5kg();
      Serial.println("Cage 1 Manual feed 0.5 KG");
    }
    if ((messageInfo == "ON1 0.5 KG") || (messageInfo == "0.5 KG ON1")) {
      feed_1_scheduled_indicator = true;
      msg1 ="Feed 0.5KG|Scheduled|" + timeRead + "|" + dateRead;
      msg1_success = msg1 + "|Successful";
      feed_1_on_0p5kg();
      Serial.println("Cage 1 Scheduled feed 0.5KG");
    }
    if ((messageInfo == "ON 1 KG") || (messageInfo == "1 KG ON")) {
      feed_1_manual_indicator = true;
      msg1 = "Feed 1KG|Manual|" + timeRead + "|" + dateRead;
      msg1_success = msg1 + "|Successful";
      feed_1_on_1kg();
      Serial.println("Cage 1 Manual feed 1KG");
    }
    if ((messageInfo == "ON1 1 KG") || (messageInfo == "1 KG ON1")) {
      feed_1_scheduled_indicator = true;
      msg1 = "Feed 1KG|Scheduled|" + timeRead + "|" + dateRead;
      msg1_success = msg1 + "|Successful";
      feed_1_on_1kg();
      Serial.println("Cage 1 Scheduled feed 1KG");
    }
    if ((messageInfo == "ON 1.5 KG") || (messageInfo == "1.5 KG ON")) {
      feed_1_manual_indicator = true;
      msg1 = "Feed 1.5KG|Manual|" + timeRead + "|" + dateRead;
      msg1_success = msg1 + "|Successful";
      feed_1_on_1p5kg();
      Serial.println("Cage 1 Manual feed 1.5KG");
    }
    if ((messageInfo == "ON1 1.5 KG") || (messageInfo == "1.5 KG ON1")) {
      feed_1_scheduled_indicator = true;
      msg1 = "Feed 1.5KG|Scheduled|" + timeRead + "|" + dateRead;
      msg1_success = msg1 + "|Successful";
      feed_1_on_1p5kg();
      Serial.println("Cage 1 Scheduled feed 1.5KG");
    }
  }

  // CAGE 2 Feed
  if (topic == "cage_2/feed_2") {
    if ((messageInfo == "ON 0.5 KG") || (messageInfo == "0.5 KG ON")) {
      feed_2_manual_indicator = true;
      msg2 = "Feed 0.5KG|Manual|" + timeRead + "|" + dateRead;
      msg2_success = msg2 + "|Successful";
      feed_2_on_0p5kg();
      Serial.println("Cage 2 Manual feed 0.5 KG");
    }
    if ((messageInfo == "ON1 0.5 KG") || (messageInfo == "0.5 KG ON1")) {
      feed_2_scheduled_indicator = true;
      msg2 = "Feed 0.5KG|Scheduled|" + timeRead + "|" + dateRead;
      msg2_success = msg2 + "|Successful";
      feed_2_on_0p5kg();
      Serial.println("Cage 2 Scheduled feed 0.5KG");
    }
    if ((messageInfo == "ON 1 KG") || (messageInfo == "1 KG ON")) {
      feed_2_manual_indicator = true;
      msg2 = "Feed 1KG|Manual|" + timeRead + "|" + dateRead;
      msg2_success = msg2 + "|Successful";
      feed_2_on_1kg();
      Serial.println("Cage 2 Manual feed 1KG");
    }
    if ((messageInfo == "ON1 1 KG") || (messageInfo == "1 KG ON1")) {
      feed_2_scheduled_indicator = true;
      msg2 = "Feed 1KG|Scheduled|" + timeRead + "|" + dateRead;
      msg2_success = msg2 + "|Successful";
      feed_2_on_1kg();
      Serial.println("Cage 2 Scheduled feed 1KG");
    }
    if ((messageInfo == "ON 1.5 KG") || (messageInfo == "1.5 KG ON")) {
      feed_2_manual_indicator = true;
      msg2 = "Feed 1.5KG|Manual|" + timeRead + "|" + dateRead;
      msg2_success = msg2 + "|Successful";
      feed_2_on_1p5kg();
      Serial.println("Cage 2 Manual feed 1.5KG");
    }
    if ((messageInfo == "ON1 1.5 KG") || (messageInfo == "1.5 KG ON1")) {
      feed_2_scheduled_indicator = true;
      msg2 = "Feed 1.5KG|Scheduled|" + timeRead + "|" + dateRead;
      msg2_success = msg2 + "|Successful";
      feed_2_on_1p5kg();
      Serial.println("Cage 2 Scheduled feed 1.5KG");
    }
  }

  // CAGE 1 Wash

  if (topic == "cage_1/wash_1") {
    if (messageInfo == "ON") {
      wash_1_manual_indicator = true;
      msg1 = "Wash|Manual|" + timeRead + "|" + dateRead;
      msg1_success = msg1 + "|Successful";
      wash_1();
      Serial.println("Wash 1");
    }

    if (messageInfo == "ON1") {
      wash_1_scheduled_indicator = true;
      msg1 = "Wash|Scheduled|" + timeRead + "|" + dateRead;
      msg1_success = msg1 + "|Successful";
      wash_1();
      Serial.println("Wash 1");
    }

    if (messageInfo == "ON2") {
      wash_1_sensor_indicator = true;
      msg1 = "Wash|Detected|" + timeRead + "|" + dateRead;
      msg1_success = msg1 + "|Successful";
      wash_1();
      Serial.println("Wash 1");
    }
  }

  if (topic == "cage_2/wash_2") {
    if (messageInfo == "ON") {
      wash_2_manual_indicator = true;
      msg2 = "Wash|Manual|" + timeRead + "|" + dateRead;
      msg2_success = msg2 + "|Successful";
      wash_2();
      Serial.println("Wash 2");
    }

    if (messageInfo == "ON1") {
      wash_2_scheduled_indicator = true;
      msg2 = "Wash|Scheduled|" + timeRead + "|" + dateRead;
      msg2_success = msg2 + "|Successful";
      wash_2();
      Serial.println("Wash 2");
    }

    if (messageInfo == "ON2") {
      wash_2_sensor_indicator = true;
      msg2 = "Wash|Detected|" + timeRead + "|" + dateRead;
      msg2_success = msg2 + "|Successful";
      wash_2();
      Serial.println("Wash 2");
    }
  }

  // CAGE 1 Bath

  if (topic == "cage_1/bath_1") {
    if (messageInfo == "ON") {
      bath_1_manual_indicator = true;
      msg1 = "Bath|Manual|" + timeRead + "|" + dateRead;
      msg1_success = msg1 + "|Successful";
      bath_1();
      Serial.println("Bath 1");
    }

    if (messageInfo == "ON1") {
      bath_1_scheduled_indicator = true;
      msg1 = "Bath|Scheduled|" + timeRead + "|" + dateRead;
      msg1_success = msg1 + "|Successful";
      bath_1();
      Serial.println("Bath 1");
    }
  }

  if (topic == "cage_2/bath_2") {
    if (messageInfo == "ON") {
      bath_2_manual_indicator = true;
      msg2 = "Bath|Manual|" + timeRead + "|" + dateRead;
      msg2_success = msg2 + "|Successful";
      bath_2();
      Serial.println("Bath 2");
    }

    if (messageInfo == "ON1") {
      bath_2_scheduled_indicator = true;
      msg2 = "Bath|Scheduled|" + timeRead + "|" + dateRead;
      msg2_success = msg2 + "|Successful";
      bath_2();
      Serial.println("Bath 2");
    }
  }

  //CAGE 1 sched

  if (topic == "cage1_feed_sched/0") {
    feed1_0 = messageInfo;
    Serial.println(messageInfo);
  }

  if (topic == "cage1_feed_sched/1") {
    feed1_1 = messageInfo;
    Serial.println(messageInfo);
  }

  if (topic == "cage1_feed_sched/2") {
    feed1_2 = messageInfo;
    Serial.println(messageInfo);
  }

  //CAGE 2 sched

  if (topic == "cage2_feed_sched/0") {
    feed2_0 = messageInfo;
    Serial.println(messageInfo);
  }

  if (topic == "cage2_feed_sched/1") {
    feed2_1 = messageInfo;
    Serial.println(messageInfo);
  }

  if (topic == "cage2_feed_sched/2") {
    feed2_2 = messageInfo;
    Serial.println(messageInfo);
  }

  //cage 1 wash sched
  if (topic == "cage1_wash_sched/0") {
    wash1_0 = messageInfo;
    Serial.println(messageInfo);
  }

  if (topic == "cage1_wash_sched/1") {
    wash1_1 = messageInfo;
    Serial.println(messageInfo);
  }

  if (topic == "cage1_wash_sched/2") {
    wash1_2 = messageInfo;
    Serial.println(messageInfo);
  }

  // Cage 2 wash sched
  if (topic == "cage2_wash_sched/0") {
    wash2_0 = messageInfo;
    Serial.println(messageInfo);
  }

  if (topic == "cage2_wash_sched/1") {
    wash2_1 = messageInfo;
    Serial.println(messageInfo);
  }

  if (topic == "cage2_wash_sched/2") {
    wash2_2 = messageInfo;
    Serial.println(messageInfo);
  }

   //cage 1 bath sched
  if (topic == "cage1_bath_sched/0") {
    bath1_0 = messageInfo;
    Serial.println(messageInfo);
  }

  if (topic == "cage1_bath_sched/1") {
    bath1_1 = messageInfo;
    Serial.println(messageInfo);
  }

  if (topic == "cage1_bath_sched/2") {
    bath1_2 = messageInfo;
    Serial.println(messageInfo);
  }

  // Cage 2 bath sched
  if (topic == "cage2_bath_sched/0") {
    bath2_0 = messageInfo;
    Serial.println(messageInfo);
  }

  if (topic == "cage2_bath_sched/1") {
    bath2_1 = messageInfo;
    Serial.println(messageInfo);
  }

  if (topic == "cage2_bath_sched/2") {
    bath2_2 = messageInfo;
    Serial.println(messageInfo);
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
      espclient.subscribe("cage_1/wash_1/switch");

      espclient.subscribe("cage_2/feed_2");
      espclient.subscribe("cage_2/feed_2/switch");
      espclient.subscribe("cage_2/weight_2");
      espclient.subscribe("cage_2/logs");
      espclient.subscribe("cage_2/wash_2/switch");

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

      espclient.subscribe("cage1_wash_sched/0");
      espclient.subscribe("cage1_wash_sched/1");
      espclient.subscribe("cage1_wash_sched/2");

      espclient.subscribe("cage2_wash_sched/0");
      espclient.subscribe("cage2_wash_sched/1");
      espclient.subscribe("cage2_wash_sched/2");

      espclient.subscribe("cage_1/bath_1");
      espclient.subscribe("cage_2/bath_2");

      espclient.subscribe("cage_1/bath_1/switch");
      espclient.subscribe("cage_2/bath_2/switch");

      espclient.subscribe("cage1_bath_sched/0");
      espclient.subscribe("cage1_bath_sched/1");
      espclient.subscribe("cage1_bath_sched/2");

      espclient.subscribe("cage2_bath_sched/0");
      espclient.subscribe("cage2_bath_sched/1");
      espclient.subscribe("cage2_bath_sched/2");

      espclient.subscribe("cage_1/refresh");
      espclient.subscribe("cage_2/refresh");
      espclient.subscribe("cage_1/refresh/switch");
      espclient.subscribe("cage_2/refresh/switch");

      espclient.subscribe("cage_1/feed_1_manual_indicator");
      espclient.subscribe("cage_2/feed_2_manual_indicator");

      espclient.subscribe("cage_1/feed_1_scheduled_indicator");
      espclient.subscribe("cage_2/feed_2_scheduled_indicator");

      espclient.subscribe("cage_1/wash_1_manual_indicator");
      espclient.subscribe("cage_2/wash_2_manual_indicator");

      espclient.subscribe("cage_1/wash_1_scheduled_indicator");
      espclient.subscribe("cage_2/wash_2_scheduled_indicator");

      espclient.subscribe("cage_1/bath_1_manual_indicator");
      espclient.subscribe("cage_2/bath_2_manual_indicator");

      espclient.subscribe("cage_1/bath_1_scheduled_indicator");
      espclient.subscribe("cage_2/bath_2_scheduled_indicator");

      espclient.subscribe("tanks/logs");

      espclient.subscribe("cage_1/wash_1_sensor_indicator");
      espclient.subscribe("cage_2/wash_2_sensor_indicator");

    } else {
      Serial.print("failed, rc=");
      Serial.print(espclient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  mySerial2.begin(9600);

  delay(1000);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);

  pinMode(LED_BUILTIN, OUTPUT);
  espclient.setServer(mqtt_server, 1883);
  espclient.setCallback(callback);

  ultrasonicIntY = 100;
  ultrasonicIntZ = 100;

}

void loop() {

  //mySerial_rd = mySerial.read();
  //mySerial2_rd = mySerial2.read(); 
  //Serial_rd = Serial.read();

  while (m == true){
  delay(50);
  if (mySerial2.read() == 'x') {
    m = false;
    if (m == false){
    ultrasonicRead = mySerial2.readStringUntil('\r');
    ultrasonicInt = ultrasonicRead.toInt();
    Serial.println();
    Serial.print("Water Tank : ");
    Serial.print(ultrasonicInt);
    espclient.publish("water_tank", String(ultrasonicInt).c_str());
    n = true;
    }
  }
  }

  while (n == true){
  delay(50);
  if (mySerial2.read() == 't' && time_delay < millis()) {
    time_delay = millis() + 200;
    n = false;
    if (n == false) {
    timeRead = mySerial2.readStringUntil('\n');
    Serial.println();
    Serial.print("time: ");
    Serial.print(timeRead);
    m = true;
    }
  }
  }

  if (mySerial2.read() == 'd') {
    dateRead = mySerial2.readStringUntil('\n');
    Serial.println();
    Serial.print("date: ");
    Serial.print(dateRead);
  }

  if ((ultrasonicInt <=50) && (x == true)) {
    x = false;
    if (x == false) {
    water_tank_msg = "Below 50%|Refilling|" + timeRead + "|" + dateRead;  
    mySerial.write('t');
    Serial.println("Refilling");
    espclient.publish("tanks/logs", water_tank_msg.c_str());
    y = true;  
    }
  }

  if ((ultrasonicInt >= 80) && (y == true)){
    y = false;
    if (y == false) {
    water_tank_msg = "Above 80%|Refilled|" + timeRead + "|" + dateRead;  
    mySerial.write('T');
    Serial.println("Refilled!");
    espclient.publish("tanks/logs", water_tank_msg.c_str());
    x = true;
    }
  }


  if (Serial.read() == 'z') {
    ultrasonicReadZ = Serial.readStringUntil('\r');
    ultrasonicIntZ = ultrasonicReadZ.toInt();
    Serial.println();
    Serial.print("Feed Tank 2: ");
    Serial.print(ultrasonicIntZ);
    espclient.publish("feed_tank_2", String(ultrasonicIntZ).c_str());
    if (ultrasonicIntZ <= 15) {
      mySerial.write('z');
      mySerial.write("ooooooooo");
    }
  }

  if (mySerial.read() == 'y'){
    ultrasonicReadY = mySerial.readStringUntil('\r');
    ultrasonicIntY = ultrasonicReadY.toInt();
    Serial.println();
    Serial.print("Feed Tank 1: ");
    Serial.print(ultrasonicIntY);
    espclient.publish("feed_tank_1", String(ultrasonicIntY).c_str());
    if (ultrasonicIntY <= 15) {
      mySerial.write('y');
      mySerial.write("ooooooooo");
    }
  }

  if ((mySerial.read() == 'k') && (thermal_delay < millis())) {
    thermal_delay = millis() + 2000;
    Serial.println("Cage 1 thermal detected");
    espclient.publish("cage_1/wash_1_sensor_indicator", "success");
    delay(1000);
    espclient.publish("cage_1/wash_1_sensor_indicator", " ");
    msg1_detected = "Wash|Detected|" + timeRead + "|" + dateRead + "|Successful";
    espclient.publish("cage_1/logs", msg1_detected.c_str());
  }

  if (Serial.read() == 'K') {
    Serial.println("Cage 2 thermal detected");
    espclient.publish("cage_2/wash_2_sensor_indicator", "success");
    delay(1000);
    espclient.publish("cage_2/wash_2_sensor_indicator", " ");
    msg2_detected = "Wash|Detected|" + timeRead + "|" + dateRead + "|Successful";
    espclient.publish("cage_2/logs", msg2_detected.c_str());
  }



while (feed_1_manual_indicator == true) {

    delay(200);
    if (mySerial.read() == 's') {
      Serial.println("success");
        espclient.publish("cage_1/feed_1_manual_indicator", "success");
        delay(1000);
        espclient.publish("cage_1/feed_1_manual_indicator", " ");
        espclient.publish("cage_1/logs", msg1_success.c_str());

        feed_1_manual_indicator = false;
    }


    else {
      if (millis() >= failed_delay + 3000) {
  failed_delay += 3000;
      while (feed_1_manual_indicator == true) {
      msg1_failed = msg1 + " Failed";
      Serial.println("failed");
      espclient.publish("cage_1/feed_1_manual_indicator", "failed");
      espclient.publish("cage_1/logs", msg1_failed.c_str());
      delay(1000);
      espclient.publish("cage_1/feed_1_manual_indicator", " ");
      feed_1_manual_indicator = false;
      }
      }
    }
}

while (feed_1_scheduled_indicator == true) {

    delay(500);
    if (mySerial.read() == 's') {
      Serial.println("success");
        espclient.publish("cage_1/feed_1_scheduled_indicator", "success");
        delay(1000);
        espclient.publish("cage_1/feed_1_scheduled_indicator", " ");
        espclient.publish("cage_1/logs", msg1_success.c_str());
        feed_1_scheduled_indicator = false;
    }


    else {
      if (millis() >= failed_delay + 3000) {
  failed_delay += 3000;
      while (feed_1_scheduled_indicator == true) {
      msg1_failed = msg1 + " Failed";
      Serial.println("failed");
      espclient.publish("cage_1/feed_1_scheduled_indicator", "failed");
      espclient.publish("cage_1/logs", msg1_failed.c_str());
      delay(1000);
      espclient.publish("cage_1/feed_1_scheduled_indicator", " ");
      feed_1_scheduled_indicator = false;
      }
      }
    }
}

while (feed_2_manual_indicator == true) {

    delay(200);
    if (Serial.read() == 'S') {
      Serial.println("success");
        espclient.publish("cage_2/feed_2_manual_indicator", "success");
        delay(1000);
        espclient.publish("cage_2/feed_2_manual_indicator", " ");
        espclient.publish("cage_2/logs", msg2_success.c_str());
        feed_2_manual_indicator = false;
    }


    else {
      if (millis() >= failed_delay + 3000) {
  failed_delay += 3000;
      while (feed_2_manual_indicator == true) {
      msg2_failed = msg2 + " Failed";
      Serial.println("failed");
      espclient.publish("cage_2/feed_2_manual_indicator", "failed");
      espclient.publish("cage_2/logs", msg2_failed.c_str());
      delay(1000);
      espclient.publish("cage_2/feed_2_manual_indicator", " ");
      feed_2_manual_indicator = false;
      }
      }
    }
}

while (feed_2_scheduled_indicator == true) {

    delay(500);
    if (Serial.read() == 'S') {
      Serial.println("success");
        espclient.publish("cage_2/feed_2_scheduled_indicator", "success");
        delay(1000);
        espclient.publish("cage_2/feed_2_scheduled_indicator", " ");
        espclient.publish("cage_2/logs", msg2_success.c_str());
        feed_2_scheduled_indicator = false;
    }


    else {
      if (millis() >= failed_delay + 3000) {
  failed_delay += 3000;
      while (feed_2_scheduled_indicator == true) {
      msg2_failed = msg2 + " Failed";
      Serial.println("failed");
      espclient.publish("cage_2/feed_2_scheduled_indicator", "failed");
      espclient.publish("cage_2/logs", msg2_failed.c_str());
      delay(1000);
      espclient.publish("cage_2/feed_2_scheduled_indicator", " ");
      feed_2_scheduled_indicator = false;
      }
      }
    }
}

while (wash_1_manual_indicator == true) {

    delay(200);
    if (mySerial.read() == 's') {
      Serial.println("success");
        espclient.publish("cage_1/wash_1_manual_indicator", "success");
        delay(1000);
        espclient.publish("cage_1/wash_1_manual_indicator", " ");
        espclient.publish("cage_1/logs", msg1_success.c_str());
        wash_1_manual_indicator = false;
    }


    else {
      if (millis() >= failed_delay + 3000) {
  failed_delay += 3000;
      while (wash_1_manual_indicator == true) {
      msg1_failed = msg1 + " Failed";
      Serial.println("failed");
      espclient.publish("cage_1/wash_1_manual_indicator", "failed");
      espclient.publish("cage_1/logs", msg1_failed.c_str());
      delay(1000);
      espclient.publish("cage_1/wash_1_manual_indicator", " ");
      wash_1_manual_indicator = false;
      }
      }
    }
}

while (wash_1_scheduled_indicator == true) {

    delay(500);
    if (mySerial.read() == 's') {
      Serial.println("success");
        espclient.publish("cage_1/wash_1_scheduled_indicator", "success");
        delay(1000);
        espclient.publish("cage_1/wash_1_scheduled_indicator", " ");
        espclient.publish("cage_1/logs", msg1_success.c_str());
        wash_1_scheduled_indicator = false;
    }


    else {
      if (millis() >= failed_delay + 3000) {
  failed_delay += 3000;
      while (wash_1_scheduled_indicator == true) {
      msg1_failed = msg1 + " Failed";
      Serial.println("failed");
      espclient.publish("cage_1/wash_1_scheduled_indicator", "failed");
      espclient.publish("cage_1/logs", msg1_failed.c_str());
      delay(1000);
      espclient.publish("cage_1/wash_1_scheduled_indicator", " ");
      wash_1_scheduled_indicator = false;
      }
      }
    }
}


while (wash_1_sensor_indicator == true) {

    delay(200);
    if (mySerial.read() == 's') {
      Serial.println("success");
        espclient.publish("cage_1/wash_1_sensor_indicator", "success");
        delay(1000);
        espclient.publish("cage_1/wash_1_sensor_indicator", " ");
        espclient.publish("cage_1/logs", msg1_success.c_str());
        wash_1_sensor_indicator = false;
    }


    else {
      if (millis() >= failed_delay + 3000) {
  failed_delay += 3000;
      while (wash_1_sensor_indicator == true) {
      msg1_failed = msg1 + " Failed";
      Serial.println("failed");
      espclient.publish("cage_1/wash_1_sensor_indicator", "failed");
      espclient.publish("cage_1/logs", msg1_failed.c_str());
      delay(1000);
      espclient.publish("cage_1/wash_1_sensor_indicator", " ");
      wash_1_sensor_indicator = false;
      }
      }
    }
}

while (wash_2_manual_indicator == true) {

    delay(200);
    if (Serial.read() == 'S') {
      Serial.println("success");
        espclient.publish("cage_2/wash_2_manual_indicator", "success");
        delay(1000);
        espclient.publish("cage_2/wash_2_manual_indicator", " ");
        espclient.publish("cage_2/logs", msg2_success.c_str());
        wash_2_manual_indicator = false;
    }


    else {
      if (millis() >= failed_delay + 3000) {
  failed_delay += 3000;
      while (wash_2_manual_indicator == true) {
      msg2_failed = msg2 + " Failed";
      Serial.println("failed");
      espclient.publish("cage_2/wash_2_manual_indicator", "failed");
      espclient.publish("cage_2/logs", msg2_failed.c_str());
      delay(1000);
      espclient.publish("cage_2/wash_2_manual_indicator", " ");
      wash_2_manual_indicator = false;
      }
      }
    }
}

while (wash_2_scheduled_indicator == true) {

    delay(500);
    if (Serial.read() == 'S') {
      Serial.println("success");
        espclient.publish("cage_2/wash_2_scheduled_indicator", "success");
        delay(1000);
        espclient.publish("cage_2/wash_2_scheduled_indicator", " ");
        espclient.publish("cage_2/logs", msg2_success.c_str());

        wash_2_scheduled_indicator = false;
    }


    else {
      if (millis() >= failed_delay + 3000) {
  failed_delay += 3000;
      while (wash_2_scheduled_indicator == true) {
      msg2_failed = msg2 + " Failed";
      Serial.println("failed");
      espclient.publish("cage_2/wash_2_scheduled_indicator", "failed");
      espclient.publish("cage_2/logs", msg2_failed.c_str());
      delay(1000);
      espclient.publish("cage_2/wash_2_scheduled_indicator", " ");
      wash_2_scheduled_indicator = false;
      }
      }
    }
}

while (wash_2_sensor_indicator == true) {

    delay(200);
    if (Serial.read() == 'S') {
      Serial.println("success");
        espclient.publish("cage_2/wash_2_sensor_indicator", "success");
        delay(1000);
        espclient.publish("cage_2/wash_2_sensor_indicator", " ");
        espclient.publish("cage_2/logs", msg2_success.c_str());

        wash_2_sensor_indicator = false;
    }


    else {
      if (millis() >= failed_delay + 3000) {
  failed_delay += 3000;
      while (wash_2_sensor_indicator == true) {
      msg2_failed = msg2 + " Failed";
      Serial.println("failed");
      espclient.publish("cage_2/wash_2_sensor_indicator", "failed");
      espclient.publish("cage_2/logs", msg2_failed.c_str());
      delay(1000);
      espclient.publish("cage_2/wash_2_sensor_indicator", " ");
      wash_2_sensor_indicator = false;
      }
      }
    }
}

while (bath_1_manual_indicator == true) {

    delay(200);
    if (mySerial.read() == 's') {
      Serial.println("success");
        espclient.publish("cage_1/bath_1_manual_indicator", "success");
        delay(1000);
        espclient.publish("cage_1/bath_1_manual_indicator", " ");
        espclient.publish("cage_1/logs", msg1_success.c_str());

        bath_1_manual_indicator = false;
    }


    else {
      if (millis() >= failed_delay + 3000) {
  failed_delay += 3000;
      while (bath_1_manual_indicator == true) {
      msg1_failed = msg1 + " Failed";
      Serial.println("failed");
      espclient.publish("cage_1/bath_1_manual_indicator", "failed");
      espclient.publish("cage_1/logs", msg1_failed.c_str());
      delay(1000);
      espclient.publish("cage_1/bath_1_manual_indicator", " ");
      bath_1_manual_indicator = false;
      }
      }
    }
}

while (bath_1_scheduled_indicator == true) {

    delay(500);
    if (mySerial.read() == 's') {
      Serial.println("success");
        espclient.publish("cage_1/bath_1_scheduled_indicator", "success");
        delay(1000);
        espclient.publish("cage_1/bath_1_scheduled_indicator", " ");
        espclient.publish("cage_1/logs", msg1_success.c_str());

        bath_1_scheduled_indicator = false;
    }


    else {
      if (millis() >= failed_delay + 3000) {
  failed_delay += 3000;
      while (bath_1_scheduled_indicator == true) {
      msg1_failed = msg1 + " Failed";
      Serial.println("failed");
      espclient.publish("cage_1/bath_1_scheduled_indicator", "failed");
      espclient.publish("cage_1/logs", msg1_failed.c_str());
      delay(1000);
      espclient.publish("cage_1/bath_1_scheduled_indicator", " ");
      bath_1_scheduled_indicator = false;
      }
      }
    }
}

while (bath_2_manual_indicator == true) {

    delay(200);
    if (Serial.read() == 'S') {
      Serial.println("success");
        espclient.publish("cage_2/bath_2_manual_indicator", "success");
        delay(1000);
        espclient.publish("cage_2/bath_2_manual_indicator", " ");
        espclient.publish("cage_2/logs", msg2_success.c_str());

        bath_2_manual_indicator = false;
    }


    else {
      if (millis() >= failed_delay + 3000) {
  failed_delay += 3000;
      while (bath_2_manual_indicator == true) {
      msg2_failed = msg2 + " Failed";
      Serial.println("failed");
      espclient.publish("cage_2/bath_2_manual_indicator", "failed");
      espclient.publish("cage_2/logs", msg2_failed.c_str());
      delay(1000);
      espclient.publish("cage_2/bath_2_manual_indicator", " ");
      bath_2_manual_indicator = false;
      }
      }
    }
}

while (bath_2_scheduled_indicator == true) {

    delay(500);
    if (Serial.read() == 'S') {
      Serial.println("success");
        espclient.publish("cage_2/bath_2_scheduled_indicator", "success");
        delay(1000);
        espclient.publish("cage_2/bath_2_scheduled_indicator", " ");
        espclient.publish("cage_2/logs", msg2_success.c_str());

        bath_2_scheduled_indicator = false;
    }


    else {
      if (millis() >= failed_delay + 3000) {
  failed_delay += 3000;
      while (bath_2_scheduled_indicator == true) {
      msg2_failed = msg2 + " Failed";
      Serial.println("failed");
      espclient.publish("cage_2/bath_2_scheduled_indicator", "failed");
      espclient.publish("cage_2/logs", msg2_failed.c_str());
      delay(1000);
      espclient.publish("cage_2/bath_2_scheduled_indicator", " ");
      bath_2_scheduled_indicator = false;
      }
      }
    }
}


if (ultrasonicIntY > 15) {
  if ((timeRead == feed1_0) || (timeRead == feed1_1) || (timeRead == feed1_2)) {
    Serial.println("feed 1 scheduled successful!");
    espclient.publish("cage_1/feed_1/switch", "ON1");
    if (timeRead == feed1_0) {
      feed1_0 = " ";
    }
    if (timeRead == feed1_1) {
      feed1_1 = " ";
    }
    if (timeRead == feed1_2) {
      feed1_2 = " ";
    }
  }
}

if (ultrasonicIntY <= 15) {
  if ((timeRead == feed1_0) || (timeRead == feed1_1) || (timeRead == feed1_2)) {
     //msg1 = "Feed " + weight_1_msg_info + " Scheduled" + dateRead + " " + timeRead;
     feed_1_scheduled_indicator = true;
    Serial.print("Cannot feed, please refill feed tank 1");
    if (timeRead == feed1_0) {
      feed1_0 = " ";
    }
    if (timeRead == feed1_1) {
      feed1_1 = " ";
    }
    if (timeRead == feed1_2) {
      feed1_2 = " ";
    }
  }
}

if (ultrasonicIntZ > 15) {
  if ((timeRead == feed2_0) || (timeRead == feed2_1) || (timeRead == feed2_2)) {
    Serial.println("feed 2 scheduled successful!");
    espclient.publish("cage_2/feed_2/switch", "ON1");
    if (timeRead == feed2_0) {
      feed2_0 = " ";
    }
    if (timeRead == feed2_1) {
      feed2_1 = " ";
    }
    if (timeRead == feed2_2) {
      feed2_2 = " ";
    }
  }
}

if (ultrasonicIntZ <= 15) {
  if ((timeRead == feed2_0) || (timeRead == feed2_1) || (timeRead == feed2_2)) {
     //msg2 = "Feed " + weight_2_msg_info + " Scheduled" + dateRead + " " + timeRead;
     feed_2_scheduled_indicator = true;
    Serial.print("Cannot feed, please refill feed tank 2");
    if (timeRead == feed2_0) {
      feed2_0 = " ";
    }
    if (timeRead == feed2_1) {
      feed2_1 = " ";
    }
    if (timeRead == feed2_2) {
      feed2_2 = " ";
    }
  }
}

  if ((timeRead == wash1_0) || (timeRead == wash1_1) || (timeRead == wash1_2)) {
    Serial.println("wash 1 scheduled successful!");
    espclient.publish("cage_1/wash_1/switch", "ON1");
    if (timeRead == wash1_0) {
      wash1_0 = " ";
    }
    if (timeRead == wash1_1) {
      wash1_1 = " ";
    }
    if (timeRead == wash1_2) {
      wash1_2 = " ";
    }
  }

  if ((timeRead == wash2_0) || (timeRead == wash2_1) || (timeRead == wash2_2)) {
    Serial.println("wash 2 scheduled successful!");
    espclient.publish("cage_2/wash_2/switch", "ON1");
    if (timeRead == wash2_0) {
      wash2_0 = " ";
    }
    if (timeRead == wash2_1) {
      wash2_1 = " ";
    }
    if (timeRead == wash2_2) {
      wash2_2 = " ";
    }
  }

  if ((timeRead == bath1_0) || (timeRead == bath1_1) || (timeRead == bath1_2)) {
    Serial.println("bath 1 scheduled successful!");
    espclient.publish("cage_1/bath_1/switch", "ON1");
    if (timeRead == bath1_0) {
      bath1_0 = " ";
    }
    if (timeRead == bath1_1) {
      bath1_1 = " ";
    }
    if (timeRead == bath1_2) {
      bath1_2 = " ";
    }
  }

  if ((timeRead == bath2_0) || (timeRead == bath2_1) || (timeRead == bath2_2)) {
    Serial.println("bath 2 scheduled successful!");
    espclient.publish("cage_2/bath_2/switch", "ON1");
    if (timeRead == bath2_0) {
      bath2_0 = " ";
    }
    if (timeRead == bath2_1) {
      bath2_1 = " ";
    }
    if (timeRead == bath2_2) {
      bath2_2 = " ";
    }
  }

  /*if (millis() >= delay_sched + 500) {
    delay_sched += 500;

    mySerial.write('o');
  }*/

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
  if (!espclient.loop())
    espclient.connect("ESP8266Client2");

}