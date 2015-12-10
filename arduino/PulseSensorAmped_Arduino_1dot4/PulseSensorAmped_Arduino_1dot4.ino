
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
int innerGreen = 5;                  // 5 = blue, 6 = green (inside), 9 = red, 10 = blue
int innerBlue = 6;
int outerRed = 9;
int outerBlue = 10;
int innerGreenFade = 0;                 // used to fade LED on with PWM on innerGreen
int innerBlueFade = 0;
int outerRedFade = 0;
int outerBlueFade = 0;
int photoresistorPin = A1;

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
  pinMode(innerGreen, OUTPUT);         // pin that will fade to your heartbeat!
  pinMode(innerBlue, OUTPUT);
  pinMode(outerRed, OUTPUT);
  pinMode(outerBlue, OUTPUT);

  Serial.begin(115200);             // we agree to talk fast!

  interruptSetup();                 // sets up to read Pulse Sensor signal every 2mS
  // IF YOU ARE POWERING The Pulse Sensor AT VOLTAGE LESS THAN THE BOARD VOLTAGE,
  // UN-COMMENT THE NEXT LINE AND APPLY THAT VOLTAGE TO THE A-REF PIN
  //   analogReference(EXTERNAL);

  // start of handshaking
//  if (Serial.available() <= 0) {
//    Serial.println("no data"); // send a starting message
//    delay(300);              // wait 1/3 second
//  }
}

long idlecounter = 0;
long noHBcounter = 0;
//long initializeCounter = 0;
bool initializing = false;
long initializeLength = 100000;
long c = 0;

int ibVal = 0;
int igVal = 0;
int orVal = 0;
int obVal = 0;

//  Where the Magic Happens
void loop() {
  //  Serial.print("memory: ");
  //  Serial.println(freeRam());
  // Initialize routine must:
  // check for photoresistor value to be less than a certain value
  // not currently be running through the initialization phase
  // must not currently be reading heartbeats
  // must restart when heartbeat is not found for x seconds

  // check photoresistor value at the start of every loop
  int psval = analogRead(photoresistorPin);
  // if a hand is present (the value is low), and we haven't yet initialized, start up
  if (psval < 100 && initializing == false) {
    // run start up routine
    // send byte to tell p5 to start playing start-up audio
    Serial.println("startup");
    noHBcounter = 0;

    int outerR = 0;
    int outerB = 0;
    int innerG = 0;
    int innerB = 0;

    for (int i = 0; i < 255; i++) {
      outerR += 1;
      outerB += 4;
      innerG += 5;
      innerB += 1;

    innerG = constrain(innerG, 0, 200); //  keep LED fade value from going into negative numbers!
    innerB = constrain(innerB, 0, 200);
    outerB = constrain(outerB, 0, 200);
    outerR = constrain(outerR, 0, 200);

      analogWrite(innerGreen, innerG);
      analogWrite(innerBlue, innerB);
      analogWrite(outerRed, outerR);
      analogWrite(outerBlue, outerB);
      delay(30);
    }

    //    for (long i = 0; i < initializeLength; i++) {
    //      int mappedValue = map(i, 0, initializeLength, 0, 255);
    //      int innerBlueMV = map(i, 0, initializeLength, 0, 100);
    //      int innerGreenMV = map(i, 0, initializeLength, 20, 255);
    //      int outerRedMV = map(i, 0, initializeLength, 20, 255);
    //      int outerBlueMV = map(i, 0, initializeLength, 20, 255);
    //
    //      analogWrite(innerGreen, innerGreenMV);
    //      analogWrite(innerBlue, innerBlueMV);
    //      analogWrite(outerRed, mappedValue);
    //      analogWrite(outerBlue, mappedValue);
    //    }

    // ensure this won't run again by setting initialize = true
    initializing = true;
  }

// Create interval variable - to ensure false data

  //  serialOutput();
//  Serial.println(psval);
  if ( (QS == true) && (psval < 100) ) {    // A Heartbeat Was Found, and resistor value is low (prevents false positives)
    // set maxspeed 
    noHBcounter = 0;
    // BPM and IBI have been Determined
    // Quantified Self "QS" true when arduino finds a heartbeat

    //    digitalWrite(blinkPin, HIGH);    // Blink LED, we got a beat.
    innerGreenFade = 200;         // Makes the LED Fade Effect Happen
    innerBlueFade = 85;
    outerBlueFade = 200;
    outerRedFade = 200;
    // Set 'innerGreenFade' Variable to 255 to fade LED with pulse

    // If there's data on the serial buffer from p5, we're reading to output more serial data
//    if (Serial.available() > 0) {
      // read byte off buffer
//      int inByte = Serial.read();
      serialOutputWhenBeatHappens();   // A Beat Happened, Output that to serial.
//    }
    QS = false;                      // reset the Quantified Self flag for next time
  } else {  // no beat was found
    noHBcounter++;                   // add to the counter to track how long between found beats
    noHBcounter = constrain(noHBcounter, 0, 1001);  // constrain counter to prevent overflow
    if (noHBcounter > 500) {  // if the counter is above a certain wait time, reset initializing flag to allow for start up routine to run again
      initializing = false;
    }
  }

  ledFadeToBeat();                      // Makes the LED Fade Effect Happen
  delay(20);                             //  take a break
}

void ledFadeToBeat() {
  innerGreenFade -= 10;   //  set LED fade value
  innerBlueFade -= 2;
  outerRedFade -= 5;
  outerBlueFade -= 10;
  //Serial.println(innerBlueFade);
  if (initializing == true) {
    innerGreenFade = constrain(innerGreenFade, 0, 150); //  keep LED fade value from going into negative numbers!
    innerBlueFade = constrain(innerBlueFade, 0, 70);
    outerBlueFade = constrain(outerBlueFade, 100, 150);
    outerRedFade = constrain(outerRedFade, 20, 200);
    //        innerGreenFade = constrain(innerGreenFade, 0, 200); //  keep LED fade value from going into negative numbers!
    //    innerBlueFade = constrain(innerBlueFade, 0, 110);
    //    outerBlueFade = constrain(outerBlueFade, 100, 200);
    //    outerRedFade = constrain(outerRedFade, 20, 255);
  } else {
    innerGreenFade = constrain(innerGreenFade, 0, 200); //  keep LED fade value from going into negative numbers!
    innerBlueFade = constrain(innerBlueFade, 0, 110);
    outerBlueFade = constrain(outerBlueFade, 20, 200);
    outerRedFade = constrain(outerRedFade, 4, 255);
  }

  analogWrite(innerGreen, innerGreenFade);         //  fade LED
  analogWrite(innerBlue, innerBlueFade);
  analogWrite(outerBlue, outerBlueFade);
  analogWrite(outerRed, outerRedFade);
}




