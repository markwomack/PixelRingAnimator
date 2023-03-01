//
// Licensed under Apache 2.0 license.
// See accompanying LICENSE file for details.
//

#include "PixelRingAnimator.h"


PixelRingColor::PixelRingColor() {
  _red = 0;
  _green = 0;
  _blue = 0;
  _brightness = 0;
}

PixelRingColor::PixelRingColor(const uint8_t red, const uint8_t green, const uint8_t blue) {
  _red = red;
  _green = green;
  _blue = blue;
  _brightness = 0;
}

PixelRingColor::PixelRingColor(const uint8_t red, const uint8_t green, const uint8_t blue, uint8_t brightness) {
  _red = red;
  _green = green;
  _blue = blue;
  _brightness = brightness;
}

PixelRingColor::PixelRingColor(const uint32_t wRGB32Bit) {
  _brightness = (wRGB32Bit >> 24) & 0x000000FF;
  _red = (wRGB32Bit >> 16) & 0x000000FF;
  _green = (wRGB32Bit >> 8) & 0x000000FF;
  _blue = wRGB32Bit & 0x000000FF;
}

PixelRingColor::PixelRingColor(PixelRingColor* color) {
  _red = color->_red;
  _green = color->_green;
  _blue = color->_blue;
  _brightness = color->_brightness;
}

uint8_t PixelRingColor::getRed(void) {
  return _red;
}

uint8_t PixelRingColor::getGreen(void) {
  return _green;
}

uint8_t PixelRingColor::getBlue(void) {
  return _blue;
}

uint8_t PixelRingColor::getBrightness(void) {
  return _brightness;
}

void PixelRingColor::setRed(const uint8_t red) {
  _red = red;
}

void PixelRingColor::setGreen(const uint8_t green) {
  _green = green;
}

void PixelRingColor::setBlue(const uint8_t blue) {
  _blue = blue;
}

void PixelRingColor::setBrightness(const uint8_t brightness) {
  _brightness = brightness;
}

void PixelRingColor::setPixelRingColor(PixelRingColor* color) {
  _red = color->_red;
  _green = color->_green;
  _blue = color->_blue;
  _brightness = color->_brightness;
}

void PixelRingColor::setWRGB32Bit(const uint32_t wRGB32Bit) {
  _brightness = (wRGB32Bit >> 24) & 0x000000FF;
  _red = (wRGB32Bit >> 16) & 0x000000FF;
  _green = (wRGB32Bit >> 8) & 0x000000FF;
  _blue = wRGB32Bit & 0x000000FF;
}

uint32_t PixelRingColor::asWRGB32Bit(void) {
  return ((uint32_t)_brightness << 24) | ((uint32_t)_red << 16) | ((uint32_t)_green << 8) | ((uint32_t)_blue);
}

bool PixelRingColor::isSameColor(PixelRingColor* color) {
  return (_red == color->_red && _green == color->_green && _blue == color->_blue && _brightness == color->_brightness);
}

PixelRingAnimator::PixelRingAnimator() {
  _neoPixel = 0;
}

void PixelRingAnimator::start(const uint8_t ringPin, const uint16_t numPixels, const neoPixelType pixelType, const uint16_t basePixel) {
  if (!_neoPixel) {
    _numPixels = numPixels;
    _basePixel = basePixel;
    _mode = OFF;
    _neoPixel = new Adafruit_NeoPixel(_numPixels, ringPin, pixelType);
    _neoPixel->begin();
    _neoPixel->clear();
  }
}

void PixelRingAnimator::doOff() {
  _mode = OFF;
  _neoPixel->clear();
  _neoPixel->show();
}

