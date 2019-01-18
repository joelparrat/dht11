
/*

    dht11

    data <--> D0

	convertisseur i2c 3.3v 5v

	BVCC <--> +3.3v
	BSCL <--> D2
	BSDA <--> D1
	BGND <--> 0v
	AVCC <--> +5v
	AGND <--> 0v
	ASCL <--> lcd
	ASDA <--> lcd
	AGND <--> lcd

	lcd 2*16 i2c

	gnd <--> agnd
	vcc <--> avcc
	sda <--> asda
	scl <--> ascl
*/


// On charge les librairies
#include <DHTesp.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "ESP8266WiFi.h"
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <ThingSpeak.h>
#include "secret.h"

#define USE_SERIAL Serial

ESP8266WiFiMulti WiFiMulti;

#ifdef ESP32
#pragma message(THIS EXAMPLE IS FOR ESP8266 ONLY!)
#error Select ESP8266 board.
#endif

WiFiClient  client;

// dht11
DHTesp dht; 

//lcd
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

// On définit le PIN qui sera utilisée pour les données
//#define DHT11_PIN D0


void setup()
{

  // On initialise la liaison série
  Serial.begin(115200);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  
  Wire.begin(D1, D2); // (broche SDA, broche SCL);
  dht.setup(D0, DHTesp::DHT11); // Connect DHT sensor to GPIO D0
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Humidite:");
  lcd.setCursor(14,0);
  lcd.print("%");
  lcd.setCursor(0,1);
  lcd.print("Temperat:");
  lcd.setCursor(14,1);
  lcd.print("C");

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

    WiFi.mode(WIFI_STA);
    WiFiMulti.addAP(ssid, psswrd);
    ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop()
{
    int x;
/*
   Serial.print(".");
  if ((WiFiMulti.run() == WL_CONNECTED))
  {
    Serial.println("connected");

    HTTPClient http;
    Serial.print("[HTTP] begin...\n");

    http.begin("http://51.75.126.27/index.html"); //HTTP
    Serial.print("[HTTP] GET...\n");

    int httpCode = http.GET();
    if (httpCode > 0)
    {
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);
 
      // file found at server
      if (httpCode == HTTP_CODE_OK)
      {
        String payload = http.getString();
        Serial.println(payload);
      }
    }
    else
    {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }
*/
  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, psswrd);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }

  //delay(dht.getMinimumSamplingPeriod());
  delay(5000);
  
  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();

  if (strcmp(dht.getStatusString(), "OK"))
  {
    Serial.println("Erreur");
    delay(60000);
    return;
  }

    HTTPClient http; 
    http.begin("http://51.75.126.27/dht11/dht11/php/store_temp.php"); 
    http.addHeader("Content-Type", "application/json");
    String data = "{\"temperature\":\"" + String(temperature) + "\",\"humidite\":\"" + String(humidity) +"\"}";

    int httpCode = http.POST(data);
    http.end();
 
   Serial.print("\t");
    Serial.print(humidity, 1);
    Serial.print("%");
    Serial.print("\t\t");
    Serial.print(temperature, 1);
    Serial.println("C");
    
    lcd.setCursor(10,0);
    lcd.print(humidity, 1);
    lcd.setCursor(10,1);
    lcd.print(temperature, 1);

    // set the fields with the values
    x = ThingSpeak.setField(1, temperature);
    if(x == 200)
    {
        Serial.println("set field Channel update successful.");
    }
    else
    {
        Serial.println("set field Problem updating channel. HTTP error code " + String(x));
        return;
    }

    x = ThingSpeak.setField(2, humidity);
    if(x == 200)
    {
        Serial.println("setfield Channel update successful.");
    }
    else
    {
        Serial.println("setfield Problem updating channel. HTTP error code " + String(x));
        return;
    }

    // write to the ThingSpeak channel
    Serial.println(myChannelNumber);
    Serial.println(myWriteAPIKey);
    x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    if(x == 200)
    {
        Serial.println("write field Channel update successful.");
    }
    else
    {
        Serial.println("write field Problem updating channel. HTTP error code " + String(x));
        return;
    }

    digitalWrite(D7, LOW);
    digitalWrite(D6, LOW);
    digitalWrite(D5, LOW);
    if (temperature < 20.0)
        digitalWrite(D7, HIGH);
    else if (temperature > 24.0)
        digitalWrite(D5, HIGH);
    else
         digitalWrite(D6, HIGH);

  delay(3600000);
}
