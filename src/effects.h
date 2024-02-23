#ifndef Effects_H
#define Effects_H

#include <Arduino.h>
#include <functional>

namespace Interpolation {
  byte lerp(float progress, float fromValue, float toValue){
    return (int)(fromValue + (toValue - fromValue) * progress);
  }
}

struct RGBW {
  byte red;
  byte green;
  byte blue;
  byte brightness;

  uint32_t toInt() {
    float bri = (this->brightness / 255.0f);
    byte r = this->red * bri;
    byte g = this->green * bri;
    byte b = this->blue * bri;

    return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
  }

  RGBW static zero() {
    return RGBW { 0, 0, 0, 0 };
  }
};

namespace Effects {
  RGBW fade(float progress, RGBW src, RGBW dst) {
    RGBW out = RGBW();

    out.red = Interpolation::lerp(progress, src.red, dst.red);
    out.green = Interpolation::lerp(progress, src.green, dst.green);
    out.blue = Interpolation::lerp(progress, src.blue, dst.blue);
    out.brightness = Interpolation::lerp(progress, src.brightness, dst.brightness);
    return out;
  }
}

namespace SimpleEffects {
  float fade(float progress, float src, float dst) {
    return Interpolation::lerp(progress, src, dst);
  }
}

#define EFFECT_FUNCTION std::function<RGBW(float, RGBW, RGBW)> processEffect
#define SIMPLE_EFFECT_FUNCTION std::function<float(float, float, float)> processEffect

#endif
