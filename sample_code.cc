#include <FastLED.h>

#define LED_PIN_LEFT_EYE     7
#define LED_PIN_RIGHT_EYE     8
#define NUM_LEDS    12
#define MAX_DELAY 200
#define CYCLE 0
#define FREQ_10HZ 100 //100 msec delay 

CRGB leds[NUM_LEDS];
CRGB ledsLeft[NUM_LEDS];


void setup() {

  FastLED.addLeds<WS2812, LED_PIN_RIGHT_EYE, GRB>(leds, NUM_LEDS);
  FastLED.addLeds<WS2812, LED_PIN_LEFT_EYE, GRB>(ledsLeft, NUM_LEDS);
  Serial.begin(9600);
  //basic_demo();
}


void flashAlternative(int delay_period, unsigned long int color)
{
  int i = 0;
  Serial.print(color);
  for(i =0;i < NUM_LEDS;i++)
  {
    //leds[i].fadeToBlackBy( 64 );
    leds[i] = CRGB::Black;
    FastLED.show();
    ledsLeft[i] = color;
    FastLED.show();
  }
  delay(delay_period/2);
  for(i =0;i < NUM_LEDS;i++)
  {
    leds[i] = color;
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
    glassLeds = leds;
  else
    glassLeds = ledsLeft;
  Serial.print(color);
  for(i =0;i < NUM_LEDS;i++)
  {
    //leds[i].fadeToBlackBy( 64 );
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


void basic_demo()
{

  leds[0] = CRGB(255, 0, 0);
  FastLED.show();
  delay(500);  
  leds[1] = CRGB(0, 255, 0);
  FastLED.show();
  delay(500);
  leds[2] = CRGB(0, 0, 255);
  FastLED.show();
  delay(500);
  leds[3] = CRGB(0, 0, 255);
  FastLED.show();
  delay(500);
  leds[4] = CRGB(150, 0, 255);
  FastLED.show();
  delay(500);
  leds[5] = CRGB(150, 0, 255);
  FastLED.show();
  delay(500);
  leds[6] = CRGB(255, 200, 20);
  FastLED.show();
  delay(500);
  leds[7] = CRGB(85, 60, 180);
  FastLED.show();
  delay(500);
  leds[8] = CRGB(50, 255, 20);
  FastLED.show();
  delay(500);
  leds[9] = CRGB(50, 255, 20);
  FastLED.show();
  delay(500);
  leds[10] = CRGB(50, 255, 20);
  FastLED.show();
  delay(500);
  leds[11] = CRGB(50, 255, 20);
  FastLED.show();
  delay(500);
  leds[12] = CRGB(50, 255, 20);
  FastLED.show();
  delay(500);

}


void loop() {

  //delay(3000);
  // int i = 0;
  // for(i =0;i < NUM_LEDS;i++)
  // {
  //   //leds[i].fadeToBlackBy( 64 );
  //   leds[i].nscale8( 192);
  //   FastLED.show();
  // }
  // for(i =0;i < NUM_LEDS;i++)
  // {
  //   leds[i] = CRGB::Black;
  //   FastLED.show();
  // }
  if( CYCLE )
  {
    for(int i =0;i < MAX_DELAY;i++)
    {
      flash(LED_PIN_RIGHT_EYE ,i,CRGB::Aqua);
    }
    for(int i =0;i < MAX_DELAY;i++)
    {
      flash(LED_PIN_RIGHT_EYE, i,CRGB::Blue);
    }
    for(int i =0;i < MAX_DELAY;i++)
    {
      flash(LED_PIN_RIGHT_EYE, i,CRGB::CornflowerBlue);
    }
  }
  // for(int i =0;i < MAX_DELAY;i++)
  // {
  //   flash(i,0xFF0000);
  // }
  // flash(LED_PIN_RIGHT_EYE ,FREQ_10HZ,CRGB::Aqua);
  // flash(LED_PIN_LEFT_EYE ,FREQ_10HZ,CRGB::Red);
  flashAlternative(FREQ_10HZ, CRGB::Aqua);

}
