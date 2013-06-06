int blue     = 9;  // BLUE
int green    = 10; // GREEN
int red      = 11; // RED
int silence  = 12; // SILENCE button
int presence = 13; // PRESENCE button

boolean present = false;
boolean quiet   = false;

void setup()  {
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);

  pinMode(silence, INPUT);
  pinMode(presence, INPUT);
}

void loop() {
  if (present) {
    if(quiet) {
      setColor(0,0,60);
    } else {
      setColor(0,60,0);
    }
  } else {
    setColor(255,0,0);
  }

  quiet   = digitalRead(silence);
  present = digitalRead(presence);
}

void setColor(int r, int g, int b) {
  analogWrite(red, r);
  analogWrite(green, g);
  analogWrite(blue, b);  
}


