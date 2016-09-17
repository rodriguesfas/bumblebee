/**
 * ========================================================================================================================
 * @ Software: BUMBLEBEE Lib.
 * @ Description: Library Bumblebee robot uses Driver Motor Bridge H L298N
 * @ Version: 0.0.1
 * @ Data: 14 de setembro de 2016.
 * @ Developer: Rodrigues FAS
 * @ Site: rodriguesfas.com.br
 * @ E-mail: franciscosouzaacer@gmail.com
 * @ Translate from Portuguese to English with Google Translate
 * ========================================================================================================================
 * @ Histórico Version
 *
 * @ 14 de setember de 2016 | Version 0.0.1 - Construction lib.
 *
 * ========================================================================================================================
 */

 #include "BUMBLEBEE.h"

/**
 * The BUMBLEBEE BUMBLEBEE :: Builder (x1, y1, z1, x2, y2, z2) sets the pins passed as parameter to exit and then assigns
 * Their values to private variables, so they possão be used by methods in classes
 * Future.
 */
 BUMBLEBEE::BUMBLEBEE(int pin1MotorLeft, int pin2MotorLeft, int pinSpeedMotorLeft, int pin1MotorRight, int pin2MotorRight, int pinSpeedMotorRight){
  /* Set pin as output. */
  pinMode(pin1MotorLeft, OUTPUT);
  pinMode(pin2MotorLeft, OUTPUT);
  pinMode(pinSpeedMotorLeft, OUTPUT);

  pinMode(pin1MotorRight, OUTPUT);
  pinMode(pin2MotorRight, OUTPUT);
  pinMode(pinSpeedMotorRight, OUTPUT);

  _pino1MotorLeft = pin1MotorLeft; // Motor Left pin 1
  _pino2MotorLeft = pin2MotorLeft; // Motor Left pin 2
  _pinoSpeedMotorLeft = pinSpeedMotorLeft; // Motor Left pin PWM speed 

  _pino1MotorRight = pin1MotorRight; // Motor Right pin 1
  _pino2MotorRight = pin2MotorRight; // Motor Right pin 2
  _pinoSpeedMotorRight = pinSpeedMotorRight; // Motor Right pin PWM speed
}

/**
 * move - given by parameter values of engine speeds.
 * Positive values, rotate clockwise.
 * Negative values, turns towards antiorário.
 * Value of 0 to the engine.
*/
void BUMBLEBEE::move(int speedMotorLeft, int speedMotorRight){

  /* pinSpeedMotorLeft */
  if(speedMotorLeft > 0){ //motor rotation clockwise.
    digitalWrite(_pino1MotorLeft,HIGH);
    digitalWrite(_pino2MotorLeft,LOW);
    analogWrite(_pinoSpeedMotorLeft, speedMotorLeft);
  }
  else if(speedMotorLeft < 0){ //engine speed counterclockwise.
    digitalWrite(_pino1MotorLeft,LOW);
    digitalWrite(_pino2MotorLeft,HIGH);
    analogWrite(_pinoSpeedMotorLeft, speedMotorLeft*(-1));

  }else if(speedMotorLeft == 0){ //off motor
    digitalWrite(_pino1MotorLeft, LOW);
    digitalWrite(_pino2MotorLeft, LOW);
  }

  /* pinSpeedMotorRight */
  if(speedMotorRight > 0){
    digitalWrite(_pino1MotorRight,HIGH);
    digitalWrite(_pino2MotorRight,LOW);
    analogWrite(_pinoSpeedMotorRight, speedMotorRight);
  }else if(speedMotorRight < 0){
      digitalWrite(_pino1MotorRight,LOW);
      digitalWrite(_pino2MotorRight,HIGH);
      analogWrite(_pinoSpeedMotorRight, speedMotorRight*(-1));
  }else if(speedMotorRight == 0){
        digitalWrite(_pino1MotorRight, LOW);
        digitalWrite(_pino2MotorRight, LOW);
  }

}

/**
 * stop - for both engines.
 */
 void BUMBLEBEE::stop(){

  /* stop MotorLeft */
  digitalWrite(_pino1MotorLeft, LOW);
  digitalWrite(_pino2MotorLeft, LOW);

  /* stop MotorRight */
  digitalWrite(_pino1MotorRight, LOW);
  digitalWrite(_pino2MotorRight, LOW);

}
