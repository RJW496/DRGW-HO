/*
 *  Speedometer version 1
 */

 //========== Customization ==================================

#define O    48
#define S    64
#define HO   87.1
#define OO   96
#define TT   120
#define N-UK 148
#define N-US 160
#define Z    220

float scale = HO;             // change HO to your scale

#define MPH 1.0
#define KPH 1.609344

float speedUnits = MPH;       // select MPH or KPH

#define INCHES  1
#define CM      2

float distance = 6;           // enter distance between sensors
int distUnits = INCHES;       // select INCHES or CM

//=========== End Customization ===============================

#define ST_READY      1
#define ST_DETECT_LB  2
#define ST_DETECT_RB  3
#define ST_CALC       4
#define ST_WAITING    5

int state;

const int leftLED = A0;
const int rghtLED = A1;

unsigned long timerStart;
unsigned long elapsedTime;

float constant;

void setup() {
  Serial.begin(9600);
  pinMode(leftLED,INPUT);
  pinMode(rghtLED,INPUT);
  state = ST_READY;
  Serial.println("Ready...");

  // calculate constant for speed calculation
  if (distUnits==CM) distance = distance / 2.54;
  constant = distance * scale / 12. / 5280. * speedUnits;
}

void loop() {
  switch (state) {
    case ST_READY:
      if (detected(leftLED)) {
        timerStart = millis();
        state = ST_DETECT_RB;
        Serial.println("Detecting...");
      }
      else if (detected(rghtLED)) {
        timerStart = millis();
        state = ST_DETECT_LB;
        Serial.println("Detecting...");
      }
      break;

    case ST_DETECT_LB:
      if (detected(rghtLED)) {
        elapsedTime = millis() - timerStart;
        state = ST_CALC;
      }
      break;

    case ST_DETECT_RB:
      if (detected(leftLED)) {
        elapsedTime = millis() - timerStart;
        state = ST_CALC;
      }
      break;

    case ST_CALC:
      Serial.println("Calculating...");
      float speed = constant / (elapsedTime / 1000. / 3600.);
      Serial.print("Scale speed = ");
      Serial.print(speed,1);
      if (speedUnits==MPH)
        Serial.println(" MPH");
      else
        Serial.println(" KPH");
      state = ST_WAITING;
      break;

    case ST_WAITING:
      if (!detected(leftLED) && !detected(rghtLED)) {
        state = ST_READY;
        Serial.println("Ready...");
      }
  }
}

boolean detected(int pin) {
  if (analogRead(pin)>500) return true; else return false;
}
