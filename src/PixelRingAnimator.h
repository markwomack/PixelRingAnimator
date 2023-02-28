//
// Licensed under Apache 2.0 license.
// See accompanying LICENSE file for details.
//

#ifndef PIXELRINGANIMATOR_H
#define PIXELRINGANIMATOR_H

#include <Adafruit_NeoPixel.h>

//** A data class to represent colors for the PixelRingAnimator.
//*  It manages the data members and converting to/from and testing
//*  values. The 32 bit representation matches that used by the
//*  Adafruit library, WRGB, so that it can be used interchangeably
//*  if needed.
class PixelRingColor {
  public:
    PixelRingColor();
    PixelRingColor(const uint8_t red, const uint8_t green, const uint8_t blue);
    PixelRingColor(const uint8_t red, const uint8_t green, const uint8_t blue, uint8_t brightness);
    PixelRingColor(const uint32_t wRGB32Bit);
    PixelRingColor(PixelRingColor* color);
    
    uint8_t getRed(void);
    uint8_t getGreen(void);
    uint8_t getBlue(void);
    uint8_t getBrightness(void);
    
    void setRed(const uint8_t red);
    void setGreen(const uint8_t green);
    void setBlue(const uint8_t blue);
    void setBrightness(const uint8_t brightness);
    void setPixelRingColor(PixelRingColor* color);
    void setWRGB32Bit(const uint32_t wRGB32Bit);
    
    uint32_t asWRGB32Bit(void);
    
    bool isSameColor(PixelRingColor* color);
    
  private:
    uint8_t _red;
    uint8_t _green;
    uint8_t _blue;
    uint8_t _brightness;
};

//** A class to manage predefined animations for the Adafruit pixel rings.
//*  The start() method should be called before any other methods are used.
//*  Once a pattern is set, the update rate is determined by the caller.
//*  Each call to update() will increment the pixel ring display to the next
//*  step in the pattern. The update() method should be called at the rate
//*  desired, usually from the sketch loop() method.
//*  Besides the data needed to drive the patterns, no data is stored per
//*  pixel. The state of the neopixel is stored with the Adafruit library.
class PixelRingAnimator {
  public:
    PixelRingAnimator();

    //** Initializes and starts the PixelRingAnimator and the pixel ring.
    //*  Must be called before executing any other methods.
    //*  ringPin - The pin the pixel ring data line is connected to.
    //*  numPixels - Number of pixels on the pixel ring.
    //*  pixelType - The type of neopixel used on the ring. See
    //*              Adafruit_NeoPixel.h for options.
    //*  basePixel - The pixel number that will be treated as start pixel
    //*              for patterns where it matters. This might matter if
    //*              the ring can't be positioned with pixel 0 exactly
    //*              where you need it. Defaults to 0.
    void start(const uint8_t ringPin, const uint16_t numPixels, const neoPixelType pixelType, uint16_t basePixel = 0);
    
    //** Stops any animation, sets all of the pixels to off (0, 0, 0).
    void doOff(void);
    
    //** Starts the fade pattern, going between the start and end colors by
    //*  the number of steps. The number of steps should not be more than the
    //*  difference between the RGB values of the start and end colors.
    void doFade(PixelRingColor* startColor, PixelRingColor* endColor, const uint8_t numSteps);
    
    //** Starts the spin pattern. For values of dir, true = clockwise,
    //*  false - counter clockwise.
    void doSpin(PixelRingColor* baseColor, const bool dir);
    
    //** Starts the countdown pattern. For values of dir, true = clockwise,
    //*  false - counter clockwise.
    void doCountdown(PixelRingColor* startColor, PixelRingColor* endColor, const bool dir);
    
    //** Starts the alternate pattern.
    void doAlternate(PixelRingColor* colorOne, PixelRingColor* colorTwo);
    
    //** Updates the pattern to the next step. Call this method from the
    //*  sketch loop() method at the rate you want the pattern to change.
    void update(void);

  private:
    enum Mode {
      OFF,
      FADE,
      SPIN,
      COUNTDOWN,
      ALTERNATE
    };

    // Data to support fade pattern
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

    // Data to support spin pattern
    struct SpinInfo {
      PixelRingColor baseColor;
      bool dir;
      uint16_t index;
    };

    // Data to support countdown pattern
    struct CountdownInfo {
      PixelRingColor baseColor;
      PixelRingColor countdownColor;
      bool dir;
      uint16_t index;
    };

    // Data to support alternate pattern
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

    uint16_t getIndexWithOffset(const uint16_t index, const int offset);
    void renderSolidColor(PixelRingColor* color);
    void renderSinglePixel(const uint16_t pixelNum, PixelRingColor* color);
    void renderFade(void);
    void renderSpin(void);
    void renderCountdown(void);
    void renderAlternate(void);
};
#endif // PIXELRINGANIMATOR_H
