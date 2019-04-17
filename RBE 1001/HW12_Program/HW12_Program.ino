// Part 1

int leftEncoderValue = getEncoder(LEFT_ENCODER);
int rightEncoderValue = getEncoder(RIGHT_ENCODER);


void driveStraight(){
  int error = leftEncoderValue - rightEncoderValue   //find which encoder has turned farther
  if (error > 0) drive(speed, -1 * error);    //if left motor has turned farther, need to turn right  
  if (error < 0) drive(speed, -1 * error);    //if right motor has turned farther, need to turn left
}



// Part 2

int leftEncoderValue = getEncoder(LEFT_ENCODER);
int rightEncoderValue = getEncoder(RIGHT_ENCODER);
int distanceInInches = getUltrasonic();


void driveStraight(){
  int error = leftEncoderValue - rightEncoderValue   //find which encoder has turned farther
  double spd = distanceInInches - 10;
  if (error > 0) drive(spd, -1 * error);    //if left motor has turned farther, need to turn right  
  if (error < 0) drive(spd, -1 * error);    //if right motor has turned farther, need to turn left
}


