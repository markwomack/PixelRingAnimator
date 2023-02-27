//
// Licensed under Apache 2.0 license.
// See accompanying LICENSE file for details.
//

#include "PixelRingAnimator.h"

PixelRingAnimator::PixelRingAnimator(const uint8_t ringPin, const uint16_t numPixels, const neoPixelType pixelType, const uint16_t basePixel) {
  _neoPixel = new Adafruit_NeoPixel(_numPixels, ringPin, pixelType);
  _neoPixel->begin();
  _numPixels = numPixels;
  _basePixel = basePixel;
  _mode = OFF;
}

void PixelRingAnimator::clear() {
  _neoPixel->clear();
  _neoPixel->show();
}

void PixelRingAnimator::doFade(PixelRingColor* startColor, PixelRingColor* endColor, uint8_t numSteps) {
  _mode = FADE;
  _fadeInfo.startColor = *startColor;
  _fadeInfo.endColor = *endColor;
  _fadeInfo.numSteps = numSteps;
  _fadeInfo.redSteps = ((int)(_fadeInfo.endColor.red - _fadeInfo.startColor.red))/numSteps;
  _fadeInfo.greenSteps = ((int)(_fadeInfo.endColor.green - _fadeInfo.startColor.green))/numSteps;
  _fadeInfo.blueSteps = ((int)(_fadeInfo.endColor.blue - _fadeInfo.startColor.blue))/numSteps;
  _fadeInfo.stepCount = 0;
  _fadeInfo.fadeDir = true;
  renderSolidColor(&_fadeInfo.startColor);
  _neoPixel->show();
}

void PixelRingAnimator::doSpin(PixelRingColor* baseColor, const bool dir) {
  _mode = SPIN;
  _spinInfo.baseColor = *baseColor;
  _spinInfo.dir = dir;
  _spinInfo.index = getIndexWithOffset(_basePixel, dir ? 1 : -1);
  renderSolidColor(&_spinInfo.baseColor);
  _neoPixel->show();
}

void PixelRingAnimator::doCountdown(PixelRingColor* baseColor, PixelRingColor* countdownColor, const bool dir) {
  _mode = COUNTDOWN;
  _countdownInfo.baseColor = *baseColor;
  _countdownInfo.countdownColor = *countdownColor;
  _countdownInfo.dir = dir;
  _countdownInfo.index = getIndexWithOffset(_basePixel, dir ? 1 : -1);
  renderSolidColor(&_countdownInfo.baseColor);
  _neoPixel->show();
}

void PixelRingAnimator::doAlternate(PixelRingColor* colorOne, PixelRingColor* colorTwo) {
  _mode = ALTERNATE;
  _alternateInfo.colorOne = *colorOne;
  _alternateInfo.colorTwo = *colorTwo;
  renderAlternate();
  _neoPixel->show();
}

