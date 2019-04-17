// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int ledg = 13;
int ledy = 12;
int ledr = 11;
int ledButton = 2;

enum LightStatus
{
  GREEN,
  YELLOW,
  RED,
  NONE
};

LightStatus status;

// the setup routine runs once when you press reset:
void setup() {
  // initialize the digital pin as an output.
  pinMode(ledg, OUTPUT);
  pinMode(ledy, OUTPUT);
  pinMode(ledr, OUTPUT);
  pinMode(ledButton, INPUT);
  status = NONE;
  digitalWrite(ledg,0);
  digitalWrite(ledy,0);
  digitalWrite(ledr,0);
}

// the loop routine runs over and over again forever:
void loop() {
  if(digitalRead(ledButton)==HIGH)
  {
    switch(status)
    {
    case NONE:
      digitalWrite(ledg,20);
      digitalWrite(ledy,0);
      digitalWrite(ledr,0);
      status = GREEN;
      break;
     case GREEN:
      digitalWrite(ledg,0);
      digitalWrite(ledy,10);
      digitalWrite(ledr,0);
      status = YELLOW;
      break;
    case YELLOW:
      digitalWrite(ledg,0);
      digitalWrite(ledy,0);
      digitalWrite(ledr,10);
      status = RED;
      break;
    case RED:
      digitalWrite(ledg,0);
      digitalWrite(ledy,0);
      digitalWrite(ledr,0);
      status = NONE;
      break;
    }
    delay(500);
  }

}
