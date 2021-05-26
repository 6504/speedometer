#include <FastLED.h>

// Limit brightness of lights. Max is 255
#define DISPLAY_BRIGHTNESS 50
#define UNDERGLOW_BRIGHTNESS 255

// Ring light info
#define NUM_RING_LEDS 24
#define RING_DATA_PIN 6

// Strip light info
#define NUM_STRIP_LEDS 20
#define STRIP_DATA_PIN 7

// Strip 2 light info
#define NUM_STRIP2_LEDS 20
#define STRIP2_DATA_PIN 8

// Underglow light info
#define NUM_UNDERGLOW_LEDS 180
#define UNDERGLOW_DATA_PIN 9

// Current colors of lights
CRGB ringLeds[NUM_RING_LEDS];
CRGB stripLeds[NUM_STRIP_LEDS];
CRGB strip2Leds[NUM_STRIP2_LEDS];
CRGB underglowLeds[NUM_UNDERGLOW_LEDS];

int boostDelay;
int colorSkip;

void setup() {
  Serial.begin(9600);
  
  //FastLED.setBrightness(MAX_BRIGHTNESS);
  //FastLED.setMaxPowerInVoltsAndMilliamps(5, 2000); // Set max amps to 2 amps (2000 milliamps) at 5 volts
  FastLED.addLeds<NEOPIXEL, RING_DATA_PIN>(ringLeds, NUM_RING_LEDS);
  FastLED.addLeds<NEOPIXEL, STRIP_DATA_PIN>(stripLeds, NUM_STRIP_LEDS);
  FastLED.addLeds<NEOPIXEL, STRIP2_DATA_PIN>(strip2Leds, NUM_STRIP2_LEDS);
  FastLED.addLeds<NEOPIXEL, UNDERGLOW_DATA_PIN>(underglowLeds, NUM_UNDERGLOW_LEDS);
  FastLED.clear();
  FastLED.show();

  // Reset all lights to white
  for (int i = 0; i < NUM_RING_LEDS; i++) {
    ringLeds[i] = CHSV(0, 0, DISPLAY_BRIGHTNESS);
  }
  for (int i = 0; i < NUM_STRIP_LEDS; i++) {
    stripLeds[i] = CHSV(0, 0, DISPLAY_BRIGHTNESS);
  }
  for (int i = 0; i < NUM_STRIP2_LEDS; i++) {
    strip2Leds[i] = CHSV(0, 0, DISPLAY_BRIGHTNESS);
  }
  for (int i = 0; i < NUM_UNDERGLOW_LEDS; i++) {
    underglowLeds[i] = CHSV(0, 0, UNDERGLOW_BRIGHTNESS);
  }

  boostDelay = 100;
  colorSkip = 5;
  
  FastLED.show();
}


int j = 0;

void loop()
{
  // Recalculate colors for underglow
  for(int i = 0; i < NUM_UNDERGLOW_LEDS; i++) {
    underglowLeds[i] = Scroll((i * 256 / NUM_UNDERGLOW_LEDS + j) % 256);      
  } 

  FastLED.show();
  delay(boostDelay);
  j+=colorSkip;
}

CRGB Scroll(int pos) {
  CRGB color (0,0,0);
  if(pos < 85) {
    color.g = 0;
    color.r = ((float)pos / 85.0f) * 255.0f;
    color.b = 255 - color.r;
  } else if(pos < 170) {
    color.g = ((float)(pos - 85) / 85.0f) * 255.0f;
    color.r = 255 - color.g;
    color.b = 0;
  } else if(pos < 256) {
    color.b = ((float)(pos - 170) / 85.0f) * 255.0f;
    color.g = 255 - color.b;
    color.r = 1;
  }
  return color;
}

/* Listen for serial events from the USB connection
 * Available commands: 
 *  r<num LEDs> <hue>
 *  s<num LEDs> <hue>
 *  S<num LEDs> <hue>
 * 
 * Replace <num LEDs> with the number of LEDs to light up
 * Replace <hue> with the color of the lights, where 0 or 255 is red, 85 is green, 170 is blue
 * 
 * Example 1:
 * Light up four LEDs on the ring light and make them red
 *  r4 0
 * 
 * Light up seven LEDs on the ring light and make them green
 *  s7 85
 *  
 */
void serialEvent() {
  while (Serial.available()) {
    // Get the first character and interpret it as a command
    char cmdChar = (char)Serial.read();

    // Ring light update command
    if (cmdChar == 'r') {
      // Read the number of LEDs to light up
      int numLeds = Serial.parseInt();

      // Read the separator character
      Serial.read();
      int hue = Serial.parseInt();

      int i;

      // Light up a number of LEDs in the right color
      for (i = 0; i < NUM_RING_LEDS && i < numLeds; i++) {
        ringLeds[i] = CHSV(hue, 255, DISPLAY_BRIGHTNESS);
      }

      // The rest of the LEDs should be black
      for (; i < NUM_RING_LEDS; i++) {
        ringLeds[i] = CRGB::Black;
      }
      FastLED.show();
    } else if (cmdChar == 's') {
      // Strip light update command
      
      // Read the number of LEDs to light up
      int numLeds = Serial.parseInt();
      
      // Read the separator character
      Serial.read();
      int hue = Serial.parseInt();

      int i;
      
      // Light up a number of LEDs in the right color
      for (i = 0; i < NUM_STRIP_LEDS && i < numLeds; i++) {
        stripLeds[i] = CHSV(hue, 255, DISPLAY_BRIGHTNESS);
      }
      
      // The rest of the LEDs should be black
      for (; i < NUM_STRIP_LEDS; i++) {
        stripLeds[i] = CRGB::Black;
      }
      FastLED.show();
    } else if (cmdChar == 'S') {
      // Strip 2 light update command
      
      // Read the number of LEDs to light up
      int numLeds = Serial.parseInt();
      
      // Read the separator character
      Serial.read();
      int hue = Serial.parseInt();

      int i;
      
      // Light up a number of LEDs in the right color
      for (i = 0; i < NUM_STRIP2_LEDS && i < numLeds; i++) {
        strip2Leds[i] = CHSV(hue, 255, DISPLAY_BRIGHTNESS);
      }
      
      // The rest of the LEDs should be black
      for (; i < NUM_STRIP2_LEDS; i++) {
        strip2Leds[i] = CRGB::Black;
      }
      FastLED.show();
    } else if (cmdChar == 'u') {
      boostDelay=100;
      colorSkip = 5;
    } else if (cmdChar == 'U') {
      boostDelay=5;
      colorSkip = 40;
    }
  }
}
