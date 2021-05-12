#include <FastLED.h>

// Limit brightness of lights. Max is 255
#define MAX_BRIGHTNESS          30

// Ring light info
#define NUM_RING_LEDS 24
#define RING_DATA_PIN 6

// Strip light info
#define NUM_STRIP_LEDS 20
#define STRIP_DATA_PIN 7

// Current colors of lights
CRGB ringLeds[NUM_RING_LEDS];
CRGB stripLeds[NUM_STRIP_LEDS];

void setup() {
  Serial.begin(9600);
  
  FastLED.setBrightness(MAX_BRIGHTNESS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 2000); // Set max amps to 2 amps (2000 milliamps) at 5 volts
  FastLED.addLeds<NEOPIXEL, RING_DATA_PIN>(ringLeds, NUM_RING_LEDS);
  FastLED.addLeds<NEOPIXEL, STRIP_DATA_PIN>(stripLeds, NUM_STRIP_LEDS);
  FastLED.clear();
  FastLED.show();

  // Reset all lights to white
  for (int i = 0; i < NUM_RING_LEDS; i++) {
    ringLeds[i] = CRGB::White;
  }
  for (int i = 0; i < NUM_STRIP_LEDS; i++) {
    stripLeds[i] = CRGB::White;
  }
  
  FastLED.show();
}

void loop()
{
  
}

/* Listen for serial events from the USB connection
 * Available commands: 
 *  r<num LEDs> <hue>
 *  s<num LEDs> <hue>
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
        ringLeds[i] = CHSV(hue, 255, MAX_BRIGHTNESS);
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
        stripLeds[i] = CHSV(hue, 255, MAX_BRIGHTNESS);
      }
      
      // The rest of the LEDs should be black
      for (; i < NUM_STRIP_LEDS; i++) {
        stripLeds[i] = CRGB::Black;
      }
      FastLED.show();
    }
  }
}
