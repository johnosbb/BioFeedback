#include <FastLED.h>
#include <ezButton.h>
#include <Keypad.h>
#include <math.h>
#include <stdlib.h>



#define LED_PIN_LEFT_EYE     7
#define LED_PIN_RIGHT_EYE     8
#define NUM_LEDS    12
#define MAX_DELAY 200
#define CYCLE 0
#define FREQ_10HZ 100 //100 msec delay 
#define VERBOSE 1
#define SHOW_HSV_TO_RGB_CALCULATIONS 0

const byte ROWS = 1; // number of rows
const byte COLS = 4; // number of columns

char keys[ROWS][COLS] = {
{'1','2','3','4'}
};
byte rowPins[ROWS] = {9};
byte colPins[COLS] = {10,11, 12, 13};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

#define VRX_PIN  A0 // Arduino pin connected to VRX pin
#define VRY_PIN  A1 // Arduino pin connected to VRY pin
#define SW_PIN   2  // Arduino pin connected to SW  pin

ezButton button(SW_PIN);

int xValue = 0; // To store value of the X axis
int yValue = 0; // To store value of the Y axis
int bValue = 0; // To store value of the button
int frequency = FREQ_10HZ;
int centre = 508;
double Value = 1.0;
double Saturation = 1.0;
unsigned long color = CRGB::Aqua;
CRGB ledsRight[NUM_LEDS];
CRGB ledsLeft[NUM_LEDS];
CRGB ledReference;
unsigned long colors[12] = {CRGB::DarkBlue, CRGB::MidnightBlue, CRGB::Navy, CRGB::MediumBlue, CRGB::DarkSlateBlue, CRGB::Aqua , CRGB::Indigo, CRGB::DarkViolet, CRGB::DarkMagenta , CRGB::DeepPink, CRGB::Crimson,  CRGB::Red};

void setup() {

  FastLED.addLeds<WS2812, LED_PIN_RIGHT_EYE, GRB>(ledsRight, NUM_LEDS);
  FastLED.addLeds<WS2812, LED_PIN_LEFT_EYE, GRB>(ledsLeft, NUM_LEDS);
  Serial.begin(9600);

}



void HSVToRGB(double hue, double saturation, double value, int* rgb)
{
  double M = 255 * value;
  double m = M *(1 - saturation);
  double z = (M-m) * (1 - fabs((((int)hue/60)%2)-1));
  int r_prime, g_prime, b_prime;
  if (hue >= 300) {
    r_prime = M;
    g_prime = 0;
    b_prime = z + M;
    
  } else if (hue >= 240) {
    r_prime = z + m;
    g_prime = 0;
    b_prime = M;
    
  } else if (hue >= 180) {
    r_prime = 0;
    g_prime = z + m;
    b_prime = M;
    
  } else if (hue >= 120) {
    r_prime = 0;
    g_prime = M;
    b_prime = z + m;
    
  } else if (hue >= 60) {
    r_prime = z + m;
    g_prime = M;
    b_prime = 0;

  } else { // hue >= 0
    r_prime = M;
    g_prime = z + m;
    b_prime = 0;
  }

    if (SHOW_HSV_TO_RGB_CALCULATIONS) {
    Serial.print("Hue: ");
    Serial.print(hue);
    Serial.print("\n");
    Serial.print("Value: ");
    Serial.print(value);
    Serial.print("\n");
    Serial.print("Saturation: ");
    Serial.print(saturation);
    Serial.print("\n");
    Serial.print("m: ");
    Serial.print(m);
    Serial.print("\n");
    Serial.print("M: ");
    Serial.print(m);
    Serial.print("\n");
    Serial.print("z: ");
    Serial.print(z);
    Serial.print("\n");
    Serial.print("r_prime: ");
    Serial.print(r_prime);
    Serial.print("\n");
    Serial.print("g_prime: ");
    Serial.print(g_prime);
    Serial.print("\n");
    Serial.print("b_prime: ");
    Serial.print(b_prime);
    Serial.print("\n");
  }    

  rgb[0] = (r_prime);
  rgb[1] = (g_prime);
  rgb[2] = (b_prime);

}



