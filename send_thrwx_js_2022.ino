#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFi.h>

const char* ssid = "TM5";
const char* password = "MaTNet2020&";

const char* server = "192.168.88.64";
const char* thingName = "123";
const char* serviceName = "got";
const char* appKey = "99338a5e-79c5-4f18-ae4d-2a2cda718583";

int i;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  WiFi.begin("TM2", "MaTNet2020&");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to wifi...");
  }
  Serial.println("Success connect to wifi!");
}

void loop() {
  delay(2000);
  i++;
  // put your main code here, to run repeatedly:
  WiFiClient client;
  HTTPClient http;
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
      Serial.print((String)"&temp=" + i);
      client.print((String)"&temp=" + i);
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
//      int iii = 0;
//      bool currentLineIsBlank = true;
//      bool flagJSON = false;
//      timer_iot_timeout = millis();
//
//      while (client.available() > 0)
//      {
//        char symb = client.read();
//        Serial.print(symb);
//        if (symb == '{')
//        {
//          flagJSON = true;
//        }
//        else if (symb == '}')
//        {
//          flagJSON = false;
//        }
//        if (flagJSON == true)
//        {
//          buff[iii] = symb;
//          iii ++;
//        }
//        timer_iot_timeout = millis();
//      }
//
//      buff[iii] = '}';
//      buff[iii + 1] = '\0';
//      Serial.println(buff);
      // Закрываем соединение
      client.stop();

      // Расшифровываем параметры
//      StaticJsonBuffer<BUFF_LENGTH> jsonBuffer;
//      JsonObject& json_array = jsonBuffer.parseObject(buff);
//      relay_status = json_array["relay_status"];
//      Serial.println("Relay status:  " + String(relay_status));
//      Serial.println();
      //end
      Serial.println("Packet successfully sent!");
      Serial.println();

    }
  }
}
