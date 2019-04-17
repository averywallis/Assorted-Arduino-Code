char test='A';
long count=2147483600;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println(test);
  Serial.print("The ASCII Code for 'A' is: ");
  Serial.print((int)test);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(count);
  count++;
  delay(1000);
}