void joystick()
{
  char buffer[40];

  xValue = analogRead(VRX_PIN);
  yValue = analogRead(VRY_PIN);
  button.loop();
    // Read the button value
  bValue = button.getState();

  if (button.isPressed()) {
    Serial.println("The joystick button is pressed");
    Saturation = 1;
    Value = 1;
    // TODO do something here
  }

  if (button.isReleased()) {
    Serial.println("The joystick button is released");
    // TODO do something here
  }
  if(VERBOSE)
  {
    sprintf(buffer,"Raw:  X: %d, Y: %d \n", xValue,yValue);
    Serial.print(buffer);
    Serial.print("\n");
  }
  if ( xValue > 490 && xValue < 534 && yValue > 490 && yValue < 534) {
    ledReference = CRGB::White;

  // Otherwise, calculate hue angle & convert to RGB vals
  // It is assumed/hardcoded for S & V == 1 (in HSV mode)
  } else {

    // Floor round x and y values by the 10's place
    // Eliminates weird arithmetic issues caused by imperfect
    // analog feedback (e.g. max val for each is 1024; only gets to 1018)
    float x_val = xValue/10-51; 
    float y_val = yValue/10-51;

    if (VERBOSE) {
      Serial.print("x_val: ");
      Serial.print(x_val);
      Serial.print("\n");
      Serial.print("y_val: ");
      Serial.print(y_val);
      Serial.print("\n");
    }
        // Use arctan for the hue angle in radians from [-pi,pi]
    double hue = atan2(y_val,x_val);

    if (VERBOSE) {
      Serial.print("Raw Hue: ");
      Serial.print(hue);
      Serial.print("\n\n");
 
    } 
    // Adjust the hue to be in range [0,2pi]
    // then convert the hue from rad to degrees
    hue = hue < 0 ? hue + 2 * 3.14 : hue;
    hue = hue/6.28 * 360;

    if (VERBOSE) {
      Serial.print("Hue: ");
      Serial.print(hue);
      Serial.print("\n\n");
 
    } 

    // Get the RGB vals, change the color accordingly
    int* rgb = (int*) malloc (3 * sizeof(int));
    float v = (abs(x_val)/47)/2 + ((abs(y_val)/47)/2);    
    // getRGBVals(hue, rgb,.2,.2);
    HSVToRGB(hue, Saturation, Value,rgb);
    //getRGBVals(hue, rgb);
    ledReference.setRGB(rgb[0],rgb[1],rgb[2]);
    free(rgb);

  }
}




void flashAlternative(int delay_period, unsigned long int color)
{
  int i = 0;

  for(i =0;i < NUM_LEDS;i++)
  {
    //leds[i].fadeToBlackBy( 64 );
    ledsRight[i] = CRGB::Black;
    FastLED.show();
    ledsLeft[i].r = ledReference.r;
    ledsLeft[i].g = ledReference.g;
    ledsLeft[i].b = ledReference.b;
    FastLED.show();
  }
  delay(delay_period/2);
  for(i =0;i < NUM_LEDS;i++)
  {
    ledsRight[i].r = ledReference.r;
    ledsRight[i].g = ledReference.g;
    ledsRight[i].b = ledReference.b;
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
  // Serial.print(color);
  for(i =0;i < NUM_LEDS;i++)
  {
    glassLeds[i] = CRGB::Black;
    FastLED.show();
  }
  delay(delay_period/2);
  for(i =0;i < NUM_LEDS;i++)
  {
    glassLeds[i].r = ledReference.r;
    glassLeds[i].g = ledReference.g;
    glassLeds[i].b = ledReference.b;
    FastLED.show();
  }
  delay(delay_period/2);
}


void handleKeys(byte key)
{
  if(key == '1')
  {
    //Serial.println("Received number 1");
    Saturation += .1;
    if(Saturation > 1)
      Saturation = 1;

  }
  else if(key == '2')
  {
    //Serial.println("Received number 2");
    Saturation -= .1;
    if(Saturation < 0)
      Saturation = 0;
  }
  else if(key == '3')
  {
    //Serial.println("Received number 3");
    Value += .1;
    if(Value > 1)
      Value = 1;

  }
  else if(key == '4')
  {
    //Serial.println("Received number 4");
    Value -= .1;
    if(Value < 0)
      Value = 0;
  }
  else
  {
    Serial.println("Received unknown key");
  }

  
}


void loop() {
  char key = keypad.getKey(); 
  char buffer[40];
  if (key != NO_KEY)
  {
    sprintf(buffer,"Key: %c \n", key);
    Serial.print(buffer);
    Serial.print("\n");  
    handleKeys(key);
  }
  joystick();

    
    // frequency = FREQ_10HZ + (xValue * .203);
    frequency = FREQ_10HZ + (xValue * .203);
    if (VERBOSE) {
    Serial.print("Saturation: ");
    Serial.print(Saturation);
    Serial.print("\n");
    Serial.print("Value: ");
    Serial.print(Value);
    Serial.print("\n");

  } 
    if(VERBOSE)
    {
      sprintf(buffer," Setting frequency to %d \n", frequency);
      Serial.print(buffer);
      Serial.print("\n");
    }
  flashAlternative(frequency, color);
}
