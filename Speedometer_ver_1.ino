/*
 *  Speedometer version 1
 */

 //========== Customization ==================================

#define O    48
#define S    64
#define HO   87.1
#define OO   96
#define TT   120
#define N_UK 148
#define N_US 160
#define Z    220
float scale = HO;             // change HO to your scale

#define MPH 1.0
#define KPH 1.609344
float speedUnits = MPH;       // select MPH or KPH

#define INCHES  1
#define CM      2
float distance = 8.0;         // enter distance between sensors
int distUnits = INCHES;       // select INCHES or CM

//=========== End Customization ===============================

#define ST_READY      1
#define ST_DETECT_LB  2
#define ST_DETECT_RB  3
#define ST_CALC       4
#define ST_WAITING    5

int state;

const unsigned leftLED = A0;
const unsigned rghtLED = A1;

unsigned long timerStart;
unsigned long elapsedTime;

float constant;

void setup() {
  Serial.begin(9600);
  pinMode(rghtLED,INPUT);
  pinMode(leftLED,INPUT);
  state = ST_READY;
  display("Ready...");

  // calculate constant for speed calculation
  if (distUnits==CM) distance = distance / 2.54;
  constant = distance * scale / 12. / 5280. * speedUnits;
}

void loop() {
int newState=state;

  switch (state) {
    case ST_READY:
      if (detected(leftLED)) {
        timerStart = millis();
        newState = ST_DETECT_RB;
        display("Detecting...");
      }
      else if (detected(rghtLED)) {
        timerStart = millis();
        newState = ST_DETECT_LB;
        display("Detecting...");
      }
      break;

    case ST_DETECT_LB:
      if (detected(leftLED)) {
        elapsedTime = millis() - timerStart;
        newState = ST_CALC;
      }
      break;

    case ST_DETECT_RB:
      if (detected(rghtLED)) {
        elapsedTime = millis() - timerStart;
        newState = ST_CALC;
      }
      break;

    case ST_CALC:
      {
      display("Calculating...");
      float speed = constant / (elapsedTime / 1000. / 3600.);
      display(speed,speedUnits);
      newState = ST_WAITING;
      display("Waiting...");
      timerStart = millis();
      break;
      }
  
    case ST_WAITING:
      // force delay of 2 seconds
      if (millis()-timerStart<2000) return;

      if (!detected(leftLED) && !detected(rghtLED)) {
        newState = ST_READY;
        display("Ready...");
      }
      break;
}
  state = newState;
}

boolean detected(unsigned pin) {
  return (analogRead(pin)<500);
}

void display(const char* text) {
  Serial.println(text);
}

void display(float speed, float units) {
  Serial.print("Speed: ");
  Serial.print(speed,1);
  if (units==MPH)
    Serial.println(" MPH");
  else
    Serial.println(" KPH");
}