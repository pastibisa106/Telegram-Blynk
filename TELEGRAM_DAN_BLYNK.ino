#define BLYNK_PRINT Serial
#include <SPI.h>
#include <SimpleTimer.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
SimpleTimer timer;
char auth[] = "8vo8QtIdluubrdmhbKk8IYiE8VXi7Umr"; //get token from blynk app android
char ssid[] = "HUAWEI"; //nama wifi
char password[] = "pastibisa"; //password wifi
#define BOTtoken "1191159695:AAHyuwPdy3bc0Ok7d_zg3-cp8Tzt_3jZWKU" //token bot telegram
#define idChat1 "326286458" //idbot
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);
unsigned long interval = 40000; // the time we need to wait 30 minute
unsigned long previousMillis = 0; // millis() returns an unsigned long.
void sendSensor () {
    float h = dht.readHumidity();
    float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
    if (isnan(h) || isnan(t)) {
        Serial.println("Failed to read from DHT sensor!");
        return;
    }
    Blynk.virtualWrite(V0, t); // V6 for temperature
    Blynk.virtualWrite(V1, h); // V5 for humidity
}
void setup() {
    Serial.begin(115200);
    Blynk.begin(auth, ssid, password);
    Serial.println("");
    Serial.println("WiFi connected");
    client.setInsecure();
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    dht.begin();
    timer.setInterval(1000L, sendSensor);
    delay(3000);
}
void loop() {
    delay(1000);
    Blynk.run();
    timer.run();
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    Serial.print("Suhu : ");
    Serial.print(t);
    Serial.print(" *C\t");
    Serial.print("RH : ");
    Serial.print(h);
    Serial.println("%");
    if (t > 27.00) {
        unsigned long currentMillis = millis(); // grab current time
        bot.sendChatAction(idChat1, "Sedang mengetik...");
        delay(1000);
        String suhu = "Suhu sekarang : ";
        suhu += float(t);
        suhu += " °C\t";
        suhu += "RH : ";
        suhu += float(h);
        suhu += "%\n";
        suhu += "Suhu Over!\n";
        delay(1000);
        if ((unsigned long)(currentMillis - previousMillis) >= interval) {
            bot.sendMessage(idChat1, suhu, "");
            delay(1000);
            Serial.println("Mengirim data sensor ke telegram");
            // save the "current" time
            previousMillis = millis();
            delay(1000);
        }
    }
}
