//All of our code will go here. We only need the one file.
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <HotButton.h>
#include <LiquidCrystal_I2C.h>
#include <I2CScanner.h>
#include <SPI.h>
#include <MFRC522.h>

//Pins Used
const int dispenseOK = 4;       //LED pin constant
const int needRefill = 3;       // red LED
const int dispenseButton = 5;   //Dispense button pin
const int wizardButton = 9;     //Button for Tut 

//Variables
int wizardState = 0;
int lastWizardpress = HIGH;
int wizardPress = 0;
int buttonState = 0;

  //Board will run this automatically once when it is plugged in. If we were connecting to wifi for example, it would go here
void setup() {
  //DO NOT PUT ANYTHING ABOVE THIS
  Serial.begin(115200); //This is to match the rate of the computer and the Arduino
  delay(100); //This is to add a delay, so the computer doesnt go crazy
  pinMode(dispenseOK, OUTPUT);
  pinMode (needRefill, OUTPUT);

  pinMode(dispenseButton, INPUT_PULLUP);
  pinMode(wizardButton, INPUT_PULLUP);

}
 
void loop(void) {

  Serial.println(wizardState);

  wizardPress = digitalRead(wizardButton);
  Serial.println(wizardPress);

  if (wizardPress == LOW && lastWizardpress == HIGH){
    wizardState++;
    if (wizardState > 3){
      wizardState = 0;
    }
    delay(50);
  }


  if (wizardState == 0){ //no glass, no nothin
    digitalWrite(dispenseOK, LOW);
    digitalWrite(needRefill, LOW);

  } else if (wizardState == 1){ //glass is inside of receptical, no pour yet
    digitalWrite(dispenseOK, HIGH);
    //POUR LIQUID

  } else if (wizardState == 2){ //button has been pressed
    //DELAY OF TIME FOR POURING
    digitalWrite(dispenseOK, LOW);

  } else if (wizardState == 3){ //Need refill
    digitalWrite(needRefill, HIGH);
  }

  lastWizardpress = wizardPress;

  delay(100);
} 
