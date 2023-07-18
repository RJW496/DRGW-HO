/*
 *  Listing 3: Classes_3
 *  Copyright (c) 2023 by Richard J. Wissbaum
 *  All rights reserved
 *  
 *  Permission is granted to private, non-commerical use only.
 */
 
#define CMN_CATHODE false
#define CMN_ANODE true

/*
 *  LED class Version 0.2
 */
class LED {
  public:
    LED(unsigned pin);
    LED(unsigned pin, boolean invert);
    void on();
    void off();
  private:
    unsigned _pin;  
    unsigned _led_on;
    unsigned _led_off;
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
  digitalWrite(_pin,_led_on);
}

void LED::off() {
  digitalWrite(_pin,_led_off);
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
}

void loop() {
  // start with red LED on
  red1.on();
  red2.on();
  delay(RED_ON);

  // turn red LED off, green LED on
  red1.off();
  red2.off();
  grn1.on();
  grn2.on();
  delay(GRN_ON);

  // turn green LED off, yellow LED on
  grn1.off();
  grn2.off();
  ylw1.on();
  ylw2.on();
  delay(YLW_ON);

  // turn yellow LED off
  ylw1.off();
  ylw2.off();
}
