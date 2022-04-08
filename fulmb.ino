

#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFi.h>

const char* ssid = "TM2";
const char* password = "MaTNet2020&";

const char* server = "192.168.88.64";
const char* thingName = "Test2";
const char* serviceName = "params_get";
const char* appKey = "99338a5e-79c5-4f18-ae4d-2a2cda718583";

#define BUFF_LENGTH 64
#define IOT_TIMEOUT2 100
#define IOT_TIMEOUT1 5000


char buff[BUFF_LENGTH] = "";
int timer_iot_timeout = 0;

int relay_holod_status = 0;
int relay_teplo_status = 0;
int need_temp_status = 20;
int servo_door_status, servo_window_status, auto_manually, svet_outside, svet_inside;

int hum = 0;
int temp = 2;
int smoke = 4;
int svet_datchik, puls, protechka, move_outside, move_inside;

//
//#include <TroykaMQ.h>
//#define mq_pin  34
//MQ2 mq2(mq_pin);
//
//#include <TroykaDHT.h>
//#define temp_pin 4
//DHT dht(temp_pin, DHT11)
//
//#define svet_datchik_pin 33
//
//#define puls_pin 35
//#define protechka_pin 32
//
//#define trig_pin 13
//#define echo_pin 14
//
//int duration, cm;

#include <ESP32_Servo.h>
Servo myservo_window;
Servo myservo_door;
int myservo_window_pin = 25;
int myservo_door_pin = 32;


#define svet_outside_pin 18
#define svet_inside_pin 19
#define relay_holod_pin 25
#define relay_teplo_pin 26
void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to wifi...");
  }
  Serial.println("Success connect to wifi!");

  //  mq2.calibrate();
  //  Serial.print("Ro = ");
  //  Serial.println(mq2.getRo());
  //
  //  dht.begin();
  //
  //  pinMode(trig_pin, OUTPUT);

  myservo_window.attach(myservo_window_pin);
  myservo_door.attach(myservo_door_pin);
}

void loop() {
  delay(2000);

  //  smoke = mq2.readSmoke();
  //  dht.read();
  //  temp = dht.getTemperatureC();
  //  hum = dht.getHumidity();
  //  svet_datchik = analogRead(svet_datchik_pin);
  //  puls = analogRead(puls_pin) / 3.3;
  //  if (analogRead(protechka_pin) > 2000) {
  //    protechka = 1;
  //  }
  //  else {
  //    protechka = 0;
  //  }


  //  digitalWrite(trig_pin, LOW);
  //  delayMicroseconds(5);
  //  digitalWrite(trig_pin, HIGH);
  //  delayMicroseconds(10);
  //  digitalWrite(trig_pin, LOW);
  //  duration = pulseIn(echo_pin, HIGH);
  //  cm = (duration / 2) / 29.1;
  //  if (cm <= 5) {
  //
  //  }
  //  else {
  //
  //  }
  WiFiClient client;
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("error connection to wifi");
    return;
  }
  if (client.connect(server, 80)) {
    if (client.connected()) {
      //send
      Serial.println("Sending data to IoT server...\n");
      Serial.print("POST /Thingworx/Things/");
      client.print("POST /Thingworx/Things/");
      Serial.print(thingName);
      client.print(thingName);
      Serial.print("/Services/");
      client.print("/Services/");
      Serial.print(serviceName);
      client.print(serviceName);
      Serial.print("?appKey=");
      client.print("?appKey=");
      Serial.print(appKey);
      client.print(appKey);
      Serial.print("&method=post&x-thingworx-session=true");
      client.print("&method=post&x-thingworx-session=true");
      Serial.print((String)"&temp=" + temp);
      client.print((String)"&temp=" + temp);
      Serial.print((String)"&hum=" + hum);
      client.print((String)"&hum=" + hum);
      Serial.print((String)"&smoke=" + smoke);
      client.print((String)"&smoke=" + smoke);
      Serial.print((String)"&move_inside=" + move_inside + "&move_outside=" + move_outside + "&protechka=" + protechka + "&puls=" + puls + "&svet_datchik" + svet_datchik);
      client.print((String)"&move_inside=" + move_inside + "&move_outside=" + move_outside + "&protechka=" + protechka + "&puls=" + puls + "&svet_datchik" + svet_datchik);
      Serial.println(" HTTP/1.1");
      client.println(" HTTP/1.1");
      Serial.println("Accept: application/json");
      client.println("Accept: application/json");
      Serial.print("Host: ");
      client.print("Host: ");
      Serial.println(server);
      client.println(server);
      Serial.println("Content-Type: application/json");
      client.println("Content-Type: application/json");
      Serial.println();
      client.println();
      //anwser
      timer_iot_timeout = millis();
      while ((client.available() == 0) && (millis() < timer_iot_timeout + IOT_TIMEOUT1));

      // Выводим ответ о сервера, и, если медленное соединение, ждем выход по таймауту
      int iii = 0;
      bool currentLineIsBlank = true;
      bool flagJSON = false;
      timer_iot_timeout = millis();
      while ((millis() < timer_iot_timeout + IOT_TIMEOUT2) && (client.connected()))
      {
        while (client.available() > 0)
        {
          char symb = client.read();
          Serial.print(symb);
          if (symb == '{')
          {
            flagJSON = true;
          }
          else if (symb == '}')
          {
            flagJSON = false;
          }
          if (flagJSON == true)
          {
            buff[iii] = symb;
            iii ++;
          }
          timer_iot_timeout = millis();
        }
      }
      buff[iii] = '}';
      buff[iii + 1] = '\0';
      Serial.println(buff);
      // Закрываем соединение
      client.stop();

//      // Расшифровываем параметры
      StaticJsonBuffer<BUFF_LENGTH> jsonBuffer;
      JsonObject& json_array = jsonBuffer.parseObject(buff);
      relay_holod_status = json_array["relay_holod_status"];
//      relay_teplo_status = json_array["relay_teplo_status"];
//      need_temp_status = json_array["need_temp_status"];
//      svet_inside = json_array["svet_inside"];
//      svet_outside = json_array["svet_outside"];
//      auto_manually = json_array["auto_manually"];
//      servo_window_status = json_array["servo_window_status"];
//      servo_door_status = json_array["servo_door_status"];
//      Serial.println("holod:   " + String(relay_holod_status));
//      Serial.println("teplo:   " + String(relay_teplo_status));
//      Serial.println("temp:   " + String(need_temp_status));
//      Serial.println("servo_door_status:   " + String(servo_door_status));
//      Serial.println("servo_window_status:   " + String(servo_window_status));
//      Serial.println("auto_manually:   " + String(auto_manually));
//      Serial.println("svet_outside:   " + String(svet_outside));
//      Serial.println("svet_inside:   " + String(svet_inside));
//      Serial.println();
//      Serial.println("Packet successfully sent!");
//      Serial.println();
      control();
    }
  }
}

void control() {
  switch (auto_manually) {
    case 1:
      //you code
      break;
    case 0:
//      digitalWrite(svet_outside_pin, svet_outside);
//      digitalWrite(svet_inside_pin, svet_inside);
//      digitalWrite(relay_holod_pin , relay_holod_status);
//      digitalWrite(relay_teplo_pin , relay_teplo_status);
//      switch (servo_window_status) {
//        case 1:
//          myservo_window.write(90);
//          break;
//        case 0:
//          myservo_window.write(0);
//          break;
//      }
//      switch (servo_door_status) {
//        case 1:
//          myservo_door.write(90);
//          break;
//        case 0:
//          myservo_door.write(0);
//          break;
//      }
      break;
  }
}
