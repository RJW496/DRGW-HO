/*
Grade crossing flasher controller with sound

  Uses pins D8 and D9 to control the Adafruit
  Audio FX Mini sound board. Pin D8 connects to
  the Audio FX control pad (0 through 7). Pin D9
  connects to the Audio FX reset pin.
*/

#define LED_INVERTED true
#define CMN_ANODE true
#define CMN_CATHODE false

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
    unsigned _ontime;
    unsigned _offtime;
    unsigned long _cyclestart;
    boolean  _blinkon;
};

// these constants are required for the LED class
#define LED_OFF   0
#define LED_ON    1
#define LED_BLINK 2

LED::LED(unsigned pin) {
  _pin = pin;
  _led_on = HIGH;
  _led_off = LOW;
  pinMode(_pin,OUTPUT);
  off();
  _state = LED_OFF;
  _ontime = 0;
  _offtime = 0;
  _cyclestart = 0;
  _blinkon = false;
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
  _cyclestart = 0;
  _blinkon = false;
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
  _cyclestart = millis() - offTime + delay;
  digitalWrite(_pin,_led_off);
  _ontime = onTime;
  _offtime = offTime;
  _blinkon = false;
}


/*    refresh MUST be called on every cycle through loop()
 *    for any LED which could be blinking.
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
  }
}


// define flashing lights
LED a1 = LED(2,CMN_ANODE);
LED a2 = LED(3,CMN_ANODE);
LED b1 = LED(4,CMN_ANODE);
LED b2 = LED(5,CMN_ANODE);

// pin assignments
#define pushButton 7
#define sndOnPin 8
#define resetPin 9

bool flashing;

void setup() {
  // put your setup code here, to run once:
  pinMode(pushButton,INPUT_PULLUP);
  pinMode(sndOnPin,OUTPUT);
  pinMode(resetPin,OUTPUT);
  soundOff();
}

void loop() {
  // put your main code here, to run repeatedly:
  a1.refresh();
  a2.refresh();
  b1.refresh();
  b2.refresh();

  // test push button
  if (pushButtonPressed() && !flashing) {
      flashing = true;
      a1.blink(1000,1000,0);
      a2.blink(1000,1000,1000);
      b1.blink(1000,1000,0);
      b2.blink(1000,1000,1000);
      soundOn();
  }
  if (!pushButtonPressed()) {
      a1.off();
      a2.off();
      b1.off();
      b2.off();
      soundOff();
      flashing = false;
  }
}

void soundOff() {
  digitalWrite(sndOnPin,HIGH);
  digitalWrite(resetPin,LOW);
}

void soundOn() {
  digitalWrite(resetPin,HIGH);
  digitalWrite(sndOnPin,LOW);
}

bool pushButtonPressed() {
  return (digitalRead(pushButton)==LOW);
}
