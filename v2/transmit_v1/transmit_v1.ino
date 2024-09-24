/*

WIRING DIFFERENCES: connect PTT to GND. Manually operate it by plugging and unplugging it. This current version works. 


NOTES: 
PD is not needed--SET IT TO HIGH CONSTANTLY. It is the power saving control pin where LOW is sleep and HIGH is normal mode. 
PD pin 11 repurposed into controlling PTT pin
Configure it to DRA818U 
*/

#include <stdio.h>
#include <SoftwareSerial.h>
#include "DRA818.h" // uncomment the following line in DRA818.h (#define DRA818_DEBUG)

/* Used Pins */
#define PD      11  // to the DRA818 PTT
#define RX      10   // arduino serial RX pin to the DRA818 TX pin
#define TX      4   // arduino serial TX pin to the DRA818 RX pin
#define MIC_IN 5 //PWM output where message is sent 

void sendMorse(const char* message) {
  for (int i = 0; message[i] != '\0'; i++) {
    switch (message[i]) {
      case 'S':
        dot(); dot(); dot(); // S: ...
        break;
      case 'O':
        dash(); dash(); dash(); // O: ---
        break;
    }
    delay(300); // Space between letters
  }
}

void dot() {
  tone(MIC_IN, 1000); // 1 kHz tone for dot
  delay(200); // Duration of dot
  noTone(MIC_IN);
  delay(100); // Space between parts of the same letter
}

void dash() {
  tone(MIC_IN, 1000); // 1 kHz tone for dash
  delay(600); // Duration of dash
  noTone(MIC_IN);
  delay(100); // Space between parts of the same letter
}









SoftwareSerial *dra_serial; // Serial connection to DRA818
DRA818 *dra;                // the DRA object once instanciated
float freq;                 // the next frequency to scan

void setup(){
  Serial.begin(9600); // for logging

  Serial.println("Booting ...");

  Serial.print("initializing I/O ... ");  
  dra_serial = new SoftwareSerial(RX, TX); // Instantiate the Software Serial Object.
  pinMode(PD, OUTPUT);                     // Power control of the DRA818
  pinMode(MIC_IN, OUTPUT);
  digitalWrite(PD,HIGH);                    // start at low power !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  Serial.println("done");

  Serial.print("initializing DRA818 ... ");
  /*
   * Configure DRA818V using 145.500 MHz, squelch 4, volume 8, no ctcss, 12.5 kHz bandwidth, all filters activated
   * We add the &Serial parameter to tell the DRA object where to log its debug information (to the builtin serial)
   *
   * Alternative call:
   *  dra = new DRA818(dra_serial, DRA818_VHF);
   *  dra->set_log(&Serial);
   *  dra->handshake();
   *  dra->group(DRA818_12K5, 145.500, 145.500, 0, 4, 0);
   *  dra->volume(8);
   *  dra->filters(true, true, true);
   */
  dra = DRA818::configure(dra_serial, DRA818_UHF, 430.040, 430.040, 4, 8, 0, 0, DRA818_12K5, true, true, true, &Serial);
  if (!dra) {
    Serial.println("\nError while configuring DRA818");
  }
  //freq = DRA818_UHF_MIN;
  //freq = 400.0000;

  Serial.println("done");

  Serial.println("Starting ... ");
}

void loop(){
  //char buf[9];
  digitalWrite(PD, HIGH); //turn to NORMAL MODE not sleep mode

  sendMorse("SSSSSSS");
  delay(100);

  /*if (!dra) return; // do nothing if DRA configuration failed
  
  dtostrf(freq, 8, 4, buf);  // convert frequency to string with right precision
  //Serial.print(String("Scanning frequency ") +  String(buf) + String(" kHz ..."));
  Serial.println(dra->scan(freq));
    /* scan the frequency 
  if (dra->scan(freq)){
    Serial.print("FOUND!!!!!!!!!!!!!!");
  }*/
  //Serial.println("");

  //freq += 0.0125; //12.5kHz step
  //if (freq > DRA818_UHF_MAX) freq = DRA818_UHF_MIN; // when DRA818_VHF_MAX (174.0) is reached, start over at DRA818_VHF_MIN (134.0)
}
