/*
  Originally written by VE6BTC IDE 1.60
  Pin 7 is your tx                                   Line 22
  Pin 5 controls autio output                        Line 23
  Transmission delay "rest"                          Line 21

  Updated Nov 2020, VA6MCP IDE 1.8.13
  The purpose of this sketch is to use your Baofeng UV5R or other radio
  and to tx morse code for a fox hunt.

*/

#define tonehz 600       //the approximate frequency of the tones in hz, in reality it will be a tad lower, and includes lots of harmonics.
#define dit 64           //the length of the dit in milliseconds. The dah and pauses are derived from this.
#define rest 6000       //the amount of time between transmits in ms 60000=60seconds
#define longlength 10000 //length of long tone in milliseconds 10000=10seconds
#define tx 7             //pin 7 on the board controls the tx, signal to 5v relay where you would have speaker/mic pins connected
#define audio 5          //the pin 5 on the board that controls audio output, connected to mic on radio


//CW array DO NOT CHANGE
// 1 = dit , 2 = dah, 0 = space
// Letters/#                 A     B      C     D    E    F      G     H      I     J     K      L    M     N    O     P      Q      R     S    T    U      V     W      X     Y      Z       0        1      2       3       4       5       6       7       8       9
String morseTable[] = {"0", "12", "2111", "2121", "211", "1", "1121", "221", "1111", "11", "1222", "212", "1211", "22", "21", "222", "1221", "2212", "121", "111", "2", "112", "1112", "122", "2112", "2122", "2211", "22222", "12222", "11222", "11122", "11112", "11111", "21111", "22111", "22211", "22221"};


// Declaring Function prototypes
String formMorse(String input);
void playcode(String input);
void playtone(int note_duration);


// Change to your call sign and text you want to announces
String Text = "KK6UC FOX KK6UC FOX";                   //This is what will be transmitted
String code = formMorse(Text);
String easterEgg = "KK6UC Fox found Fox Found KK6UC";  //This will be transmitted instead if pin 10 is held high at reset.


// Global Varibables
bool egg = 0;
int TextChars = 15;
int CodeChars;
String EEgg = formMorse(easterEgg);
int duration;
int note = 1000000 / tonehz; //converts the frequency into period



void setup() { //set the pins to output mode
  pinMode(tx, OUTPUT);
  pinMode(10, INPUT);
  pinMode(audio, OUTPUT);
  pinMode(13, OUTPUT);               //this is to see how the cw looks with on board led pin 13
  egg = digitalRead(10);

  // Used for Debugging
  Serial.begin(9600);
  Serial.println(egg);
  Serial.println(Text);
  Serial.println(code);
  Serial.println(easterEgg);
  Serial.println(EEgg);

} // End of setup loop

void loop() {

  /* his transmits a long tone "optianl", followed by a call in CW.
    It then waits a set amount of time and transmits again
    This transmits by connecting to a radio's ptt and mic input.
  */
  digitalWrite(tx, HIGH); 
  playtone(longlength);
  
  // if (egg) {
  //   //Serial.println(EEgg);
  //   playcode(EEgg);
  // } // End of if egg statment
  // else {
  //   //Serial.println(code);
  delay(250);
  digitalWrite(tx, LOW);     //starts the radio transmitting 
  
  playcode(code);
  
  //} // End of Else statment

  digitalWrite(tx, HIGH); //Stops the radio's transmission

  delay(rest);           // Delay in transmissions
} // End of loop



//   Function definition: playtone
void playtone(int note_duration) {
  long elapsed_time = 0;
  long startTime = millis();
  if (note_duration > 0) {
    digitalWrite(13, HIGH);                  //See when it is making a tone on the led
    while (elapsed_time < note_duration) {
      digitalWrite(audio, HIGH);
      delayMicroseconds(note / 2);
      digitalWrite(audio, LOW);
      delayMicroseconds(note / 2);
      elapsed_time = millis() - startTime;
    }
    digitalWrite(13, LOW);
  }
  else { //if it's a pause this will run

    delay(dit * 2);
  }
} // End of Function "playtone"


//   Function definition: playcode
void playcode(String input) {
  for (int i = 0; i < input.length(); i++) {
    Serial.print(input[i]);
    if (input[i] == '0') { //See if it's a pause
      duration = 0;
    }
    else if (input[i] == '1') { //See if it's a dit
      duration = dit;
    }
    else if (input[i] == '2') { //See if it's a dah
      duration = dit * 3;
    }
    playtone(duration);
    delay(dit); //makes a pause between sounds, otherwise each letter would be continuous.
  }
  Serial.println();
} // End of Function "playcode"


//   Function definition: formMorse
String formMorse(String input) {
  input.toUpperCase();
  String output = "";
  for (int i = 0; i < input.length() ; i++) {
    if (input[i] >= 65 && input[i] <= 90)
      output = output + morseTable[input[i] - 64] + '0';
    else if (input[i] >= 48 && input[i] <= 57)
      output = output + morseTable[input[i] - 21] + '0';
    else if (input[i] == 32)
      output = output + morseTable[0];
  }
  return output;
} // End of Function "formMorse"