void PixelRingAnimator::doFade(PixelRingColor* startColor, PixelRingColor* endColor, uint8_t numSteps) {
  _mode = FADE;
  _fadeInfo.startColor.setPixelRingColor(startColor);
  _fadeInfo.endColor.setPixelRingColor(endColor);
  _fadeInfo.numSteps = numSteps;
  _fadeInfo.redSteps = ((int)(_fadeInfo.endColor.getRed() - _fadeInfo.startColor.getRed()))/numSteps;
  _fadeInfo.greenSteps = ((int)(_fadeInfo.endColor.getGreen() - _fadeInfo.startColor.getGreen()))/numSteps;
  _fadeInfo.blueSteps = ((int)(_fadeInfo.endColor.getBlue() - _fadeInfo.startColor.getBlue()))/numSteps;
  _fadeInfo.stepCount = 0;
  _fadeInfo.fadeDir = true;
  renderSolidColor(&_fadeInfo.startColor);
  _neoPixel->show();
}

void PixelRingAnimator::doSpin(PixelRingColor* baseColor, const bool dir) {
  _mode = SPIN;
  _spinInfo.baseColor.setPixelRingColor(baseColor);
  _spinInfo.dir = dir;
  _spinInfo.index = getIndexWithOffset(_basePixel, dir ? 1 : -1);
  renderSolidColor(&_spinInfo.baseColor);
  _neoPixel->show();
}

void PixelRingAnimator::doCountdown(PixelRingColor* baseColor, PixelRingColor* countdownColor, const bool dir) {
  _mode = COUNTDOWN;
  _countdownInfo.baseColor.setPixelRingColor(baseColor);
  _countdownInfo.countdownColor.setPixelRingColor(countdownColor);
  _countdownInfo.dir = dir;
  _countdownInfo.index = getIndexWithOffset(_basePixel, dir ? 0 : -1);
  renderSolidColor(&_countdownInfo.baseColor);
  _neoPixel->show();
}

void PixelRingAnimator::doAlternate(PixelRingColor* colorOne, PixelRingColor* colorTwo) {
  _mode = ALTERNATE;
  _alternateInfo.colorOne.setPixelRingColor(colorOne);
  _alternateInfo.colorTwo.setPixelRingColor(colorTwo);
  renderAlternate();
  _neoPixel->show();
}

void PixelRingAnimator::update(void) {
  switch(_mode) {
    case OFF:
      // do nothing
      break;

    case FADE:
      renderFade();
      break;

    case SPIN:
      renderSpin();
      break;

    case COUNTDOWN:
      renderCountdown();
      break;
 
    case ALTERNATE:
      renderAlternate();
      break;
  }
  _neoPixel->show();
}

uint16_t PixelRingAnimator::getIndexWithOffset(const uint16_t index, const int offset) {
  int newValue;
  // This will only work when the negative offset is less than the number of pixels
  // But not really expecting big offsets here
  if (index == 0 && offset < 0) {
    newValue = 15 + (offset + 1);
  } else {
    newValue = (index + offset) % _numPixels;
  }
  return newValue;
}

void PixelRingAnimator::renderSolidColor(PixelRingColor* color) {
  for (uint16_t x = 0; x < _numPixels; x++) {
    _neoPixel->setPixelColor(x, color->asWRGB32Bit());
  }
}

void PixelRingAnimator::renderSinglePixel(const uint16_t pixelNum, PixelRingColor* color) {
  _neoPixel->setPixelColor(pixelNum, color->asWRGB32Bit());
}

void PixelRingAnimator::renderFade(void) {
  _fadeInfo.stepCount++;
  if (_fadeInfo.stepCount >= _fadeInfo.numSteps) {
    _fadeInfo.fadeDir = !_fadeInfo.fadeDir;
    _fadeInfo.stepCount = 0;
  }

  PixelRingColor newColor(_fadeInfo.fadeDir ? &_fadeInfo.startColor : &_fadeInfo.endColor);
  newColor.setRed(newColor.getRed() + _fadeInfo.stepCount * (_fadeInfo.fadeDir ? _fadeInfo.redSteps :  -_fadeInfo.redSteps));
  newColor.setGreen(newColor.getGreen() + _fadeInfo.stepCount * (_fadeInfo.fadeDir ? _fadeInfo.greenSteps :  -_fadeInfo.greenSteps));
  newColor.setBlue(newColor.getBlue() + _fadeInfo.stepCount * (_fadeInfo.fadeDir ? _fadeInfo.blueSteps :  -_fadeInfo.blueSteps));
    
  renderSolidColor(&newColor);
}

