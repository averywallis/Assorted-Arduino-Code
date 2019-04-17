#ifndef LINFOLLOW
#define LINFOLLOW

//declare oppropriate functions that will be called by the template
void preperation(Servo lMotor, int lMotorPort, Servo rMotor, int rMotorPort, Servo lArmMotor, int lArmMotorPort, Servo rArmMotor, int rArmMotorPort, Servo collectorMotor, int collectorPort);
void Drive(int speed);
void Lift(int level);
void lcdPrint(int sensor);
void followLine(int leftSensor, int rightSensor);
void DoDriving();
void auton();

#endif
