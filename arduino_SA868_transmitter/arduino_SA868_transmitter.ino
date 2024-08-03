/* 

Use the SA868 module and an arduino to transmit morse code (callsign identifier) at a specific UHF/VHF frequency for creating a transmitter for fox hunt

Code for morse code referenced from VA6MCP's fox hunt baofeng code 

Wiring specifications: 
- ardunio RX and TX pins connected to RX and TX of SA868 module. They talk through the serial module
- arduino pin 5 connected to MIC of SA868 module

*/

//SA868 PARAMETERS
#define GBW "0"//bandwidth settings
#define TFV 446.0000//transmit frequency value. example format: 134MHz would be 134.0000 --- 415.1250
#define RFV 446.0000//recieving frequency value. same format as transmit freqeucny
#define Tx_CXCSS "0012" //TX (transmitting) CXCSS Value
#define SQ 4//Squelch level
#define Rx_CXCSS "0013"//RX (recieving) CXCSS Value 

/*Important variable limitations

Frequency range: 
UHF band 400 to 480 MHz
VHF band 134 to 174 MHz

The transmission frequency and reception frequency can be the same or different, but the set 
frequency must be 12.5K and 25K integer multiple

Different CXCSS values can be used for transmission and reception, 0000: no coding
0001-0038: CTCSS After the letter: CDCSS, the code is shown in the attached table 1 in SA868 specification sheet
*/


//MORSE CODE PARAMETERS
#define tonehz 600       //the approximate frequency of the tones in hz
#define dit 64           //the length of the dit in milliseconds. The dah and pauses are derived from this.
#define rest 15000       //the amount of time between transmits in ms 60000=60seconds
#define longlength 10000 //length of long tone in milliseconds 10000=10seconds
#define audio 5          //the pin 5 on the board that controls audio output, this pin is connected to SA868 MIC pin
#define ppt 6 //still need this to allow thing to key

// Declaring Function prototypes so i can shove the functions below the loop for clarity
String formMorse(String input);
void playcode(String input);
void playtone(int note_duration);
void playcode(String input);

//global variables
int note = 1000000 / tonehz; //converts the frequency into period
int duration;
String code;

//CW array DO NOT CHANGE
// 1 = dit , 2 = dah, 0 = space
// Letters/#                 A     B      C     D    E    F      G     H      I     J     K      L    M     N    O     P      Q      R     S    T    U      V     W      X     Y      Z       0        1      2       3       4       5       6       7       8       9
String morseTable[] = {"0", "12", "2111", "2121", "211", "1", "1121", "221", "1111", "11", "1222", "212", "1211", "22", "21", "222", "1221", "2212", "121", "111", "2", "112", "1112", "122", "2112", "2122", "2211", "22222", "12222", "11222", "11122", "11112", "11111", "21111", "22111", "22211", "22221"};


void setup() {
  //for led 
  //pinMode(LED_BUILTIN, OUTPUT);

  //start serial to talk to SA868 module
  Serial.begin(9600); 
  delay(200);
  //perform handshake to determine module is connected
  //AT+DMOCONNECT
  Serial.print("AT+DMOCONNECT\r\n"); //r is carage return; n is linefeed

  //set parameters 
  //AT+DMOSETGROUP=GBW, TFV, RFV, Tx_CXCSS, SQ, Rx_CXCSS
  String comma = String(',');
  String settings= String("AT+DMOSETGROUP=" + String(GBW) + comma + TFV + comma+ RFV + comma + Tx_CXCSS + comma + SQ + comma + Rx_CXCSS);
  //Serial.println(settings);
  Serial.print(settings);
  //set morse code message
  String text = "KM6ZMN FOX KM6ZMN FOX KM6ZMN FOX";
  String code = formMorse(text);
  //Serial.println("Morse is" + code);

  //digitalWrite(ppt, LOW); //set ppt to 0
}

void loop() {
 playcode(code);
  //digitalWrite(LED_BUILTIN, HIGH);  //for led
 delay(rest);
  //digitalWrite(LED_BUILTIN, LOW); //for led
  //externally low pass filter????????
  //while(true);
}


//Converts string to string of 0 and 1 referencing the morseTable array
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
} 


//Determines length of tone and calls function playtone
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
}

//Actually prints to the "audio" pin aka pin 5 aka MIC pin of AS868
void playtone(int note_duration) {
  long elapsed_time = 0;
  long startTime = millis();
  if (note_duration > 0) {
    digitalWrite(13, HIGH);                  //See when it is making a tone on the led
    while (elapsed_time < note_duration) {
      digitalWrite(audio, HIGH);
      Serial.print("LED HIGH");
      delayMicroseconds(note / 2);
      digitalWrite(audio, LOW);
      Serial.print("LED LOW");
      delayMicroseconds(note / 2);
      elapsed_time = millis() - startTime;
    }
    digitalWrite(13, LOW);
  }
  else { //if it's a pause this will run

    delay(dit * 2);
  }
}