void PixelRingAnimator::renderSpin(void) {
  _spinInfo.index = getIndexWithOffset(_spinInfo.index, _spinInfo.dir ? -1 : 1);
  for (int offset = -1; offset < 3; offset++) {
    int index = getIndexWithOffset(_spinInfo.index, offset);
    PixelRingColor newColor;

    if ((offset == -1 && !_spinInfo.dir) || (offset == 2 && _spinInfo.dir)) {
      newColor.setPixelRingColor(&_spinInfo.baseColor);
    } else if ((offset == 0 && !_spinInfo.dir) || (offset == 1 && _spinInfo.dir)) {
      newColor.setRed((_spinInfo.baseColor.getRed() != 0) ? _spinInfo.baseColor.getRed() + (255 -_spinInfo.baseColor.getRed())/4 : 0);
      newColor.setGreen((_spinInfo.baseColor.getGreen() != 0) ? _spinInfo.baseColor.getGreen() + (255 -_spinInfo.baseColor.getGreen())/4 : 0);
      newColor.setBlue((_spinInfo.baseColor.getBlue() != 0) ? _spinInfo.baseColor.getBlue() + (255 -_spinInfo.baseColor.getBlue())/4 : 0);
    } else if ((offset == 1 && !_spinInfo.dir) || (offset == 0 && _spinInfo.dir)) {
      newColor.setRed((_spinInfo.baseColor.getRed() != 0) ? _spinInfo.baseColor.getRed() + (255 -_spinInfo.baseColor.getRed())/2 : 0);
      newColor.setGreen((_spinInfo.baseColor.getGreen() != 0) ? _spinInfo.baseColor.getGreen() + (255 -_spinInfo.baseColor.getGreen())/2 : 0);
      newColor.setBlue((_spinInfo.baseColor.getBlue() != 0) ? _spinInfo.baseColor.getBlue() + (255 -_spinInfo.baseColor.getBlue())/2 : 0);
    } else {
      newColor.setRed((_spinInfo.baseColor.getRed() != 0) ? 255 : 0);
      newColor.setGreen((_spinInfo.baseColor.getGreen() != 0) ? 255 : 0);
      newColor.setBlue((_spinInfo.baseColor.getBlue() != 0) ? 255 : 0);
    }
    renderSinglePixel(index, &newColor);
  }
  _neoPixel->show();
}

void PixelRingAnimator::renderCountdown(void) {
  _countdownInfo.index = getIndexWithOffset(_countdownInfo.index, _countdownInfo.dir ? -1 : 1);
  PixelRingColor currentColor(_neoPixel->getPixelColor(_countdownInfo.index));
  if (currentColor.isSameColor(&_countdownInfo.baseColor)) {
    renderSinglePixel(_countdownInfo.index, &_countdownInfo.countdownColor);
  } else {
    renderSinglePixel(_countdownInfo.index, &_countdownInfo.baseColor);
  }
}

void PixelRingAnimator::renderAlternate(void) {
  PixelRingColor colorOne;
  PixelRingColor colorTwo;

  PixelRingColor currentColor(_neoPixel->getPixelColor(0));
  if (!currentColor.isSameColor(&_alternateInfo.colorOne)) {
    colorOne.setPixelRingColor(&_alternateInfo.colorOne);
    colorTwo.setPixelRingColor(&_alternateInfo.colorTwo);
  } else {
    colorOne.setPixelRingColor(&_alternateInfo.colorTwo);
    colorTwo.setPixelRingColor(&_alternateInfo.colorOne);
  }

  for (uint16_t x = 0; x < _numPixels; x++) {
    renderSinglePixel(x, x % 2 == 0 ? &colorOne : &colorTwo);
  }
}
