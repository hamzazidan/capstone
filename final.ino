// Include the necessary libraries
#include <MQUnifiedsensor.h>
#include <DFRobot_DHT11.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

// Hardware-related macros
#define Board "Arduino UNO"
#define Pin A0  // Analog input pin
#define Type "MQ-9"
#define Voltage_Resolution 5
#define ADC_Bit_Resolution 10  // For Arduino UNO/MEGA/NANO
#define RatioMQ9CleanAir 9.6  // RS/R0 ratio in clean air

// DHT11 pin definition
#define DHT11_PIN 10

// Sensor objects
DFRobot_DHT11 DHT;
MQUnifiedsensor MQ9(Board, Voltage_Resolution, ADC_Bit_Resolution, Pin, Type);
Adafruit_BMP280 bmp;

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Initialize BMP280
  if (!bmp.begin(0x76)) {
    Serial.println("BMP280 initialization failed. Check connections!");
    while (1);
  }

  // Initialize MQ-9 sensor
  MQ9.setRegressionMethod(1);  // Exponential regression (PPM = a * ratio^b)
  MQ9.init();

  Serial.print("Calibrating please wait.");
  float calcR0 = 0;
  for (int i = 1; i <= 10; i++) {
    MQ9.update();  // Update data
    calcR0 += MQ9.calibrate(RatioMQ9CleanAir);
    Serial.print(".");
  }
  MQ9.setR0(calcR0 / 10);
  Serial.println(" done!");

  if (isinf(calcR0)) {
    Serial.println("Error: R0 is infinite. Check wiring!");
    while (1);
  }
  if (calcR0 == 0) {
    Serial.println("Error: R0 is zero. Check wiring!");
    while (1);
  }

  Serial.println("* MQ-9 Sensor Ready *");
}

void loop() {
  // Read temperature and humidity from DHT11
  DHT.read(DHT11_PIN);
//  Serial.print("Temp: ");
  Serial.print(DHT.temperature);
//  Serial.print(" °C, Humidity: ");
  Serial.println(",");
  Serial.print(DHT.humidity);
  Serial.println(",");
//  Serial.println(" %");                                                                                                                                                                                                                                    

  // Read gas concentration
  MQ9.update();
  MQ9.setA(599.65); MQ9.setB(-2.244);  // CO concentration model
  float CO = MQ9.readSensor();
  //Serial.print("CO Concentration: ");
  Serial.print(CO);
  Serial.println(",");

  // Read temperature, pressure, and altitude from BMP280
//  Serial.print("Temperature: ");
//  Serial.print(bmp.readTemperature());
//  Serial.println(" °C");

//  Serial.print("Pressure: ");
  Serial.print(bmp.readPressure());  // Convert to hPa
//  Serial.println(" hPa");

// */ Serial.print("Altitude: ");
//  Serial.print(bmp.readAltitude(1013.25));  // Standard sea level pressure
//  Serial.println(" m");
  Serial.println();
  delay(2000);  // Delay for 2 seconds
}
