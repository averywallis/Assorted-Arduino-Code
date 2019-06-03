int led_group1 = 9;
int led_group2 = 10;
int led_group3 = 11;
int led_group4 = 12;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(led_group1, OUTPUT);
  pinMode(led_group2, OUTPUT);
  pinMode(led_group3, OUTPUT);
  pinMode(led_group4, OUTPUT);
  digitalWrite(led_group1, LOW);
  digitalWrite(led_group2, LOW);
  digitalWrite(led_group3, LOW);
  digitalWrite(led_group4, LOW);
  randomSeed(analogRead(0));
}
int num = 0;
void loop() {
  // put your main code here, to run repeatedly:
//  for(int x = 1; x < 7; x++){
//    setDie(x);
//    Serial.print("Number: "); Serial.println(x);
//    delay(1000);
//  }
  num = random(1,7);
  setDie(num);
  Serial.println(num);
  delay(100);
}

void setDie(int number){
  //if number uses group 1
  if(number == 2 or number == 4 or number == 5 or number == 6){
    digitalWrite(led_group1, HIGH);
  }
  else{
    digitalWrite(led_group1, LOW);
  }

  //if number uses group 2
  if(number == 6){
    digitalWrite(led_group2, HIGH);
  }
  else{
    digitalWrite(led_group2, LOW);
  }

  //if number uses group 3
  if(number == 3 or number == 4 or number == 5 or number == 6){
    digitalWrite(led_group3, HIGH);
  }
  else{
    digitalWrite(led_group3, LOW);
  }
  
  //if number uses group 4
  if(number == 1 or number == 3 or number == 5){
    digitalWrite(led_group4, HIGH);
  }
  else{
    digitalWrite(led_group4, LOW);
  }
  
}
