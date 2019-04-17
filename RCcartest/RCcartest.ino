int b1=13;
int b2=12;
int f1=11;
int f2=10;

void setup() {
  pinMode(f1,OUTPUT);
  pinMode(f2,OUTPUT);
  pinMode(b1,OUTPUT);
  pinMode(b2,OUTPUT);
}

void loop() {
  digitalWrite(f1,HIGH);
  digitalWrite(f2,LOW);
  delay(500);
  digitalWrite(b1,HIGH);
  digitalWrite(b2,LOW);
  delay(500);
  digitalWrite(b1,LOW);
  digitalWrite(b2,HIGH);
  delay(500);
  digitalWrite(b1,LOW);
  digitalWrite(b2,LOW);
  digitalWrite(f1,LOW);
  digitalWrite(f2,LOW);
  delay(1000);
}
