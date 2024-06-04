/*
 *  Listing 5: Classes_5
 *  Copyright (c) 2024 by Richard J. Wissbaum
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

#undef SIGNAL

#define SIG_CLR         1
#define SIG_DIV_APP_MED 2
#define SIG_APP_MED     3
#define SIG_DIV_CLR     4
#define SIG_APP         5
#define SIG_DIV_APP     6
#define SIG_RESTR       7
#define SIG_STOP_PROC   8
#define SIG_STOP        9

class SIGNAL {
  public:
    SIGNAL(unsigned grnPin, unsigned ylwPin, unsigned redPin);
    SIGNAL(unsigned grnPin, unsigned ylwPin, unsigned redPin, boolean invert);
    void setAspect (unsigned aspect);
    void refresh();
  private:
    LED* _grn;
    LED* _ylw;
    LED* _red;
};

SIGNAL::SIGNAL(unsigned grnPin, unsigned ylwPin, unsigned redPin) {
  _grn = new LED(grnPin);
  _ylw = new LED(ylwPin);
  _red = new LED(redPin);
}

SIGNAL::SIGNAL(unsigned grnPin, unsigned ylwPin, unsigned redPin, boolean invert) {
  _grn = new LED(grnPin, invert);
  _ylw = new LED(ylwPin, invert);
  _red = new LED(redPin, invert);
}

void SIGNAL::refresh() {
  _grn->refresh();
  _ylw->refresh();
  _red->refresh();
}

void SIGNAL::setAspect(unsigned aspect) {
  switch (aspect) {
    case SIG_CLR:
    // Clear: Implement as steady green
      _grn->on();
      _ylw->off();
      _red->off();
      break;
    
    case SIG_DIV_APP_MED:
    // Diverging Approach Medium: NOT IMPLEMENTED    
      break;
    
    case SIG_APP_MED:
    // Approach Medium: Implement as blinking yellow
      _grn->off();
      _ylw->blink(500,500);
      _red->off();
      break;
    
    case SIG_DIV_CLR:
    // Diverging Clear: NOT IMPLEMENTED
      break;
    
    case SIG_APP:
    // Approach: Implement as steady yellow
      _grn->off();
      _ylw->on();
      _red->off();
      break;
      
    case SIG_DIV_APP:
    // Diverging Approach: NOT IMPLEMENTED
      break;
      
    case SIG_RESTR:
    // Restricting: NOT IMPLEMENTED
      break;
      
    case SIG_STOP_PROC:
    // Stop and Proceed: NOT IMPLEMENTED
      break;
      
    case SIG_STOP:
    // Stop: Implemented as steady red
      _grn->off();
      _ylw->off();
      _red->on();
      break;
    
  }
}

SIGNAL s1 = SIGNAL(3,4,5,CMN_ANODE);
SIGNAL* s2;

void setup() {
  s1.setAspect(SIG_STOP);
  s2 = new SIGNAL(6,7,8,CMN_ANODE);
  s2->setAspect(SIG_APP_MED);
}

void loop() {
  s1.refresh();
  s2->refresh();
}
