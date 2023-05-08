#ifndef LED_h
#define LED_h

#include "Arduino.h"

#define LED_INVERTED true

class LED {
  public:
    LED(unsigned pin);
    LED(unsigned pin, boolean invert);
    void on();
    void off();
    void blink(unsigned onTime, unsigned offTime);
    void blink(unsigned onTime, unsigned offTime, unsigned delay);
    void dim(unsigned value);
    void ramp(unsigned startValue, unsigned endValue, unsigned rampTime);
    void refresh();
  private:
    unsigned _pin;
    unsigned _led_on;
    unsigned _led_off;
    unsigned _state;
    unsigned _ontime;
    unsigned _offtime;
    unsigned _startvalue;
    unsigned _endvalue;
    unsigned _ramptime;
    unsigned _dimvalue;
    unsigned long _cyclestart;
    boolean  _blinkon;
    boolean  _dimmable;
    boolean  _inverted;
};

#endif
