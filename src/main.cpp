//All of our code will go here. We only need the one file.
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <HotButton.h>

//Pins Used
const int lowLight = 3;         //LED pin constant
const int rgbPIN = 4;           //RGB pin constant
const int buttonLowerPIN = 5;   //Reduce temp pin
const int buttonHigherPIN = 6;  //increase temp pin

//Constants - Pressure reader
const int fsrPin = A4;     // the FSR and 10K pulldown are connected to a0
const int potTARE = 10;      //pot tare

//Constants - pot LEDs and Temp
const int numLEDS = 3;      //Num of RGB LEDs

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(numLEDS, rgbPIN, NEO_GRB + NEO_KHZ800);

//Variables
int pressureReading;     // the analog reading from the FSR resistor divider
int buttonLowerState = 0;
int buttonHigherState = 0;
int lastButtonHigherState = 0;
int lastButtonLowerState = 0;
int currentTemp = 0;
 

//Testing stuff
const bool THIS = true;    //Just to test things

//Delarations

void colorWipe(uint32_t color, int wait);
void changeColor(uint32_t color, int wait);

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
}
 
void loop(void) {

  pressureReading = analogRead(fsrPin);  

  buttonLowerState = digitalRead(buttonLowerPIN);
  buttonHigherState = digitalRead(buttonHigherPIN);

  uint32_t lowTemp = strip.Color(114,190,65); // LOW temp colour, green
  uint32_t medTemp = strip.Color(235,172,30); // MEDIUM temp colour, yellow/orange
  uint32_t highTemp = strip.Color(117,40,9); // HIGH temp colour, red

  if (pressureReading > potTARE){ //If there is something on the sensor,

    if (pressureReading < 800) { //low
      Serial.println("EMPTY");
      analogWrite(lowLight,HIGH);

    } else { //all good
      Serial.println(" Not Empty");
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
          changeColor(lowTemp,100);
          break;
        case 2: //Temp is med
          changeColor(medTemp,100);
          break;
        case 3: //Temp is high
          changeColor(highTemp,100);
          break;
        default: //Turn lights off
          strip.clear();
          strip.show();
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
    //SCREEN TURNS OFF
    strip.clear();
    strip.show();
  }

  delay(500);
} 

void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

void changeColor(uint32_t color, int wait){
    for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
  }
  strip.show();                          //  Update strip to match
  delay(wait);
}