/*
 *  Listing 2: Classes_2
 *  Copyright (c) 2023 by Richard J. Wissbaum
 *  All rights reserved
 *  
 *  Permission is granted to private, non-commerical use only.
 */

 /*
  *   LED class Version 0.1
  */
class LED {
  public:
    LED(unsigned pin);
    void on();
    void off();
  private:
    unsigned _pin;  
};

LED::LED(unsigned pin) {
  _pin = pin;
  pinMode(_pin,OUTPUT);
  off();
}

void LED::on() {
  digitalWrite(_pin,HIGH);
}

void LED::off() {
  digitalWrite(_pin,LOW);
}

#define RED_ON 18000
#define YLW_ON 3000
const unsigned GRN_ON = RED_ON - YLW_ON;

LED red1 = LED(3);
LED ylw1 = LED(4);
LED grn1 = LED(5);

void setup() {
}

void loop() {
  // start with red LED on
  red1.on();
  delay(RED_ON);

  // turn red LED off, green LED on
  red1.off();
  grn1.on();
  delay(GRN_ON);

  // turn green LED off, yellow LED on
  grn1.off();
  ylw1.on();
  delay(YLW_ON);

  // turn yellow LED off
  ylw1.off();
}
