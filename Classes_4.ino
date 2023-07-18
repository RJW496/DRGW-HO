/*
 *  Listing 4: Classes_4
 *  Copyright (c) 2023 by Richard J. Wissbaum
 *  All rights reserved
 *  
 *  Permission is granted to private, non-commerical use only.
 */
 
#define CMN_CATHODE false
#define CMN_ANODE true

#define LED_ON    1
#define LED_OFF   2
#define LED_BLINK 3

/*
 *  LED Class Version 0.3
 */
class LED {
  public:
    LED(unsigned pin);
    LED(unsigned pin, boolean invert);
    void on();
    void off();
    void blink(unsigned onTime, unsigned offTime);
    void blink(unsigned onTime, unsigned offTime, unsigned delay);
    void refresh();
  private:
    unsigned _pin;  
    unsigned _led_on;
    unsigned _led_off;
    unsigned _state;
    unsigned _onTime;
    unsigned _offTime;
    boolean  _blinkOn;
    unsigned long _cycleStart;
};

LED::LED(unsigned pin) {
  _pin = pin;
  _led_on = HIGH;
  _led_off = LOW;
  pinMode(_pin,OUTPUT);
  off();
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
}

void LED::on() {
  _state = LED_ON;
  refresh();
}

void LED::off() {
  _state = LED_OFF;
  refresh();
}

void LED::blink(unsigned onTime, unsigned offTime) {
  _state = LED_BLINK;
  _onTime = onTime;
  _offTime = offTime;
  _cycleStart = millis();
  _blinkOn = true;
  refresh();
}

void LED::blink(unsigned onTime, unsigned offTime, unsigned delay) {
  _state = LED_BLINK;
  _onTime = onTime;
  _offTime = offTime;
  _cycleStart = millis() - _offTime + delay;
  _blinkOn = true;
  if (delay>0) _blinkOn = false;
  refresh();
}

void LED::refresh() {
  switch (_state) {
    case LED_ON:
      digitalWrite(_pin,_led_on);
      break;

    case LED_OFF:
      digitalWrite(_pin,_led_off);
      break;

    case LED_BLINK:
      if (_blinkOn) {
        digitalWrite(_pin,_led_on);
        if (millis() > _cycleStart + _onTime) {
          _cycleStart = millis();
          digitalWrite(_pin,_led_off);
          _blinkOn = false;
        }
      }
      else {
        digitalWrite(_pin,_led_off);
        if (millis() > _cycleStart + _offTime) {
          _cycleStart = millis();
          digitalWrite(_pin,_led_on);
          _blinkOn = true;
        }
      }
  }
}

#define RED_ON 18000
#define YLW_ON 3000
const unsigned GRN_ON = RED_ON - YLW_ON;

LED red1 = LED(3,CMN_CATHODE);
LED ylw1 = LED(4,CMN_CATHODE);
LED grn1 = LED(5,CMN_CATHODE);

LED red2 = LED(6,CMN_ANODE);
LED ylw2 = LED(7,CMN_ANODE);
LED grn2 = LED(8,CMN_ANODE);

void setup() {
  red1.blink(RED_ON, GRN_ON+YLW_ON);
  ylw1.blink(YLW_ON, GRN_ON+RED_ON, RED_ON+GRN_ON);
  grn1.blink(GRN_ON, RED_ON+YLW_ON, RED_ON);

  red2.blink(RED_ON, GRN_ON+YLW_ON, GRN_ON+YLW_ON);
  ylw2.blink(YLW_ON, GRN_ON+RED_ON, GRN_ON);
  grn2.blink(GRN_ON, RED_ON+YLW_ON);
}

void loop() {
  red1.refresh();
  red2.refresh();
  grn1.refresh();
  grn2.refresh();
  ylw1.refresh();
  ylw2.refresh();
}
