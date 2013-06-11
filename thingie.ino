/**
 * Crowdtap Thingie
 * Hilal Koyuncu & Michael D'Auria
 */

const boolean commonAnode = false;

const byte btnSilence  = 2; // INT0 & SILENCE button
const byte btnPresence = 3; // INT1 & PRESENCE button

const byte ledBlue  =  9; // BLUE
const byte ledGreen = 10; // GREEN
const byte ledRed   = 11; // RED

static byte present = false;
static byte quiet   = false;

static byte prevPresent = false;
static byte prevQuiet   = false;

const unsigned int quietTime = 10*1000; // s
static unsigned long quietTimer = 0;

const byte debounce = 1200; // ms

void setup()  {
  pinMode(ledRed,   OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledBlue,  OUTPUT);

  pinMode(btnPresence, INPUT);
  digitalWrite(btnPresence, HIGH);
  attachInterrupt(0, togglePresence, FALLING);

  pinMode(btnSilence, INPUT);
  digitalWrite(btnSilence, HIGH);
  attachInterrupt(1, toggleQuiet, FALLING);

  setAway();
  Serial.begin(9600);
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

    prevPresent = present;
    prevQuiet = quiet;
  }

  if (quietTimer > 0 && millis() > quietTimer) {
    toggleQuiet();
  }
}

void setColor(byte red, byte green, byte blue) {
  if (commonAnode) {
    red   = 255-red;
    green = 255-green;
    blue  = 255-blue;
  }

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
}

void setPresent() {
  Serial.println("present");
  setColor(0, 25, 0);
  present = true;
  quiet = false;
}

void setQuiet() {
  Serial.println("quiet");
  setColor(0, 0, 25);
  quiet = true;
}

void togglePresence() {
  static unsigned long lastInterruptTime0 = 0;
  unsigned long interruptTime0 = millis();

  if (interruptTime0 - lastInterruptTime0 > debounce) {
    if (quiet) {
      toggleQuiet();
    }

    present = !present;
  }

  lastInterruptTime0 = interruptTime0;
}

void toggleQuiet() {
  static unsigned long lastInterruptTime1 = 0;
  unsigned long interruptTime1 = millis();

  if (interruptTime1 - lastInterruptTime1 > debounce) {
    quiet = !quiet;
    quietTimer = (quiet) ? (interruptTime1 + quietTime) : 0;
  }

  lastInterruptTime1 = interruptTime1;
}
