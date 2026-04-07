//All of our code will go here. We only need the one file.
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <HotButton.h>
#include <LiquidCrystal_I2C.h>
#include <I2CScanner.h>

//Pins Used
const int lowLight = 3;         //LED pin constant
const int rgbPIN = 4;           //RGB pin constant
const int buttonLowerPIN = 5;   //Reduce temp pin
const int buttonHigherPIN = 6;  //increase temp pin

//Constants - Pressure reader
const int fsrPin = A2;     // the FSR and 10K pulldown are connected to a0
const int potTARE = 10;      //pot tare

//Constants - pot LEDs and Temp
const int numLEDS = 5;      //Num of RGB LEDs

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(numLEDS, rgbPIN, NEO_GRB + NEO_KHZ800);

//Variables
int pressureReading;     // the analog reading from the FSR resistor divider
int buttonLowerState = 0;
int buttonHigherState = 0;
int lastButtonHigherState = 0;
int lastButtonLowerState = 0;
int currentTemp = 0;
int lightsOn = 0;
 
//LCD screen
LiquidCrystal_I2C tempLCD(0x23, 16, 2);
LiquidCrystal_I2C weightLCD(0x27, 16, 2);

//Testing stuff
const bool THIS = true;    //Just to test things
I2CScanner scanner;

//Delarations

void colorWipe(uint32_t color, int wait);
void changeColor(uint32_t color, int wait);
bool i2cAddrTest(uint8_t addr);
String printSize(int pressure);

  //Board will run this automatically once when it is plugged in. If we were connecting to wifi for example, it would go here
void setup() {
  //DO NOT PUT ANYTHING ABOVE THIS
  Serial.begin(115200); //This is to match the rate of the computer and the Arduino
  delay(100); //This is to add a delay, so the computer doesnt go crazy
  pinMode(lowLight, OUTPUT);
  pinMode(buttonLowerPIN, INPUT_PULLUP);
  pinMode(buttonHigherPIN, INPUT_PULLUP);

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(20); // Set BRIGHTNESS to about 1/5 (max = 255)

  // if (!i2cAddrTest(0x27)) {
  //   tempLCD = LiquidCrystal_I2C(0x3F, 16, 2);
  //   weightLCD = LiquidCrystal_I2C(0x3F, 16, 2);
  // }
  tempLCD.init();
  tempLCD.noBacklight();
  weightLCD.init();
  weightLCD.noBacklight();

  scanner.Init();
}
 
void loop(void) {

  
  pressureReading = analogRead(fsrPin);  

  buttonLowerState = digitalRead(buttonLowerPIN);
  buttonHigherState = digitalRead(buttonHigherPIN);

  uint32_t lowTemp = strip.Color(125,209,15); // LOW temp colour, green
  uint32_t medTemp = strip.Color(209,129,17); // MEDIUM temp colour, yellow/orange
  uint32_t highTemp = strip.Color(117,4,4); // HIGH temp colour, red

  if (pressureReading > potTARE){ //If there is something on the sensor,
    
    tempLCD.backlight();
    weightLCD.backlight();

    if (pressureReading < 980) { //low


      Serial.print("EMPTY. Pressure:");
      Serial.println(pressureReading);
      lightsOn = 1;

      weightLCD.setCursor (0, 0);
      weightLCD.print("     REFILL      ");
      analogWrite(lowLight,HIGH);

    } else { //all good
      
      Serial.print("not empty. Pressure:");
      Serial.println(pressureReading);
      if (pressureReading < 1000){
        lightsOn = 3;
      } else {
        lightsOn = 5;
      }

      weightLCD.setCursor(0,0);
      weightLCD.print(printSize(pressureReading));
      digitalWrite(lowLight, LOW);
    }

    if (buttonLowerState == HIGH && currentTemp > 0){
      currentTemp--;
    }

    if (buttonHigherState == HIGH && currentTemp < 3){
      currentTemp++;
    }

    Serial.print(currentTemp);
    Serial.println(" Temp");

      switch (currentTemp) {
        case 1: //Temp is low
          tempLCD.setCursor(0, 0);
          tempLCD.print("      LOW       ");
          changeColor(lowTemp,lightsOn);
          break;
        case 2: //Temp is med
          tempLCD.setCursor(0, 0);
          tempLCD.print("     MEDIUM     ");
          changeColor(medTemp,lightsOn);
          break;
        case 3: //Temp is high
          tempLCD.setCursor(0, 0);
          tempLCD.print("      HIGH      ");
          changeColor(highTemp,lightsOn);
          break;
        default: //Turn lights off
          tempLCD.setCursor(0, 0);
          tempLCD.print("      OFF       ");
          changeColor(strip.Color(255,255,255), lightsOn);
          break;

      lastButtonHigherState = buttonHigherState;
      lastButtonLowerState = buttonLowerState;
    }

    } else { //No pressure at all means not on at all.
    Serial.println(" off");
    digitalWrite(lowLight, LOW);
    buttonHigherState = 0;
    buttonLowerState = 0;
    lastButtonHigherState = 0;
    lastButtonLowerState = 0;
    currentTemp = 0;

    tempLCD.clear();
    weightLCD.clear();

    tempLCD.noBacklight();
    weightLCD.noBacklight();

    //SCREEN TURNS OFF
    strip.clear();
    strip.show();
  }

  delay(500);
} 

void changeColor(uint32_t color, int lightNum){
  strip.clear();
  for(int i=0; i<lightNum; i++) {           //For each pixel according to weight
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
  }
  strip.show();                          //  Update strip to match
  delay(100);
}

String printSize(int pressure){
  String giveSize = String(pressureReading);
  int len = giveSize.length();
  if (len == 2){
    giveSize = "      " + giveSize + "  g     ";
  } else if (len == 3) {
    giveSize = "      " + giveSize +" g     ";
  } else if (len == 4) {
    giveSize = "      " + giveSize +"g     ";
  }
  return giveSize;
}