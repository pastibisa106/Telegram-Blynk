//library ESP8266
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

//library telegram
#include <UniversalTelegramBot.h>
#define BOTtoken "1422959392:AAFA2xAGyR1eM1tvq3hzX5zazeWMkJGXgAc" //token bot telegram
#define idChat "326286458" //idbot

//library dht
#include "DHT.h"
#define DHTPIN 2 //GPIO D4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

char ssid[] = "HUAWEI"; //nama wifi
char password[] = "pastibisa"; //password wifi

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

unsigned long interval = 30000; // time to wait 1 minute
unsigned long previousMillis = 0; // millis() returns an unsigned long.

void setup() {
    Serial.begin(9600);
    Serial.print("Connecting WiFi: ");
    Serial.println(ssid);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    Serial.println("");
    Serial.println("WiFi connected");
    client.setInsecure();
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    dht.begin();
    delay(2000);
}

void loop() {
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    Serial.print("Suhu : ");
    Serial.print(t);
    Serial.print(" °C\t");
    Serial.print("RH : ");
    Serial.print(h);
    Serial.println("%");
    
    if (t >= 27.00) {
        unsigned long currentMillis = millis(); // grab current time
        bot.sendChatAction(idChat, "Sedang mengetik...");
        delay(2000);
        String suhu = "Suhu sekarang : ";
        suhu += float(t);
        suhu += " °C\t";
        suhu += "RH : ";
        suhu += float(h);
        suhu += "%\n";
        suhu += "Suhu Over!\n";
        delay(1000);
        if ((unsigned long)(currentMillis - previousMillis) >= interval) {
            bot.sendMessage(idChat, suhu, "");
            delay(500);
            Serial.println("Mengirim data sensor ke telegram");
            // save the "current" time
            previousMillis = millis();
            delay(500);
        }
    }
}
