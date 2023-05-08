/*=================================================================================================
 *  LED.cpp - LED class for controlling LEDs
 * 
 * 
 *  Version   Date    Comments
 *  ------- --------  ------------------------------------------------------------------------------
 *  1.0.0   08-27-22  First production version
 * 
 * 
 *==================================================================================================
 */
 
#include "Arduino.h"
#include "LED.h"

// these constants are required for the LED class
#define LED_OFF   0
#define LED_ON    1
#define LED_BLINK 2
#define LED_RAMP  3
#define LED_DIM   4

LED::LED(unsigned pin) {
  _pin = pin;
  _led_on = HIGH;
  _led_off = LOW;
  pinMode(_pin,OUTPUT);
  off();
  _state = LED_OFF;
  _ontime = 0;
  _offtime = 0;
  _startvalue = 0;
  _endvalue = 0;
  _ramptime = 0;
  _cyclestart = 0;
  _dimvalue = 0;
  _blinkon = false;
  _inverted = false;
  //
  // Note that only pins 3, 5, 6, 9, 10, 11 support PWM
  //
  switch (_pin) {
    case 3:
    case 5:
    case 6:
    case 9:
    case 10:
    case 11:
      _dimmable = true;
      break;

    default:
      _dimmable = false;
  }
}

LED::LED(unsigned pin, boolean invert) {
  _pin = pin;
  _led_on = HIGH;
  _led_off = LOW;
  if (invert) {
    _led_on = LOW;
    _led_off = HIGH;
  }
  pinMode(_pin,OUTPUT);
  off();
  _state = LED_OFF;
  _ontime = 0;
  _offtime = 0;
  _startvalue = 0;
  _endvalue = 0;
  _ramptime = 0;
  _cyclestart = 0;
  _dimvalue = 0;
  _blinkon = false;
  _inverted = invert;
  switch (_pin) {
    case 3:
    case 5:
    case 6:
    case 9:
    case 10:
    case 11:
      _dimmable = true;
      break;

    default:
      _dimmable = false;
  }
}

void LED::on() {
  _state = LED_ON;
  digitalWrite(_pin,_led_on);
}

void LED::off() {
  _state = LED_OFF;
  _blinkon = false;
  digitalWrite(_pin,_led_off);
}

void LED::blink(unsigned onTime, unsigned offTime) {
  _state = LED_BLINK;
  _cyclestart = millis();
  digitalWrite(_pin,_led_on);
  _ontime = onTime;
  _offtime = offTime;
  _blinkon = true;
}

void LED::blink(unsigned onTime, unsigned offTime, unsigned delay) {
  _state = LED_BLINK;
  _cyclestart = millis() + offTime - delay;
  digitalWrite(_pin,_led_off);
  _ontime = onTime;
  _offtime = offTime;
  _blinkon = false;
}

void LED::ramp(unsigned startValue, unsigned endValue, unsigned rampTime) {
  if (_dimmable) {
    startValue = min(startValue,255);
    endValue = min(endValue,255);
    
    _state = LED_RAMP;
    _startvalue = startValue;
    _endvalue = endValue;
    _ramptime = rampTime;
    analogWrite(_pin,_inverted ? 255-_startvalue : _startvalue);
    _cyclestart = millis();      
  }
}

void LED::dim(unsigned value) {
  if (_dimmable) {
    _state = LED_DIM;
    _dimvalue = value>255 ? 255 : value;
    analogWrite(_pin,_inverted ? 255-_dimvalue : _dimvalue);
  }
}


/*    refresh MUST be called on every cycle through loop()
 *    for any LED which is blinking or ramping.
 */
void LED::refresh() {
  switch (_state) {
    case LED_OFF:
      digitalWrite(_pin,_led_off);
      break;
      
     case LED_ON:
     digitalWrite(_pin,_led_on);
      break;
      
    case LED_BLINK:
      if (_blinkon) {
        if (millis() > _cyclestart + _ontime) {
          _cyclestart = millis();
          digitalWrite(_pin,_led_off);
          _blinkon = false;
        }
      }
      else {
        if (millis() > _cyclestart + _offtime) {
          _cyclestart = millis();
          digitalWrite(_pin,_led_on);
          _blinkon = true;
        }
      }
      break;
      
    case LED_RAMP:
      float value = (float)(millis() - _cyclestart)/(float)_ramptime * ((float)_endvalue - (float)_startvalue) + (float)_startvalue;
      if (value > 255.0)value = 255.0;
      if (value < 0.0) value = 0.0;
      unsigned val = (unsigned) value;
      analogWrite(_pin,_inverted ? 255-val : val);
      break;

    case LED_DIM:
      analogWrite(_pin, _dimvalue);
      break;
  }
}
