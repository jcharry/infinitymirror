
/*  Pulse Sensor Amped 1.4    by Joel Murphy and Yury Gitman   http://www.pulsesensor.com

  ----------------------  Notes ----------------------  ----------------------
  This code:
  1) Blinks an LED to User's Live Heartbeat   PIN 13
  2) Fades an LED to User's Live HeartBeat
  3) Determines BPM
  4) Prints All of the Above to Serial

  Read Me:
  https://github.com/WorldFamousElectronics/PulseSensor_Amped_Arduino/blob/master/README.md
  ----------------------       ----------------------  ----------------------
*/

//  Variables
int pulsePin = 0;                 // Pulse Sensor purple wire connected to analog pin 0
int blinkPin = 13;                // pin to blink led at each beat
int innerBlue = 5;                  // 5 = blue, 6 = green (inside), 9 = red, 10 = blue
int innerGreen = 6;
int outerRed = 9;
int outerBlue = 10;
int innerBlueFade = 0;                 // used to fade LED on with PWM on innerBlue
int innerGreenFade = 0;
int outerRedFade = 0;
int outerBlueFade = 0;

int loopcount = 0;

// Volatile Variables, used in the interrupt service routine!
volatile int BPM;                   // int that holds raw Analog in 0. updated every 2mS
volatile int Signal;                // holds the incoming raw data
volatile int IBI = 600;             // int that holds the time interval between beats! Must be seeded!
volatile boolean Pulse = false;     // "True" when User's live heartbeat is detected. "False" when not a "live beat".
volatile boolean QS = false;        // becomes true when Arduoino finds a beat.

// Regards Serial OutPut  -- Set This Up to your needs
static boolean serialVisual = false;   // Set to 'false' by Default.  Re-set to 'true' to see Arduino Serial Monitor ASCII Visual Pulse


void setup() {
  pinMode(blinkPin, OUTPUT);        // pin that will blink to your heartbeat!
  pinMode(innerBlue, OUTPUT);         // pin that will fade to your heartbeat!
  pinMode(innerGreen, OUTPUT);
  pinMode(outerRed, OUTPUT);
  pinMode(outerBlue, OUTPUT);

  Serial.begin(115200);             // we agree to talk fast!

  // start of handshaking
  if(Serial.available() <= 0) {
    Serial.println("no data"); // send a starting message
    delay(300);              // wait 1/3 second
  }

  interruptSetup();                 // sets up to read Pulse Sensor signal every 2mS
  // IF YOU ARE POWERING The Pulse Sensor AT VOLTAGE LESS THAN THE BOARD VOLTAGE,
  // UN-COMMENT THE NEXT LINE AND APPLY THAT VOLTAGE TO THE A-REF PIN
  //   analogReference(EXTERNAL);
}

int idlecounter = 0;
//  Where the Magic Happens
void loop() {
//    analogWrite(innerBlue, 255);
//    analogWrite(innerGreen, 0);
//  analogWrite(outerRed, 0);
//  analogWrite(outerBlue, 255);

  // good values! = green @ 50, blue @ 255);


  

  serialOutput() ;

//  if (idlecounter >= 30) {
//    analogWrite(outerBlue, 0);
//    analogWrite(outerRed, 0);
//  }

  if (QS == true) {    // A Heartbeat Was Found
    // reset idle counter
    idlecounter = 0;
    // BPM and IBI have been Determined
    // Quantified Self "QS" true when arduino finds a heartbeat
    //    Serial.print("loop running");
    //
    //    int pulseReading = map(analogRead(A0), 0,1023, 0,255);
    //    analogWrite(5, pulseReading);
    digitalWrite(blinkPin, HIGH);    // Blink LED, we got a beat.
    innerBlueFade = 255;         // Makes the LED Fade Effect Happen
    innerGreenFade = 140;
    outerBlueFade = 255;
    outerRedFade = 255;
    // Set 'innerBlueFade' Variable to 255 to fade LED with pulse

    if (Serial.available() > 0) {
      int inByte = Serial.read();
      serialOutputWhenBeatHappens();   // A Beat Happened, Output that to serial.
    }
    QS = false;                      // reset the Quantified Self flag for next time
  } else {
    idlecounter++;
  }

  ledFadeToBeat();                      // Makes the LED Fade Effect Happen
  delay(20);                             //  take a break

  loopcount++;
}

void ledFadeToBeat() {
  innerBlueFade -= 5;   //  set LED fade value
  innerGreenFade -= 5;
  outerRedFade -= 5;
  outerBlueFade -= 10;
  //Serial.println(innerGreenFade);
  innerBlueFade = constrain(innerBlueFade, 0,200); //  keep LED fade value from going into negative numbers!
  innerGreenFade = constrain(innerGreenFade, 0, 110);
  outerBlueFade = constrain(outerBlueFade, 100, 200);
  outerRedFade = constrain(outerRedFade, 20,255);
  analogWrite(innerBlue, innerBlueFade);         //  fade LED
  analogWrite(innerGreen, innerGreenFade);
  analogWrite(outerBlue, outerBlueFade);
  analogWrite(outerRed, outerRedFade);
}




