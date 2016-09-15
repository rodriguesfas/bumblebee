/**
   ==========================================================================================
   @ Software: DICK_Simple_Motor_Drive
   @ Description: Simple example, is to start the engine clockwise
   *                and off.
   @ Version: 0.0.1
   @ Data: 14 de setember de 2016
   @ Developer Rodrigues F. A. S.
   @ Site: rodriguesfas.com.br
   @ E-mail: franciscosouzaacer@gmail.com
   @ Translate from Portuguese to English with Google Translate
   ==========================================================================================
*/

/* import lib. */
#include <DICK.h>

/**
   Instantiate an object of class DICK, passing parameters the connecting pins
   where this on each pin of the H bridge L298N in Arduino doors.
*/

#define pin1MotorLeft 2
#define pin2MotorLeft 4
#define pinSpeedMotorLeft 3 //PWM
#define pin1MotorRight 6
#define pin2MotorRight 7
#define pinSpeedMotorRight 5 //PWM
DICK dick(pin1MotorLeft, pin2MotorLeft, pinSpeedMotorLeft, pin1MotorRight, pin2MotorRight, pinSpeedMotorRight);

void setup() {}

void loop() {
  dick.move(100, 100); // positive values alloy engine clockwise.
  delay(3000); // waiting 3 seconds,

  dick.move(0, 0); //engine off, one or the other or both.
  delay(3000);

  dick.move(-100, -100); // negative values alloy engine counterclockwise.
  delay(3000);

  dick.stop(); //turns off both motors.
  delay(3000);
}
