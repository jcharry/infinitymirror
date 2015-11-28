
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
int s1Red = 5;                  // pin to do fancy classy fading blink at each beat
int s1Green = 6;
int s2Green = 9;
int s2Red = 10;
int s1RedFade = 0;                 // used to fade LED on with PWM on s1Red
int s1GreenFade = 0;
int s2GreenFade = 0;
int s2RedFade = 0;

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
  pinMode(s1Red, OUTPUT);         // pin that will fade to your heartbeat!
  pinMode(s1Green, OUTPUT);
  pinMode(s2Green, OUTPUT);
  pinMode(s2Red, OUTPUT);

  Serial.begin(115200);             // we agree to talk fast!

  // start of handshaking
  while (Serial.available() <= 0) {
    Serial.println("no data"); // send a starting message
    delay(300);              // wait 1/3 second
  }

  interruptSetup();                 // sets up to read Pulse Sensor signal every 2mS
  // IF YOU ARE POWERING The Pulse Sensor AT VOLTAGE LESS THAN THE BOARD VOLTAGE,
  // UN-COMMENT THE NEXT LINE AND APPLY THAT VOLTAGE TO THE A-REF PIN
  //   analogReference(EXTERNAL);
}


//  Where the Magic Happens
void loop() {
  //  analogWrite(s1Red, 255);
  //  analogWrite(s1Green, 0);
  analogWrite(s2Green, 50);
  analogWrite(s2Red, 255);

  // good values! = green @ 50, blue @ 255);
  //

  serialOutput() ;

  if (QS == true) {    // A Heartbeat Was Found
    // BPM and IBI have been Determined
    // Quantified Self "QS" true when arduino finds a heartbeat
    //    Serial.print("loop running");
    //
    //    int pulseReading = map(analogRead(A0), 0,1023, 0,255);
    //    analogWrite(5, pulseReading);
    digitalWrite(blinkPin, HIGH);    // Blink LED, we got a beat.
    s1RedFade = 255;         // Makes the LED Fade Effect Happen
    s1GreenFade = 140;
    // Set 's1RedFade' Variable to 255 to fade LED with pulse

    if (Serial.available() > 0) {
      int inByte = Serial.read();
      serialOutputWhenBeatHappens();   // A Beat Happened, Output that to serial.
    }
    QS = false;                      // reset the Quantified Self flag for next time
  }

  ledFadeToBeat();                      // Makes the LED Fade Effect Happen
  delay(20);                             //  take a break
}

void ledFadeToBeat() {
  s1RedFade -= 5;   //  set LED fade value
  s1GreenFade -= 10;
  s2GreenFade = s1RedFade;
  s2RedFade = s1RedFade;
  //Serial.println(s1GreenFade);
  s1RedFade = constrain(s1RedFade, 50, 255); //  keep LED fade value from going into negative numbers!
  s1GreenFade = constrain(s1GreenFade, 20, 140);
  analogWrite(s1Red, s1RedFade);         //  fade LED
  analogWrite(s1Green, s1GreenFade);
  //  analogWrite(s2Green, s2GreenFade);         //  fade LED
  //  analogWrite(s2Red, s2GreenFade);
}




