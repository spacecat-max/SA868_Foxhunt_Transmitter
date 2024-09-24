/*

NOTES: 
PD is not needed--SET IT TO HIGH CONSTANTLY to normal mode. It is the power saving control pin where LOW is sleep and HIGH is normal mode. 
PD pin 11 repurposed into controlling PTT pin

PTT high - off 
PPT low - on 
Configure it to DRA818U 

Make sure serial monitor is set on 9600 AND "Both NL & CR"

note: might be able to connect PTT and PD to same pin?? unless it's backwards idk lol 
*/

#include <stdio.h>
#include <SoftwareSerial.h>
#include "DRA818.h" // uncomment the following line in DRA818.h (#define DRA818_DEBUG)

/* Used Pins */
#define PTT      11  // to the DRA818 PTT
#define RX      10   // arduino serial RX pin to the DRA818 TX pin
#define TX      4   // arduino serial TX pin to the DRA818 RX pin
#define MIC_IN 5 //PWM output where message is sent 



SoftwareSerial *dra_serial; // Serial connection to DRA818
DRA818 *dra;                // the DRA object once instanciated
float freq;                 // the next frequency to scan

void setup(){
  Serial.begin(9600); // for logging

  Serial.println("Booting ...");

  Serial.print("initializing I/O ... ");  
  dra_serial = new SoftwareSerial(RX, TX); // Instantiate the Software Serial Object.
  pinMode(PTT, OUTPUT);                     // Power control of the DRA818
  pinMode(MIC_IN, OUTPUT);
  digitalWrite(PTT,HIGH);                    // high = off 
  Serial.println("done");

  Serial.print("initializing DRA818 ... ");
  /*
   * Configure DRA818V using 145.500 MHz, squelch 4, volume 8, no ctcss, 12.5 kHz bandwidth, all filters activated
   * We add the &Serial parameter to tell the DRA object where to log its debug information (to the builtin serial)
   */
  dra = DRA818::configure(dra_serial, DRA818_UHF, 430.040, 430.040, 4, 8, 0, 0, DRA818_12K5, true, true, true, &Serial);
  if (!dra) {
    Serial.println("\nError while configuring DRA818");
  }
  Serial.println("done");

  Serial.println("Starting ... ");
}

void loop(){

  sendMorse("HELLO WORLD");
  delay(2000);

}

void sendMorse(const char* message) {
  digitalWrite(PTT, LOW); //turn PTT on
  for (int i = 0; message[i] != '\0'; i++) {
    switch (message[i]) {
      case 'A':
        dot(); dash(); // A: .-
        break;
      case 'B':
        dash(); dot(); dot(); dot(); // B: -...
        break;
      case 'C':
        dash(); dot(); dash(); dot(); // C: -.-.
        break;
      case 'D':
        dash(); dot(); dot(); // D: -..
        break;
      case 'E':
        dot(); // E: .
        break;
      case 'F':
        dot(); dot(); dash(); dot(); // F: ..-.
        break;
      case 'G':
        dash(); dash(); dot(); // G: --.
        break;
      case 'H':
        dot(); dot(); dot(); dot(); // H: ....
        break;
      case 'I':
        dot(); dot(); // I: ..
        break;
      case 'J':
        dot(); dash(); dash(); dash(); // J: .---
        break;
      case 'K':
        dash(); dot(); dash(); // K: -.- 
        break;
      case 'L':
        dot(); dash(); dot(); dot(); // L: .-..
        break;
      case 'M':
        dash(); dash(); // M: --
        break;
      case 'N':
        dash(); dot(); // N: -.
        break;
      case 'O':
        dash(); dash(); dash(); // O: ---
        break;
      case 'P':
        dot(); dash(); dash(); dot(); // P: .--.
        break;
      case 'Q':
        dash(); dash(); dot(); dash(); // Q: --.-
        break;
      case 'R':
        dot(); dash(); dot(); // R: .-.
        break;
      case 'S':
        dot(); dot(); dot(); // S: ...
        break;
      case 'T':
        dash(); // T: -
        break;
      case 'U':
        dot(); dot(); dash(); // U: ..-
        break;
      case 'V':
        dot(); dot(); dot(); dash(); // V: ...-
        break;
      case 'W':
        dot(); dash(); dash(); // W: .--
        break;
      case 'X':
        dash(); dot(); dot(); dash(); // X: -..-
        break;
      case 'Y':
        dash(); dot(); dash(); dash(); // Y: -.--
        break;
      case 'Z':
        dash(); dash(); dot(); dot(); // Z: --..
        break;
      case ' ':
        delay(800); //space between words is 1400 but delay 600 is included so 1400-600 is 800
    }
    delay(600); // Space between letters
  }
  digitalWrite(PTT, HIGH); //turn PTT off
}

void dot() {
  tone(MIC_IN, 1000); // 1 kHz tone for dot
  delay(200); // Duration of dot
  noTone(MIC_IN);
  delay(200); // Space between parts of the same letter
}

void dash() {
  tone(MIC_IN, 1000); // 1 kHz tone for dash
  delay(600); // Duration of dash
  noTone(MIC_IN);
  delay(200); // Space between parts of the same letter
}
