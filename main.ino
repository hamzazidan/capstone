

/* 
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-send-messages-whatsapp/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/
#include <ESP32Servo.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <UrlEncode.h>
#include <DHT11.h>
DHT11 dht11(4);

const char* ssid = "Arise!";
const char* password = "Shana1@Zidan2_0607";
// +international_country_code + phone number
// Portugal +351, example: +351912345678
String phoneNumber = "+201097928267";
String apiKey = "2631897";

Servo s1;
int val1 = 0;
#define Motionpin 15
#define Fanpin 18
#define Servopin 19



void sendMessage(String message){

  // Data to send with HTTP POST
  String url = "https://api.callmebot.com/whatsapp.php?phone=" + phoneNumber + "&apikey=" + apiKey + "&text=" + urlEncode(message);    
  HTTPClient http;
  http.begin(url);

  // Specify content-type header
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  
  // Send HTTP POST request
  int httpResponseCode = http.POST(url);
  if (httpResponseCode == 200){
    Serial.println("Message sent successfully");
  }
  else{
    Serial.println("Error sending the message");
    Serial.print("HTTP response code: ");
    Serial.println(httpResponseCode);
  }

  // Free resources
  http.end();
}

void setup() {
  Serial.begin(115200); // baud 
  pinMode(Fanpin,OUTPUT);
  s1.attach(Servopin, 1000, 2000);
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  // Send Message to WhatsAPP
  
}

void loop() {
  int temperature = dht11.readTemperature();
  sendMessage("Here's the temperature inside your building");
  Serial.println(temperature);
  if(digitalRead(Motionpin)==HIGH) {
    Serial.println("Movement detected.");
    s1.write(90);
    sendMessage("Movement detected.");
  } else {
    Serial.println("Did not detect movement.");
    s1.write(0);
  }
  digitalWrite(Fanpin, HIGH);
  delay(2000);
  s1.write(0);
  sendMessage(String(temperature));
}
