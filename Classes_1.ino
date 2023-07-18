/*
 *  Listing 1: Classes_1
 *  Copyright (c) 2023 by Richard J. Wissbaum
 *  All rights reserved
 *  
 *  Permission is granted to private, non-commerical use only.
 */
 
#define RED_ON 18000
#define YLW_ON 3000
const unsigned GRN_ON = RED_ON - YLW_ON;

unsigned red1 = 3;
unsigned ylw1 = 4;
unsigned grn1 = 5;

void setup() {
  pinMode(red1,OUTPUT);
  pinMode(grn1,OUTPUT);
  pinMode(ylw1,OUTPUT);
  // turn all lights off
  digitalWrite(red1,LOW);
  digitalWrite(ylw1,LOW);
  digitalWrite(grn1,LOW);
}

void loop() {
  // start with red LED on
  digitalWrite(red1,HIGH);
  delay(RED_ON);

  // turn red LED off, green LED on
  digitalWrite(red1,LOW);
  digitalWrite(grn1,HIGH);
  delay(GRN_ON);

  // turn green LED off, yellow LED on
  digitalWrite(grn1,LOW);
  digitalWrite(ylw1,HIGH);
  delay(YLW_ON);

  // turn yellow LED off
  digitalWrite(ylw1,LOW);
}
