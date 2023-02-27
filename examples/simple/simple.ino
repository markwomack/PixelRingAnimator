//
// Licensed under Apache 2.0 license.
// See accompanying LICENSE file for details.
//

// This is an example sketch that uses the PixelRingAnimator
// to drive some sample animations. You can substitute your
// own values to different colors used in the animation.

// Arduino includes
#include <Arduino.h>

// Pixel Ring includes
#include <PixelRingAnimator.h>

//*** Modify these values to match your circuit and pixel ring setup
//
const uint8_t PIXEL_RING_PIN(39);                         // pixel ring data pin
const uint16_t NUM_PIXELS(16);                            // the number of pixels on ring
const neoPixelType NEO_PIXEL_TYPE(NEO_GRBW + NEO_KHZ800); // the type of neo pixels on ring

PixelRingAnimator* pixelRingAnimator;

uint32_t nextUpdateTime;
uint32_t nextEffectChangeTime;
int effect;

// Sets up a fade effect using just white
void setupFade() {
  PixelRingColor startColor(2, 2, 2);
  PixelRingColor endColor(30, 30, 30);
  
  // Apply effect to ring
  pixelRingAnimator->doFade(&startColor, &endColor, 28);
}

// Sets up a fade effect between red and blue
void setupFade2() {
  PixelRingColor startColor(15, 0, 0);
  PixelRingColor endColor(0, 0, 15);
  
  // Apply effect to ring
  pixelRingAnimator->doFade(&startColor, &endColor, 15);
}

// Sets up a spin effect using purple
void setupSpin() {
  PixelRingColor baseColor(8, 0, 8);
  
  // Apply effect to ring
  pixelRingAnimator->doSpin(&baseColor, false);
}

// Sets up a countdown effect with black (off) and green
void setupCountdown() {
  PixelRingColor baseColor(0, 0, 0);
  PixelRingColor countdownColor(0, 8, 0);
  
  // Apply effect to ring
  pixelRingAnimator->doCountdown(&baseColor, &countdownColor, true);
}

// Sets up an alternate effect with red and purple
void setupAlternate() {
  PixelRingColor colorOne(8, 0, 0);
  PixelRingColor colorTwo(8, 0, 8);
  
  // Apply effect to ring
  pixelRingAnimator->doAlternate(&colorOne, &colorTwo);
}

void setup() {
  // Create the animator, clear the ring
  pixelRingAnimator = new PixelRingAnimator();
  pixelRingAnimator->start(PIXEL_RING_PIN, NUM_PIXELS, NEO_PIXEL_TYPE);
  
  // Set up variables for the loop
  effect = -1;
  nextEffectChangeTime = 0;
  nextUpdateTime = 0;
}

void loop() {
  // Time to change the effect?
  if (millis() >= nextEffectChangeTime) {
    
    // Move to the next effect, looping around if needed
    effect = (effect + 1) % 5;
    
    // Setup the effect and apply to ring
    switch(effect) {
      case 0:
        setupFade();
        break;

      case 1:
        setupSpin();
        break;

      case 2:
        setupCountdown();
        break;

      case 3:
        setupFade2();
        break;

      case 4:
        setupAlternate();
        break;
    }

    // Change the effect again in 7 seconds
    nextEffectChangeTime = millis() + 7000;
  }
  
  // Time to update the ring?
  if (millis() >= nextUpdateTime) {
    
    // Update the ring
    pixelRingAnimator->update();
    
    // Update again in 50 milliseconds
    nextUpdateTime = millis() + 50;
  }
}
