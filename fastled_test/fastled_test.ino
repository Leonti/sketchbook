#include "FastLED.h"

#define LED_PIN     7
#define NUM_LEDS 50
#define CHIPSET     WS2811
#define COLOR_ORDER RGB
#define BRIGHTNESS  80

CRGB leds[NUM_LEDS];
void setup() { 
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness( BRIGHTNESS );
}
void loop() { 
    leds[0] = CRGB(255,119,0); 
    leds[9] = CRGB::Red;
    leds[7] = CRGB::Red;
    leds[49] = CRGB::Green;
    FastLED.show(); 
    delay(300); 
    leds[0] = CRGB::Black;
    leds[9] = CRGB::Black;
    leds[7] = CRGB::Black;
    leds[49] = CRGB::Black;
    FastLED.show(); 
    delay(300);
}
