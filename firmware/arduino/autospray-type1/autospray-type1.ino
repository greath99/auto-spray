
// Set pin numbers
const int PIN_PUMP_RELAY    = 3;  // 5v Relay
const int PIN_BTN_SPRAY     = 4;  // Push Button
const int PIN_SENSOR_PIR    = 7;  // Passive Infrated Sensor
const int PIN_PIEZO         = 8;  // Piezo Speaker
const int BPS_SERIAL        = 9600;

int STATUS_RELAY_PUMP       = 0;
int STATUS_PIR              = 0;

#define NOTE_E7  2637
#define NOTE_F7  2794

int MELODY_STARTING[] = {
  NOTE_F7, NOTE_F7, NOTE_F7, 0,
  NOTE_F7, NOTE_F7, 0, 0
};

int TEMPO_STARTING[] = {
  3, 3, 3, 6,
  3, 3, 6, 6
};

// to increase the injection time,
// add lines in the 'MELODY_SPARY' and 'TEMPO_SPARY' arrays
int MELODY_SPARY[] = {
  NOTE_E7, 0, 0, 0,
  NOTE_E7, 0, 0, 0,
 
  NOTE_E7, 0, 0, 0,
  NOTE_E7, 0, 0, 0,
  NOTE_E7, 0, 0, 0,
  0, NOTE_E7, NOTE_E7, NOTE_F7
};

int TEMPO_SPARY[] = {
  6, 6, 6, 6,
  6, 6, 6, 6,
 
  6, 6, 6, 6,
  6, 6, 6, 6,
  6, 6, 6, 6,
  6, 6, 6, 3
};

void setup() {
  // initialize the relay signal pin as an output
  pinMode(PIN_PUMP_RELAY, OUTPUT);
  // initialize the relay signal pin as an output
  pinMode(PIN_PIEZO, OUTPUT);
  // initialize the push button pin as an input
  pinMode(PIN_BTN_SPRAY, INPUT);
  // initialize the human detec sensor pin as an input
  pinMode(PIN_SENSOR_PIR, INPUT);
 
  offPump();
  Serial.begin(BPS_SERIAL);
}

void loop() {
  checkSprayButton();
  checkPirSensor();
  delay(100);
}

void checkPirSensor() {
  STATUS_PIR = digitalRead(PIN_SENSOR_PIR);

  if (STATUS_PIR == HIGH) {
    Serial.println("#### PIR Status : high");
    onPump();
  }
}

void checkSprayButton() {
  STATUS_RELAY_PUMP = digitalRead(PIN_BTN_SPRAY);
  
  if (STATUS_RELAY_PUMP == HIGH) {
    Serial.println("#### Push Button Status : high");
    onPump();
  }
}

void onPump() {
  Serial.println("#### Pump Status : on");
  playBuzzStarting();
  digitalWrite(PIN_PUMP_RELAY, HIGH);
  //delay(SPRAY_ON_TIME_SEC * 1000);
  playBuzzSpray();
  offPump();
}

void offPump() {
  Serial.println("#### Pump Status : off");
  digitalWrite(PIN_PUMP_RELAY, LOW);
  STATUS_RELAY_PUMP = 0;
}


void buzz(int targetPin, long frequency, long length) {
  long delayValue = 1000000 / frequency / 2; // calculate the delay value between transitions
  //// 1 second's worth of microseconds, divided by the frequency, then split in half since
  //// there are two phases to each cycle
  long numCycles = frequency * length / 1000; // calculate the number of cycles for proper timing
  //// multiply frequency, which is really cycles per second, by the number of seconds to
  //// get the total number of cycles to produce
  for (long i = 0; i < numCycles; i++) { // for the calculated length of time...
    digitalWrite(targetPin, HIGH); // write the buzzer pin high to push out the diaphram
    delayMicroseconds(delayValue); // wait for the calculated delay value
    digitalWrite(targetPin, LOW); // write the buzzer pin low to pull back the diaphram
    delayMicroseconds(delayValue); // wait again or the calculated delay value
  }
}

void playBuzzStarting() {
  int size = sizeof(MELODY_STARTING) / sizeof(int);
  for (int thisNote = 0; thisNote < size; thisNote++) {
    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / TEMPO_STARTING[thisNote];

    buzz(PIN_PIEZO, MELODY_STARTING[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);

    // stop the tone playing:
    buzz(PIN_PIEZO, 0, noteDuration);
  }
}

void playBuzzSpray() {
  int size = sizeof(MELODY_SPARY) / sizeof(int);
  for (int thisNote = 0; thisNote < size; thisNote++) {
    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / TEMPO_SPARY[thisNote];

    buzz(PIN_PIEZO, MELODY_SPARY[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);

    // stop the tone playing:
    buzz(PIN_PIEZO, 0, noteDuration);
  }
}
