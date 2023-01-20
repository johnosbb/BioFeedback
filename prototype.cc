#include <FastLED.h>
#include <ezButton.h>


#define LED_PIN_LEFT_EYE     7
#define LED_PIN_RIGHT_EYE     8
#define NUM_LEDS    12
#define MAX_DELAY 200
#define CYCLE 0
#define FREQ_10HZ 100 //100 msec delay 


#define VRX_PIN  A0 // Arduino pin connected to VRX pin
#define VRY_PIN  A1 // Arduino pin connected to VRY pin
#define SW_PIN   2  // Arduino pin connected to SW  pin

ezButton button(SW_PIN);

int xValue = 0; // To store value of the X axis
int yValue = 0; // To store value of the Y axis
int bValue = 0; // To store value of the button
int frequency = FREQ_10HZ;
  int centre = 508;
CRGB ledsRight[NUM_LEDS];
CRGB ledsLeft[NUM_LEDS];


void setup() {

  FastLED.addLeds<WS2812, LED_PIN_RIGHT_EYE, GRB>(ledsRight, NUM_LEDS);
  FastLED.addLeds<WS2812, LED_PIN_LEFT_EYE, GRB>(ledsLeft, NUM_LEDS);
  Serial.begin(9600);

}



void joystick()
{
  char buffer[40];

  xValue = analogRead(VRX_PIN);
  yValue = analogRead(VRY_PIN);
  button.loop();
  sprintf(buffer,"Raw:  X: %d, Y: %d \n", xValue,yValue);
  Serial.print(buffer);
  Serial.print("\n");
  if(xValue >= centre)
    xValue = (xValue * 1) - centre;
  else
    xValue = (centre - xValue) * -1;
  sprintf(buffer,"Transformed: X: %d, Y: %d \n", xValue,yValue);
  Serial.print(buffer);
  Serial.print("\n");

    // Read the button value
  bValue = button.getState();

  if (button.isPressed()) {
    Serial.println("The button is pressed");
    // TODO do something here
  }

  if (button.isReleased()) {
    Serial.println("The button is released");
    // TODO do something here
  }

  Serial.print("\n");
}



void flashAlternative(int delay_period, unsigned long int color)
{
  int i = 0;

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

  joystick();
  if(1)
  {
    char buffer[40];
    frequency = FREQ_10HZ + (xValue * .203);
    sprintf(buffer," Setting frequency to %d \n", frequency);
    Serial.print(buffer);
    Serial.print("\n");
  }
  flashAlternative(frequency, CRGB::Aqua);
}
