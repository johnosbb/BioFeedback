#include <FastLED.h>

#define LED_PIN_LEFT_EYE     7
#define LED_PIN_RIGHT_EYE     8
#define NUM_LEDS    12
#define MAX_DELAY 200
#define CYCLE 0
#define FREQ_10HZ 100 //100 msec delay 

CRGB ledsRight[NUM_LEDS];
CRGB ledsLeft[NUM_LEDS];


void setup() {

  FastLED.addLeds<WS2812, LED_PIN_RIGHT_EYE, GRB>(ledsRight, NUM_LEDS);
  FastLED.addLeds<WS2812, LED_PIN_LEFT_EYE, GRB>(ledsLeft, NUM_LEDS);
  Serial.begin(9600);

}


void flashAlternative(int delay_period, unsigned long int color)
{
  int i = 0;
  Serial.print(color);
  for(i =0;i < NUM_LEDS;i++)
  {
    //leds[i].fadeToBlackBy( 64 );
    ledsRight[i] = CRGB::Black;
    FastLED.show();
    ledsLeft[i] = color;
    FastLED.show();
  }
  delay(delay_period/2);
  for(i =0;i < NUM_LEDS;i++)
  {
    ledsRight[i] = color;
    FastLED.show();
    ledsLeft[i] = CRGB::Black;
    FastLED.show();
  }
  delay(delay_period/2);
}




void flash(int eye,int delay_period, unsigned long int color)
{
  int i = 0;
  CRGB * glassLeds;
  if(eye == LED_PIN_RIGHT_EYE )
    glassLeds = ledsRight;
  else
    glassLeds = ledsLeft;
  Serial.print(color);
  for(i =0;i < NUM_LEDS;i++)
  {
    glassLeds[i] = CRGB::Black;
    FastLED.show();
  }
  delay(delay_period/2);
  for(i =0;i < NUM_LEDS;i++)
  {
    glassLeds[i] = color;
    FastLED.show();
  }
  delay(delay_period/2);
}





void loop() {
  flashAlternative(FREQ_10HZ, CRGB::Aqua);
}
