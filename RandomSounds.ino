/*---------------------------------------------------------------------
    Audio FX Sound Board Controller version 1.0
    
    This sketch will play up to ten sound clips, either in sequence or
    in random order. The sound clips must be named T00.wav (or T00.ogg)
    through T09.wav (or T00.ogg). You MAY mix wav files with ogg files
    but be aware that wav files might be 10 to 20 times the size of the 
    comparable ogg file.

    This sketch is copyrighted (c) 2026 by Richard Wissbaum
    Permission is granted for all private, non-commercial use

    e-mail rjw31415926@gmail.com with questions or comments
-----------------------------------------------------------------------*/

//=== Required user input here
//  Choose which audio board you are using
//  The Audio FX board will store and play up to ten files
//  The Audio FX Mini board will store and play up to eight files
//#define AudioFX
#define AudioFXMini
//===

// Map the Audio inputs to the Nano outputs
#define fxClip_0  11
#define fxClip_1  10
#define fxClip_2  9
#define fxClip_3  8
#define fxClip_4  7
#define fxClip_5  6
#define fxClip_6  5
#define fxClip_7  4
#define fxReset   12
#define fxAct     3

//=== Required User input here
#define MIN_DELAY 500   // Adjust these two parameters to get
#define MAX_DELAY 1500  // the desired time interval between clips
                        // Make them equal for constant intervals

#if defined AudioFX
#define fxClip_8  2   // customize for your hardware setup
#define fxClip_9  13
//===

int clip[] = {fxClip_0, fxClip_1, fxClip_2, fxClip_3, fxClip_4, 
              fxClip_5, fxClip_6, fxClip_7, fxClip_8, fxClip_9 };

//=== Optional user input here
// Important! If you have loaded fewer than ten (or eight) clips,
// be SURE to modify the definition of NUM_CLIPS to match the actual
// number of audio clips
#define NUM_CLIPS 10
#else
int clip[] = {fxClip_0, fxClip_1, fxClip_2, fxClip_3, fxClip_4, 
              fxClip_5, fxClip_6, fxClip_7  };
#define NUM_CLIPS 8
#endif

//#define RANDOM // comment this line out for sequential play
//===

void setup() {
  int n;digitalWrite(clip[n],HIGH);
  for (n=0;n<NUM_CLIPS;n++) {
    pinMode(clip[n],OUTPUT);
    digitalWrite(clip[n],HIGH);
  }
  reset();
  // NOTE the reset function is not used in this sketch but
  // this code is included for other possible applications
  pinMode(fxReset,OUTPUT);
  digitalWrite(fxReset,HIGH);

  pinMode(fxAct,INPUT); // not necessary - INPUT is the default pinMode

  randomSeed(analogRead(A0));
}

// Two versions of the loop function are included:
// One plays all seven clips sequentially
// The other plays them in random sequence
void loop() {

#if defined RANDOM
// This section of loop() plays clips in random order
  play(clip[random(NUM_CLIPS-1)]);
  delay(random(MIN_DELAY,MAX_DELAY));
#else
  int n;
  for (n=0;n<NUM_CLIPS;n++) {
    play(clip[n]);
    delay(random(MIN_DELAY,MAX_DELAY));
  }
#endif
}

// The play() funtion triggers the audio clip on the specified track
void play(int track) {
  digitalWrite(track,LOW);  // Setting the Nano output pin LOW connects the AudioFX pin to ground
  delay(150);                // Leave connected to ground for 150 msec (125 msec is minimum)
  digitalWrite(track,HIGH); // Return pin to HIGH (sound will continue to play)
  while(playing()) delay(100);
}

// The playing() function returns True if a sound clip is playing; False otherwise
boolean playing() {
  return (digitalRead(fxAct)==LOW);
}

//  The reset() function stops play of the currently playing audio track
//  Nothing happens if no clip is playing
// NOTE this function is not used in the current sketch but is included
//  for possible future use
void reset() {
  digitalWrite(fxReset,LOW);  // Connects the AudioFX Reset pin to ground and stops play
  delay(150);                 // Leave connected to ground for 150  msec              
  digitalWrite(fxReset,HIGH); // Return pin to HIGH to enable play 
}
