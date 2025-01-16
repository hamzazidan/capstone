#include <DHT.h>
#include <Wire.h>
#include <Adafruit_Sensor.h> // This includes the Adafruit_Sensor-master library
#include <Adafruit_BMP280.h>  // BMP280 library for the pressure sensor

// Define pin connections
const int mq2Pin = A0;   // Analog pin for MQ2 sensor (CO detection)
const int mq9Pin = A1;   // Analog pin for MQ9 sensor (CO detection)
const int ledPin1 = 12;  // Digital pin for the first LED

// DHT11 sensor configuration
#define DHTPIN 2          // Pin where the DHT11 data pin is connected
#define DHTTYPE DHT11     // DHT11 sensor type

DHT dht(DHTPIN, DHTTYPE);  // Initialize DHT sensor

// BMP280 sensor configuration
Adafruit_BMP280 bmp;  // Create an instance of the BMP280 sensor

// Define a tolerance level for comparison between MQ2 and MQ9
const int tolerance = 20;  // Adjust tolerance value based on sensor calibration

void setup() {
  // Start serial communication for debugging
  Serial.begin(9600);
  
  // Initialize DHT sensor
  dht.begin();

  // Initialize BMP280 sensor with address 0x76
  if (!bmp.begin(0x76)) {  // Explicitly use I2C address 0x76
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1);  // Stop execution if the sensor is not found
  }

  // Set the LED pin as output
  pinMode(ledPin1, OUTPUT);
}

void loop() {
  // Read values from MQ2 and MQ9 sensors (CO levels)
  int mq2Value = analogRead(mq2Pin);
  int mq9Value = analogRead(mq9Pin);

  // Read temperature and humidity from DHT11 sensor
  float temperature = dht.readTemperature();  // Celsius
  float humidity = dht.readHumidity();        // Humidity in percentage

  // Read pressure from BMP280 sensor
  float pressure = bmp.readPressure() / 100.0F;  // Pressure in hPa (hectopascals)
  float bmpTemperature = bmp.readTemperature();  // BMP280 sensor temperature in Celsius

  // Check if DHT readings are valid
  if (isnan(temperature) || isnan(humidity)) {
    Serial.print("Failed to read from DHT sensor!");
  } else {
    // Print the sensor readings for debugging
//    Serial.print("MQ2 Value (CO): ");
    Serial.print(mq2Value);
    
Serial.print(",");
//    Serial.print("\t MQ9 Value (CO): ");
    Serial.print(mq9Value);

Serial.print(",");


//    Serial.print("Temperature from DHT11: ");
    Serial.print(temperature);

    
Serial.print(",");
//    Serial.print(" °C\t Humidity: ");
    Serial.print(humidity);
//    Serial.println(" %");

Serial.print(",");
    // Print BMP280 sensor data
//    Serial.print("Pressure from BMP280: ");
    Serial.println(pressure);
    
//    Serial.print(",");
//    Serial.print(" hPa\t BMP280 Temperature: ");
//    Serial.print(bmpTemperature);
//    Serial.println(" °C");
  }

  // Check if the MQ2 and MQ9 sensor readings are equal or nearly equal for CO levels
  if (abs(mq2Value - mq9Value) <= tolerance) {
    // If the difference between values is within the tolerance, turn on the LED
    digitalWrite(ledPin1, HIGH);
  } else {
    // Otherwise, turn off the LED
    digitalWrite(ledPin1, LOW);
  }

  // Small delay for stability and sensor reading
  delay(2000);  // Delay for 2 seconds before next loop iteration
}
