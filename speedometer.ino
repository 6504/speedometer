#include <FastLED.h>

#define MAX_BRIGHTNESS          100
#define NUM_RING_LEDS 24
#define RING_DATA_PIN 6

#define NUM_STRIP_LEDS 120
#define STRIP_DATA_PIN 7

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
  
  for (int i = 0; i < NUM_RING_LEDS; i++) {
    ringLeds[i] = CRGB::Black;
  }
  for (int i = 0; i < NUM_STRIP_LEDS; i++) {
    stripLeds[i] = CRGB::Black;
  }
  
  FastLED.show();
}

void loop()
{
  
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();

    if (inChar == 'r') {
      int numLeds = Serial.parseInt();
      Serial.read();
      int hue = Serial.parseInt();

      int i;
      for (i = 0; i < NUM_RING_LEDS && i < numLeds; i++) {
        ringLeds[i] = CHSV(hue, 255, MAX_BRIGHTNESS);
      }
      
      for (; i < NUM_RING_LEDS; i++) {
        ringLeds[i] = CRGB::Black;
      }
      FastLED.show();
    } else if (inChar == 's') {
      int numLeds = Serial.parseInt();
      Serial.read();
      int hue = Serial.parseInt();

      int i;
      for (i = 0; i < NUM_STRIP_LEDS && i < numLeds; i++) {
        stripLeds[i] = CHSV(hue, 255, MAX_BRIGHTNESS);
      }
      for (; i < NUM_STRIP_LEDS; i++) {
        stripLeds[i] = CRGB::Black;
      }
      FastLED.show();
    }
  }
}
