/**
 * Crowdtap Thingie
 * Hilal Koyuncu & Michael D'Auria
 */

#include <NewPing.h>

const byte btnSilence  = 2; // INT0 & SILENCE button
const byte btnPresence = 3; // INT1 & PRESENCE button

const byte senTrigger  = 12;  // US Trigger
const byte senEcho     = 6;   // US Echo
const byte maxDistance = 100; // US Maximum detection distance

const byte ledBlue  =  9;
const byte ledGreen = 10;
const byte ledRed   = 11;

static volatile byte present = false;
static volatile byte quiet   = false;

static volatile byte prevPresent = false;
static volatile byte prevQuiet   = false;

const unsigned int quietTime = 10*1000; // s
static volatile unsigned long quietTimer = 0;

const byte debounce = 1200; // ms

NewPing sonar(senTrigger, senEcho, maxDistance);

void setup()  {
  pinMode(ledRed,   OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledBlue,  OUTPUT);

  pinMode(btnPresence, INPUT);
  digitalWrite(btnPresence, HIGH);
  attachInterrupt(0, presenceDetected, LOW);

  pinMode(btnSilence, INPUT);
  digitalWrite(btnSilence, HIGH);
  attachInterrupt(1, quietDetected, LOW);

  pinMode(13, INPUT);
  digitalWrite(13, HIGH);

  setAway();
  Serial.begin(115200);
}

void loop() {
  if (prevPresent != present || prevQuiet != quiet) {
    if (present) {
      if (quiet) {
        setQuiet();
      } else {
        setPresent();
      }
    } else {
      setAway();
    }
  }

  prevPresent = present;
  prevQuiet = quiet;

  unsigned int sonarDistance = sonar.ping()/US_ROUNDTRIP_CM;
  //Serial.println(sonarDistance);  
  present = (sonarDistance > 0);

  if (quietTimer > 0 && millis() > quietTimer) {
    setPresent();
  }

  delay(500);
}

void setColor(byte red, byte green, byte blue) {
  red   = constrain(red,   0, 255);
  green = constrain(green, 0, 255);
  blue  = constrain(blue,  0, 255);

  analogWrite(ledRed,   red);
  analogWrite(ledGreen, green);
  analogWrite(ledBlue,  blue);
}

void setAway() {
  Serial.println("away");
  setColor(25, 0, 0);
  present = false;
  quiet = false;
}

void setPresent() {
  Serial.println("present");
  setColor(0, 25, 0);
  present = true;
  quiet = false;
  quietTimer = 0;
}

void setQuiet() {
  Serial.println("quiet");
  setColor(0, 0, 25);
  quiet = true;
}

void presenceDetected() {
  static unsigned long lastInterruptTime0 = 0;
  unsigned long interruptTime0 = millis();

  if (interruptTime0 - lastInterruptTime0 > debounce) {
    setPresent();
  }

  lastInterruptTime0 = interruptTime0;
}

void quietDetected() {
  static unsigned long lastInterruptTime1 = 0;
  unsigned long interruptTime1 = millis();

  if (present && interruptTime1 - lastInterruptTime1 > debounce) {
    setQuiet();
    quietTimer = interruptTime1 + quietTime;
  }

  lastInterruptTime1 = interruptTime1;
}
