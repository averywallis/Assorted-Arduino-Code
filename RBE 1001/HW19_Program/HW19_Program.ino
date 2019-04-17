#include "BuiltIns.h"

#define LEFT_MOTOR_PORT 1
#define RIGHT_MOTOR_PORT 2

#define LEFT_IR_SENSOR 1
#define RIGHT_IR_SENSOR 2
#define LEFT_PHOTO_SENSOR 3
#define RIGHT_PHOTO_SENSOR 4
#define  PUCK_LIMIT_SWITCH 5

#define PROXIMITY_DISTANCE 100
#define PHOTO_THRESHOLD_MIN 200
#define PHOTO_THRESHOLD_MAX 400

/**
 * antiMoth behavior.
 * The Anti-moth behavior is a ballistic behavior similar to the Escape behavior.
 * It triggers whenever the total light intensity measured by the robot’s photocells exceeds a threshold.
 * @param backupTime: The amount of time to backup. 
 * @param forwardTime: The amount of time to go forward. 
 * @param spin: The angle to spin in degrees. 
 * @param maxRandom: The maximum plus or minus random component to add to the spin angle. 
 * @param threshold: The light level at which the robot will perform an Escape sequence. 
 * @returns 1 if the behavior was triggered otherwise return 0.
 */
int antiMoth(int backupTime, int forwardTime, int spin, int maxRandom, int threshold)
{

    //get sensor data
    int left = GetAnalogInput(LEFT_PHOTO_SENSOR);
    int right = GetAnalogInput(RIGHT_PHOTO_SENSOR);
    //if the left or right sensor reading is greater than the defined threshold
    if (left > threshold || right > threshold)
    {
      //back up for backupTime
      //assumed that passing -90 to the drive function will make it go in reverse
      Drive(-90,0);
      delay(backupTime);
      //spin in a random direction
      TurnInPlace(spin + maxRadom);
      //drive forward to run away for forward time
      Drive(90,0);
      delay(forwardTime);
      //return true if happened
      return 1;
    }
  //return false if didn't happen
  return 0;
}

/**
 * Dark Push behavior.
 * The Dark-push behavior is a ballistic behavior similar to the Escape behavior.
 * It triggers whenever the robot tries to push something when no light is visible.
 * @param backupTime: The ammount of time to back up
 * @param forwardTime: The amount of time to go forward
 * @param maxRandom: The maximum plus or minus random component to add to the spin in degrees
 * @param spin: The angle to spin in degrees
 * @returns 1 if the behavior was triggered otherwise return 0.
 */
int darkPush(int backupTime, int forwardTime, int maxRandom, int spin)
{
  // To be implemented
  //get sensor data
  int left = GetAnalogInput(LEFT_PHOTO_SENSOR);
  int right = GetAnalogInput(RIGHT_PHOTO_SENSOR);
  //if both photo sensors don't read enough light and the bump sensor is hit,
  if( (left < PHOTO_THRESHOLD_MIN) && (right < PHOTO_THRESHOLD_MIN) && (BUMP_SENSOR == 0)){
    //back up for backupTime
    Drive(-90, 0);
    delay(backupTime);
    //turn around
    TurnInPlace(spin + maxRandom);
    //drive forward for forwardTime
    Drive(90, 0);
    delay(forwardTime);
    return 1;
  }
  return 0;
}

/**
 * Escape behavior.
 * The Escape behavior is a ballistic behavior triggered whenever the robot bumps into something.
 * The behavior is performed in three steps: backup for a specified amount of time, spin a certain
 * angle, and go forward for a specified amount of time. A plus or minus random component should be added to the
 * spin angle by setting the maxRandom parameter to an integer greater than one. The robot should turn in the direction which 
 * will require the least amount of rotation to end up at the desired angle.
 * @param backupTime: The amount of time to backup
 * @param forwardTime: The amount of time to go forward
 * @param maxRandom: The maximum plus or minus random component to add to the spin angle in degrees
 * @param spin: The angle in degrees to spin
 * @returns 1 if the behavior was triggered otherwise return 0.
 */
int escape(int backupTime, int forwardTime, int maxRandom, int spin)
{
  // To be implemented
  //if the bump sensor has been pressed
  if(BUMP_SENSOR == 0){
    //backup for the specific amount of backup time
    Drive(-90, 0);
    delay(backupTime);
    //turn a certain angle +/- a random component
    TurnInPlace(spin + maxRandom);
    Drive(90,);
    delay(forwardTime);
    return 1;
  }
  
  return 0;
}


