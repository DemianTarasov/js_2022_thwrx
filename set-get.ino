#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFi.h>

const char* ssid = "TM2";
const char* password = "MaTNet2020&";

const char* server = "192.168.88.64";
const char* thingName = "Test1";
const char* serviceName = "params_get";
const char* appKey = "99338a5e-79c5-4f18-ae4d-2a2cda718583";

// Размер приемного буффера
#define BUFF_LENGTH 64
#define IOT_TIMEOUT2 100
#define IOT_TIMEOUT1 5000

// Приемный буфер
char buff[BUFF_LENGTH] = "";
int relay_holod_status = 0;
int relay_teplo_status = 0;
int need_temp_status = 20;
int timer_iot_timeout = 0;

int hum = 0;
int temp = 2;
int smoke = 4;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to wifi...");
  }
  Serial.println("Success connect to wifi!");
}

void loop() {
  delay(2000);
  temp++;
  hum++;
smoke++;
  // put your main code here, to run repeatedly:
  WiFiClient client;
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("error connection to wifi");
    return;
  }

  String url = (String) "/Things/" + thingName + "/Services/" + serviceName + "?appKey" + appKey + "&temp=2.0";

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

      // Расшифровываем параметры
      StaticJsonBuffer<BUFF_LENGTH> jsonBuffer;
      JsonObject& json_array = jsonBuffer.parseObject(buff);
      relay_holod_status = json_array["relay_holod_status"];
      relay_teplo_status = json_array["relay_teplo_status"];
      need_temp_status = json_array["need_temp_status"];
      Serial.println("holod:   " + String(relay_holod_status));
      Serial.println("teplo:   " + String(relay_teplo_status));
      Serial.println("temp:   " + String(need_temp_status));
      Serial.println();



      //end
      Serial.println("Packet successfully sent!");
      Serial.println();

    }
  }
}
