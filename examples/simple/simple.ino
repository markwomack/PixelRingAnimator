//
// Licensed under Apache 2.0 license.
// See accompanying LICENSE file for details.
//

#include <Arduino.h>

#include <PixelRingAnimator.h>

//*** Change these values to match your setup
const uint8_t PIXEL_RING_PIN(39);                         // pixel ring data pin
const uint16_t NUM_PIXELS(16);                            // the number of pixels on ring
const neoPixelType NEO_PIXEL_TYPE(NEO_GRBW + NEO_KHZ800); // the type of neo pixels on ring

PixelRingAnimator* pixelRingAnimator;

uint32_t nextUpdateTime;
uint32_t nextPatternChangeTime;
int pattern;

void setupFade() {
  PixelRingColor startColor;
  startColor.red = 2;
  startColor.green = 2;
  startColor.blue = 2;
  PixelRingColor endColor;
  endColor.red = 30;
  endColor.green = 30;
  endColor.blue = 30;
  pixelRingAnimator->doFade(&startColor, &endColor, 28);
}

void setupSpin() {
  PixelRingColor baseColor;
  baseColor.red = 8;
  baseColor.green = 0;
  baseColor.blue = 8;
  pixelRingAnimator->doSpin(&baseColor, false);
}

void setupCountdown() {
  PixelRingColor baseColor;
  baseColor.red = 0;
  baseColor.green = 0;
  baseColor.blue = 0;
  PixelRingColor countdownColor;
  countdownColor.red = 0;
  countdownColor.green = 8;
  countdownColor.blue = 0;
  pixelRingAnimator->doCountdown(&baseColor, &countdownColor, true);
}

void setupAlternate() {
  PixelRingColor colorOne;
  colorOne.red = 8;
  colorOne.green = 0;
  colorOne.blue = 0;
  PixelRingColor colorTwo;
  colorTwo.red = 8;
  colorTwo.green = 0;
  colorTwo.blue = 8;
  pixelRingAnimator->doAlternate(&colorOne, &colorTwo);
}

void setup() {
  pixelRingAnimator = new PixelRingAnimator(PIXEL_RING_PIN, NUM_PIXELS, NEO_PIXEL_TYPE);
  pixelRingAnimator->clear();

  pattern = -1;
  nextPatternChangeTime = 0;
  nextUpdateTime = 0;
}

void loop() {
  if (millis() >= nextPatternChangeTime) {
    pattern = (pattern + 1) % 4;
    switch(pattern) {
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
        setupAlternate();
        break;
    }

    nextPatternChangeTime = millis() + 7000;
  }
  
  if (millis() >= nextUpdateTime) {
    pixelRingAnimator->update();
    nextUpdateTime = millis() + 50;
  }
}