void PixelRingAnimator::update(void) {
  switch(_mode) {
    case OFF:
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
  if (index == 0 && offset == -1) {
    newValue = 15;
  } else
    newValue = (index + offset) % _numPixels;
  return newValue;
}

void PixelRingAnimator::getPixelColor(const uint16_t pixelNum, PixelRingColor* returnColor) {
  uint32_t pixelColor = _neoPixel->getPixelColor(pixelNum);
  returnColor->red = pixelColor >> 16 & 0x000000FF;
  returnColor->green = pixelColor >> 8 & 0x000000FF;
  returnColor->blue = pixelColor & 0x000000FF;
}

bool PixelRingAnimator::sameColors(PixelRingColor* colorOne, PixelRingColor* colorTwo) {
  return (colorOne->red == colorTwo->red && colorOne->green == colorTwo->green && colorOne->blue == colorTwo->blue);
}

void PixelRingAnimator::renderSolidColor(PixelRingColor* color) {
  for (uint16_t x = 0; x < _numPixels; x++) {
    _neoPixel->setPixelColor(x, _neoPixel->Color(color->red, color->green, color->blue));
  }
}

void PixelRingAnimator::renderSinglePixel(const uint16_t pixelNum, PixelRingColor* color) {
  _neoPixel->setPixelColor(pixelNum, _neoPixel->Color(color->red, color->green, color->blue));
}

void PixelRingAnimator::renderFade(void) {
  _fadeInfo.stepCount++;
  if (_fadeInfo.stepCount >= _fadeInfo.numSteps) {
    _fadeInfo.fadeDir = !_fadeInfo.fadeDir;
    _fadeInfo.stepCount = 0;
  }

  PixelRingColor newColor = _fadeInfo.fadeDir ? _fadeInfo.startColor : _fadeInfo.endColor;
  newColor.red += _fadeInfo.fadeDir ? (_fadeInfo.stepCount * _fadeInfo.redSteps) :  -(_fadeInfo.stepCount * _fadeInfo.redSteps);
  newColor.green += _fadeInfo.fadeDir ? (_fadeInfo.stepCount * _fadeInfo.greenSteps) :  -(_fadeInfo.stepCount * _fadeInfo.greenSteps);
  newColor.blue += _fadeInfo.fadeDir ? (_fadeInfo.stepCount * _fadeInfo.blueSteps) :  -(_fadeInfo.stepCount * _fadeInfo.blueSteps);
    
  renderSolidColor(&newColor);
}

void PixelRingAnimator::renderSpin(void) {
  _spinInfo.index = getIndexWithOffset(_spinInfo.index, _spinInfo.dir ? -1 : 1);
  for (int offset = -1; offset < 3; offset++) {
    int index = getIndexWithOffset(_spinInfo.index, offset);
    PixelRingColor newColor;

    if ((offset == -1 && !_spinInfo.dir) || (offset == 2 && _spinInfo.dir)) {
      newColor = _spinInfo.baseColor;
    } else if ((offset == 0 && !_spinInfo.dir) || (offset == 1 && _spinInfo.dir)) {
      newColor.red = (_spinInfo.baseColor.red != 0) ? _spinInfo.baseColor.red + (255 -_spinInfo.baseColor.red)/4 : 0;
      newColor.green = (_spinInfo.baseColor.green != 0) ? _spinInfo.baseColor.green + (255 -_spinInfo.baseColor.green)/4 : 0;
      newColor.blue = (_spinInfo.baseColor.blue != 0) ? _spinInfo.baseColor.blue + (255 -_spinInfo.baseColor.blue)/4 : 0;
    } else if ((offset == 1 && !_spinInfo.dir) || (offset == 0 && _spinInfo.dir)) {
      newColor.red = (_spinInfo.baseColor.red != 0) ? _spinInfo.baseColor.red + (255 -_spinInfo.baseColor.red)/2 : 0;
      newColor.green = (_spinInfo.baseColor.green != 0) ? _spinInfo.baseColor.green + (255 -_spinInfo.baseColor.green)/2 : 0;
      newColor.blue = (_spinInfo.baseColor.blue != 0) ? _spinInfo.baseColor.blue + (255 -_spinInfo.baseColor.blue)/2 : 0;
    } else {
      newColor.red = (_spinInfo.baseColor.red != 0) ? 255 : 0;
      newColor.green = (_spinInfo.baseColor.green != 0) ? 255 : 0;
      newColor.blue = (_spinInfo.baseColor.blue != 0) ? 255 : 0;
    }
    renderSinglePixel(index, &newColor);
  }
  _neoPixel->show();
}

void PixelRingAnimator::renderCountdown(void) {
  _countdownInfo.index = getIndexWithOffset(_countdownInfo.index, _countdownInfo.dir ? -1 : 1);
  PixelRingColor currentColor;
  getPixelColor(_countdownInfo.index, &currentColor);
  if (sameColors(&currentColor, &_countdownInfo.baseColor)) {
    renderSinglePixel(_countdownInfo.index, &_countdownInfo.countdownColor);
  } else {
    renderSinglePixel(_countdownInfo.index, &_countdownInfo.baseColor);
  }
}

void PixelRingAnimator::renderAlternate(void) {
  PixelRingColor* colorOne;
  PixelRingColor* colorTwo;

  PixelRingColor currentColor;
  getPixelColor(0, &currentColor);
  if (!sameColors(&currentColor, &_alternateInfo.colorOne)) {
    colorOne = &_alternateInfo.colorOne;
    colorTwo = &_alternateInfo.colorTwo;
  } else {
    colorOne = &_alternateInfo.colorTwo;
    colorTwo = &_alternateInfo.colorOne;
  }

  for (uint16_t x = 0; x < _numPixels; x++) {
    renderSinglePixel(x, x % 2 == 0 ? colorOne : colorTwo);
  }
}
