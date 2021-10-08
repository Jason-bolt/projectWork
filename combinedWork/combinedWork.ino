#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#define CAYENNE_PRINT Serial
#include <CayenneMQTTESP32.h>

#define PhSensorPin 36          // the pH meter Analog output is connected with the Arduino’s Analog
unsigned long int avgValue;  //Store the average value of the sensor feedback
float b;
int buf[10],temp;

LiquidCrystal_I2C lcd(0x3F,16,2);  // CHANGE the "0x27" ADRESS FOR YOUR SCREEN CONFIGURATION

const int TempSensorPin = 25; // ESP32 pin connected to DS18B20 sensor's DQ pin
OneWire oneWire(TempSensorPin);         // setup a oneWire instance
DallasTemperature tempSensors(&oneWire); // pass oneWire to DallasTemperature library

float tempCelsius;    // temperature in Celsius
float tempFahrenheit; // temperature in Fahrenheit

// WiFi network info.
char ssid[] = "JASON 6080";
char wifiPassword[] = "19(Y6x81";


// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "50457b00-f653-11e9-a38a-d57172a4b4d4";
char password[] = "d02f0fa606e75ca84124ef4564b50c5c292b6886";
char clientID[] = "a5aaa7c0-10c1-11ec-9b2d-abd5fca49150";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);  
  Serial.println("Ready");    //Test the serial monitor
  tempSensors.begin();    // initialize the sensor
  Cayenne.begin(username, password, clientID, ssid, wifiPassword);
  lcd.init();                      // initialize the lcd
  lcd.clear();

// Print a message to the LCD.
  lcd.backlight();
//  lcd.setCursor(0, 0);  
//  lcd.print("IT WORKS ! :D");
}

void loop() {
  // put your main code here, to run repeatedly:
  float ph = pH_sensor();
  float temp = temp_sensor();
  lcd_display(ph, temp);
  delay(500);

  Cayenne.loop();
}




float pH_sensor(){
  for(int i=0;i<10;i++)       //Get 10 sample value from the sensor for smooth the value
  { 
    buf[i]=analogRead(PhSensorPin);
    delay(10);
  }
  for(int i=0;i<9;i++)        //sort the analog from small to large
  {
    for(int j=i+1;j<10;j++)
    {
      if(buf[i]>buf[j])
      {
        temp=buf[i];
        buf[i]=buf[j];
        buf[j]=temp;
      }
    }
  }
  avgValue=0;
  for(int i=2;i<8;i++)                      //take the average value of 6 center sample
    avgValue+=buf[i];
  float phValue=(float)avgValue*5.0/4095/6; //convert the analog into millivolt
  phValue=3.5*phValue - 4.87;                      //convert the millivolt into pH value
  Serial.print("    pH:");  
  Serial.print(phValue,2);
  Serial.println(" ");

  return phValue;
}

void lcd_display(float ph_display, float temp_display){
  lcd.clear();

  String ph = String(ph_display);
  String temp = String(temp_display);
// Print a message to the LCD.
  lcd.setCursor(0, 0);  
  lcd.print("pH: " + ph);
  lcd.setCursor(0, 1);  
  lcd.print("Temp: " + temp);
}

float temp_sensor(){
  tempSensors.requestTemperatures();             // send the command to get temperatures
  tempCelsius = tempSensors.getTempCByIndex(0);  // read temperature in Celsius
  tempFahrenheit = tempCelsius * 9 / 5 + 32; // convert Celsius to Fahrenheit

  Serial.print("Temperature: ");
  Serial.print(tempCelsius);    // print the temperature in Celsius
  Serial.print("°C");
  Serial.print("  ~  ");        // separator between Celsius and Fahrenheit
  Serial.print(tempFahrenheit); // print the temperature in Fahrenheit
  Serial.println("°F");

  return tempCelsius;
//  delay(500);
}


// Default function for sending sensor data at intervals to Cayenne.
// You can also use functions for specific channels, e.g CAYENNE_OUT(1) for sending channel 1 data.
CAYENNE_OUT_DEFAULT()
{
   float temp = temp_sensor();
   float ph = pH_sensor();
  // Write data to Cayenne here. This example just sends the current uptime in milliseconds on virtual channel 0.
//  Cayenne.virtualWrite(0, millis());
  Cayenne.virtualWrite(1, temp);
  Cayenne.virtualWrite(2, ph);
  // Some examples of other functions you can use to send data.
//  Cayenne.celsiusWrite(1, 22.0);
  //Cayenne.luxWrite(2, 700);
  //Cayenne.virtualWrite(3, 50, TYPE_PROXIMITY, UNIT_CENTIMETER);
}

// Default function for processing actuator commands from the Cayenne Dashboard.
// You can also use functions for specific channels, e.g CAYENNE_IN(1) for channel 1 commands.
//CAYENNE_IN_DEFAULT()
//{
//  CAYENNE_LOG("Channel %u, value %s", request.channel, getValue.asString());
//  //Process message here. If there is an error set an error message using getValue.setError(), e.g getValue.setError("Error message");
//}
