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

/**
 * Compilation policies - They do not allow statements / settings library could be inserted
 * More than once in a project.
 */
#ifndef BUMBLEBEE_H
#define BUMBLEBEE_H

/* It allows you to use, access to some Arduino functions. */
#include <Arduino.h>

/**
* Class BUMBLEBEE -
*/
class BUMBLEBEE {

private:
  /* Sets pins engines */
  int _pino1MotorLeft;
  int _pino2MotorLeft;
  int _pinoSpeedMotorLeft;

  int _pino1MotorRight;
  int _pino2MotorRight;
  int _pinoSpeedMotorRight;

public:
  /* Builder receives the corresponding pin output parameters. */
  BUMBLEBEE(int pin1MotorLeft, int pin2MotorLeft, int pinSpeedMotorLeft, int pin1MotorRight, int pin2MotorRight, int pinSpeedMotorRight);

  /* Move method, receives engine speeds. */
  void move(int speedMotorLeft, int speedMotorRigth);

  void stop();
};


#endif
