//
// Licensed under Apache 2.0 license.
// See accompanying LICENSE file for details.
//

#ifndef PIXELRINGANIMATOR_H
#define PIXELRINGANIMATOR_H

#include <Adafruit_NeoPixel.h>

struct PixelRingColor {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
};

class PixelRingAnimator {
  public:
    PixelRingAnimator(const uint8_t ringPin, const uint16_t numPixels, const neoPixelType pixelType, uint16_t basePixel = 0);

    void clear(void);
    void doFade(PixelRingColor* startColor, PixelRingColor* endColor, const uint8_t numSteps);
    void doSpin(PixelRingColor* baseColor, const bool dir);
    void doCountdown(PixelRingColor* startColor, PixelRingColor* endColor, const bool dir);
    void doAlternate(PixelRingColor* colorOne, PixelRingColor* colorTwo);
    void update(void);

  private:
    enum Mode {
      OFF,
      FADE,
      SPIN,
      COUNTDOWN,
      ALTERNATE
    };

    struct FadeInfo {
      PixelRingColor startColor;
      PixelRingColor endColor;
      uint8_t numSteps;
      int8_t redSteps;
      int8_t greenSteps;
      int8_t blueSteps;
      uint8_t stepCount;
      bool fadeDir;
    };

    struct SpinInfo {
      PixelRingColor baseColor;
      bool dir;
      uint16_t index;
    };

    struct CountdownInfo {
      PixelRingColor baseColor;
      PixelRingColor countdownColor;
      bool dir;
      uint16_t index;
    };

    struct AlternateInfo {
      PixelRingColor colorOne;
      PixelRingColor colorTwo;
    };
    
    Adafruit_NeoPixel* _neoPixel;
    uint16_t _numPixels;
    uint16_t _basePixel;
    Mode _mode;
    FadeInfo _fadeInfo;
    SpinInfo _spinInfo;
    CountdownInfo _countdownInfo;
    AlternateInfo _alternateInfo;

    PixelRingAnimator() {};
    uint16_t getIndexWithOffset(const uint16_t index, const int offset);
    void getPixelColor(const uint16_t pixelNum, PixelRingColor* returnColor);
    bool sameColors(PixelRingColor* colorOne, PixelRingColor* colorTwo);
    void renderSolidColor(PixelRingColor* color);
    void renderSinglePixel(const uint16_t pixelNum, PixelRingColor* color);
    void renderFade(void);
    void renderSpin(void);
    void renderCountdown(void);
    void renderAlternate(void);

    
};
#endif // PIXELRINGANIMATOR_H
