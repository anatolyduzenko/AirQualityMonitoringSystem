/*
ST7789 240x320 2.0" IPS - only 4+2 wires required:
 #01 GND -> GND
 #02 VCC -> VCC (3.3V only!)
 #03 SCL -> D13/SCK
 #04 SDA -> D11/MOSI
 #05 RES -> optional
 #06 DC  -> D10 or any digital
 #07 CS  -> D9 or any digital
*/

#include <MQ135.h>
#include <DHT11.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include "ST7789_AVR.h"
#include <MQUnifiedsensor.h>

#define PIN_MQ135 A0 // MQ135 Analog Input Pin
#define DHTPIN 7 // DHT Digital Input Pin
#define MQ_PIN A1

MQ135 mq135_sensor(PIN_MQ135);
DHT11 dht(DHTPIN);
float temperature, humidity;
float rzero, correctedRZero, resistance, ppm, correctedPPM;

// Define LCD pins
#define TFT_DC   9
#define TFT_CS  8 // without CS
#define TFT_RST  10 // without CS
// Deifne screen size
#define SCR_WD 240
#define SCR_HT 320
ST7789_AVR lcd = ST7789_AVR(TFT_DC, TFT_RST, TFT_CS);

/************************Hardware Related Macros************************************/
#define         Board                   ("Arduino UNO")
#define         Pin                     (A1)  //Analog input 3 of your arduino
/***********************Software Related Macros************************************/
#define         Type                    ("MQ-2") //MQ2
#define         Voltage_Resolution      (5)
#define         ADC_Bit_Resolution      (10) // For arduino UNO/MEGA/NANO
#define         RatioMQ2CleanAir        (9.83) //RS / R0 = 9.83 ppm 

MQUnifiedsensor MQ2(Board, Voltage_Resolution, ADC_Bit_Resolution, Pin, Type);

// Prepare sensors, lcd
void setup() 
{
    Serial.begin(115200);
    lcd.init(SCR_WD,SCR_HT);
    lcd.setRotation(3);
    lcd.fillScreen(BLACK);

    MQ2.setRegressionMethod(1); //_PPM =  a*ratio^b
    MQ2.setA(36974); MQ2.setB(-3.109); // Configure the equation to to calculate LPG concentration
    MQ2.init();

    float calcR0 = 0;
    for(int i = 1; i<=10; i ++)
    {
    MQ2.update(); // Update data, the arduino will read the voltage from the analog pin
    calcR0 += MQ2.calibrate(RatioMQ2CleanAir);
    Serial.print(".");
    }
    MQ2.setR0(calcR0/10);
    lcd.println("MQ-2 calibration done.");
    delay(100);

    displayStatic();
}

void loop(void) 
{
    humidity = dht.readHumidity();
    temperature = dht.readTemperature();

    // Check if any reads failed and exit early (to try again).
    if (isnan(humidity) || isnan(temperature)) {
        return;
    }

    rzero = mq135_sensor.getRZero();
    correctedRZero = mq135_sensor.getCorrectedRZero(temperature, humidity);
    resistance = mq135_sensor.getResistance();
    ppm = mq135_sensor.getPPM();
    correctedPPM = mq135_sensor.getCorrectedPPM(temperature, humidity);

    displayMeteo();
}

// Display static texts on LCD.
unsigned long displayStatic() 
{
    lcd.setCursor(0, 0);
    lcd.setTextColor(WHITE);
    lcd.setTextSize(2);
    lcd.fillScreen(BLACK);

    lcd.println("Temperature: ");
    lcd.println("Humidity: ");
    lcd.println("");
    lcd.println("MQ-135");
    lcd.println("Rzero: ");
    lcd.println("Corrected Rzero: ");
    lcd.println("Resistance: ");
    lcd.println("PPM: ");
    lcd.println("Corrected PPM:");
    lcd.println("");
    lcd.println("MQ-2");
    lcd.print("LPG:" );
    lcd.println(" CO:");
    lcd.print("Alcohol:"); 
}

// Display real-time data from sensors on LCD.
unsigned long displayMeteo()
{
    // Temperature
    lcd.setTextColor(GREEN);
    lcd.fillRect(156, 0, 208, 16, BLACK);
    lcd.setCursor(156, 0);
    lcd.print(temperature);
    
    // Humidity
    lcd.fillRect(120, 16, 170, 16, BLACK);
    lcd.setCursor(120, 16);
    lcd.print(humidity);

    // R-zero value
    lcd.fillRect(84, 64, 120, 16, BLACK);
    lcd.setCursor(84, 64);
    lcd.print(rzero);

    // R-zero correction
    lcd.fillRect(204, 80, 240, 16, BLACK);
    lcd.setCursor(204, 80);
    lcd.print(correctedRZero);

    // Sensor resistance
    lcd.fillRect(144, 96, 180, 16, BLACK);
    lcd.setCursor(144, 96);
    lcd.print(resistance);

    // PPM
    if(ppm > 200) {
    lcd.setTextColor(RED);
    }
    lcd.fillRect(60, 112, 120, 16, BLACK);
    lcd.setCursor(60, 112);
    lcd.println(ppm);

    // Corrected PPM
    lcd.setTextColor(GREEN);
    lcd.fillRect(168, 128, 204, 16, BLACK);
    lcd.setCursor(168, 128);
    lcd.println(correctedPPM);

    // Values from MQ-2 (LPG, Alcohol, CO, etc.)
    lcd.println("");
    MQ2.setA(574.25); MQ2.setB(-2.222);
    MQ2.update(); 
    lcd.fillRect(48, 176, 72, 16, BLACK);
    lcd.setCursor(48, 176);
    lcd.setTextColor(GREY);
    lcd.print(MQ2.readSensor());

    MQ2.setA(36974); MQ2.setB(-3.109);
    MQ2.update(); 
    lcd.fillRect(156, 176, 200, 16, BLACK);
    lcd.setCursor(156, 176);
    lcd.setTextColor(GREY);
    lcd.println(MQ2.readSensor());

    MQ2.setA(658.71); MQ2.setB(-2.168);
    MQ2.update(); 
    lcd.fillRect(96, 192, 136, 16, BLACK);
    lcd.setCursor(96, 192);
    lcd.setTextColor(GREY);
    lcd.print(MQ2.readSensor());
}



