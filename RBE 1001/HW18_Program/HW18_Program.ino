//define frequencies
#define c 3830 // 261 Hz
#define cs 2610 // 277 Hz
#define d 3400 // 294 Hz
#define ds 3215 // 311 Hz
#define e 3038 // 329 Hz
#define f 2864 // 349 Hz
#define g 2550 // 392 Hz
#define a 2272 // 440 Hz
#define b 2028 // 493 Hz
#define C 1912 // 523 Hz
#define D 1703 // 587 Hz

//define the structure
struct Notes {
int note;
int duration;
}

//define the sequence and length of notes
//edited to include a note that was missed from original///
notes[] = {
  {b, 1},
  {a, 1},
  {g, 1},
  {a, 1},
  {b, 1},
  {b, 1},
  {b, 2},
  {a, 1},
  {a, 1},
  {a, 2},
  {b, 1},
  {D, 1},
  {D, 2},
  {b, 1},
  {a, 1},
  {g, 1},
  {a, 1},
  {b, 1},
  {b, 1},
  {b, 1},
  {b, 1},
  {a, 1},
  {a, 1},
  {b, 1},
  {a, 1},
  {g, 4},
  {0,0}
};

//digitalpin 9 is used as the output
const int buzzerPin = 6;

//changes how fast or slow the song plays (larger value == slower);
const int tempo = 400;

//initial time is zero
long time1 = 0;

void setup() {
  //define pin output
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  //for each array in the array of arrays
  for (int x = 0; x < 27; x++){
    //while millis is less than the previous time plus the duration times the tempo
    while(millis() < time1 + notes[x].duration*tempo){
      //turn buzzer on for very short time
      digitalWrite(buzzerPin, HIGH);
      delayMicroseconds(notes[x].note/2);
      //turn buzzer off for very short time
      digitalWrite(buzzerPin, LOW);
      delayMicroseconds(notes[x].note/2);
      //delaying by the frequency / 2 to get proper pitch
      //because the buzzer needs to be high for half the frequency, and low for the other half
    }
  //change the time variable to equal the current time
  time1 = millis();

  //set the buzzer to low to distinguish between individual notes when the pitch doesn't change
  digitalWrite(buzzerPin, LOW);
  delayMicroseconds(50);

  }

}
