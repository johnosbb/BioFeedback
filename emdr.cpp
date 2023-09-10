/* Start of Code */

#include "FastLED.h"
#include <SoftwareSerial.h>


// How many leds are connected?
#define NUM_LEDS 16

// Define the Pins
#define DATA_PIN 2
#define COLOR_PIN 7
#define RATE_PIN 8

constexpr byte RX {13};
constexpr byte TX {14};
SoftwareSerial softSerial(RX, TX);

// Define the array of leds
CRGB leds[NUM_LEDS];

long PreferredColour = CRGB::Blue;
unsigned int PreferredOffDelay = 50;
unsigned int PreferredOnDelay = 200;
unsigned int offDelay = PreferredOffDelay;
unsigned int onDelay = PreferredOnDelay;
unsigned int color = 0;
unsigned int rate = 0;
unsigned int sensorValue = 0;
unsigned long timeNow = millis();

  
void setup() { 
  FastLED.addLeds<NEOPIXEL,DATA_PIN>(leds, NUM_LEDS);
  pinMode(COLOR_PIN, INPUT);
  pinMode(RATE_PIN, INPUT);
  pinMode(A7, INPUT);
  softSerial.begin(9600);
	softSerial.print("Hello World");
}



void LeftToRightLong()
{
    // Change led colors
  for(int led = 0; led < NUM_LEDS; led+=2)
  { 
    detect_control_inputs();
    leds[led] = PreferredColour;
    leds[led+1] = PreferredColour;
    FastLED.setBrightness(50);
    FastLED.show();
    delay(onDelay);
    leds[led] = CRGB::Black;
    leds[led+1] = CRGB::Black;
    FastLED.setBrightness(50);
    FastLED.show();
    delay(offDelay);
  }
}


void RightToLeftLong()
{
    // Change led colors
  for(int led = NUM_LEDS; led > -1; led-=2)
  { 
    detect_control_inputs();
    leds[led] = PreferredColour;
    leds[led-1] = PreferredColour;
    FastLED.setBrightness(50);
    FastLED.show();
    delay(onDelay);
    leds[led] = CRGB::Black;
    leds[led-1] = CRGB::Black;
    FastLED.setBrightness(50);
    FastLED.show();
    delay(offDelay);
  }
}


void detect_control_inputs()
{
  onDelay = analogRead(A7);
  color = digitalRead(COLOR_PIN); // pin 7
  rate = digitalRead(RATE_PIN); // pin 8
  softSerial.print("Knob Value: ");
  softSerial.println(sensorValue);
  unsigned long difference = millis() - timeNow;
  timeNow = millis();
  if(color == 1 && (difference > 1000)) // debounce
  {
    if(PreferredColour == CRGB::Red )
    {
      softSerial.print("Color Value (Red): ");
      softSerial.println(color);
      PreferredColour = CRGB::Blue;    
    }    
    else
    {
      softSerial.print("Color Value: (Blue)");
      softSerial.println(color);
      PreferredColour = CRGB::Red;
    }
  }  
  else
  {
    softSerial.print("Color Value: ");
    softSerial.println(color);

  } 
  if(rate == 0) // normally 1
  {
    softSerial.print("Rate Value: ");
    softSerial.println(rate);
    
  }  
}


void loop() { 

  //Serial.println(sensorValue);  
  // Clear the existing led values
  FastLED.clear();
  detect_control_inputs();
  LeftToRightLong();
  detect_control_inputs();
  RightToLeftLong();
  detect_control_inputs();
  delay(sensorValue);


 





}





