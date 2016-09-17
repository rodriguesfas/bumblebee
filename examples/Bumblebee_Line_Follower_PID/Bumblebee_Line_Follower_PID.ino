/**
   ==========================================================================================
   @ Software: Bumblebee_Line_Follower_PID
   @ Description: Line follower pid
   @ Version: 0.0.1
   @ Data: 14 de setember de 2016
   @ Developer Rodrigues F. A. S.
   @ Site: rodriguesfas.com.br
   @ E-mail: franciscosouzaacer@gmail.com
   @ Translate from Portuguese to English with Google Translate
   @ Website Lib. QTRSensors.h: <https://github.com/pololu/qtr-sensors-arduino/releases>
   ==========================================================================================
*/

/* importe da lib. */
#include <BUMBLEBEE.h>
#include <QTRSensors.h>

#define QUANT_SENSORS             5  /* Defined amount of sensors. */
#define QUANT_SAMPLES_PER_SENSOR  5  /* sample mean by analog readings of each sensor */
#define sensorLinePin0 0
#define sensorLinePin1 1
#define sensorLinePin2 2
#define sensorLinePin3 3
#define sensorLinePin4 6
QTRSensorsAnalog qtra((unsigned char[]) {
  sensorLinePin0, sensorLinePin1, sensorLinePin2, sensorLinePin3, sensorLinePin4
}, QUANT_SENSORS, QUANT_SAMPLES_PER_SENSOR, QTR_NO_EMITTER_PIN);
unsigned int sensorValues[QUANT_SENSORS];

/**
   Instantiate an object of class BUMBLEBEE, passing parameters the connecting pins
   where this on each pin of the H bridge L298N in Arduino doors.
*/
#define pin1MotorLeft      2
#define pin2MotorLeft      4
#define pinSpeedMotorLeft  3 //PWM
#define pin1MotorRight     6
#define pin2MotorRight     7
#define pinSpeedMotorRight 5 //PWM
BUMBLEBEE bee(pin1MotorLeft, pin2MotorLeft, pinSpeedMotorLeft, pin1MotorRight, pin2MotorRight, pinSpeedMotorRight);

/* Pin's LED RGB. */
#define VERMELHO 10
#define VERDE     9
#define AZUL     13

int last_proportional;
int integral;

/**
   ========================================================================================================================
   setup - configuration initial.
*/
void setup() {
  Serial.begin(9600);

  /* Config pinos Led RGB. */
  pinMode(VERDE, OUTPUT);
  pinMode(AZUL, OUTPUT);
  pinMode(VERMELHO, OUTPUT);

  calibration();
}

/**
   ========================================================================================================================
    calibration - Performs the necessary instructions to calibrate the IR sensor.
*/
void calibration() {
  ledRED(); /* bright red warning color, informs that the sensors are being calibrated. */

  /* Move robo in turning around themselves, for sensor calibration. Note .: Position the robot on the line. */
  bee.move(-115, 115);

  /* Performs calibration in seconds. (Approximately 10s) */
  for (int i = 0; i < 100; i++) { //400
    qtra.calibrate(); /* Read all the sensors 10 times 2.5ms. (25ms for each call). */
  }

  bee.stop();

  /* Print calibration settings values. */
  // printCalibration();

  ledGREEN(); /* bright warning color green, which informs the completion of the sensor calibration. */
}

/**
   ========================================================================================================================
   loop -
*/
void loop() {
  linePID();
}

void linePID() {
  // Gets the line position
  // Here we are not interested in the individual values of each sensor
  unsigned int position = qtra.readLine(sensorValues);

  // The proportional term must be 0 when we are in line
  int proportional = ((int)position) - 2000;

  // Calculates the derivative term (change) and the integral term (sum) of the position
  int derivative = proportional - last_proportional;
  integral += proportional;

  // Remembering the last position
  last_proportional = proportional;

  /**
     It computes the difference between the power of the two motors Aranjo
       * M1 - M2. If a positive number, the robot will turn to
       * Right. If it is a negative number, the robot will turn left
       * And magnetude of numbers determine the acuteness with which will make the curves / turns
  */
  int power_difference = proportional / 10 + integral / 10000 + derivative * 3 / 2;

  /**
     Calculates the current configuration of the engines. We will never set
       * A motor with a negative value
  */
  const int max = 180;

  if (power_difference > max) power_difference = max;
  if (power_difference < -max) power_difference = -max;

  if (power_difference < 0)
    set_motors(max + power_difference, max);
  else
    set_motors(max, max - power_difference);

}

/**
   ========================================================================================================================
   set_motors - Driving the motors
*/
void set_motors(int left_speed, int right_speed) {
  if (right_speed >= 0 && left_speed >= 0)
    bee.move(right_speed, left_speed);

  if (right_speed >= 0 && left_speed < 0) {
    left_speed = -left_speed;
    bee.move(right_speed, left_speed);
  }

  if (right_speed < 0 && left_speed >= 0) {
    right_speed = -right_speed;
    bee.move(right_speed, left_speed);
  }
}

/**
   ========================================================================================================================
   ledRED() - bright alert, GREEN change to RED.
*/
void ledRED() {
  int ValVermelho = 255;
  int ValAzul = 0;
  int ValVerde = 0;

  for (int i = 0 ; i < 255 ; i += 1) {
    ValVerde += 1;
    ValVermelho -= 1;

    /**
       In each cycle difference 255 - VALVERDE REDUCTION 255 led - ValVermelho Increases Calsando a gradual transition
       from GREEN to RED
    */
    analogWrite( VERDE, 255 - ValVerde );
    analogWrite( VERMELHO, 255 - ValVermelho );

    /* Waiting to see color */
    delay(10);
  }

}

/**
   ========================================================================================================================
   ledGREEN() - Bright Green Alert, BLUE change to GREEN.
*/
void ledGREEN() {
  int ValVermelho = 0;
  int ValAzul = 0;
  int ValVerde = 255;

  for (int i = 0 ; i < 255 ; i += 1) {
    ValAzul += 1;
    ValVerde -= 1;
    analogWrite( AZUL, 255 - ValAzul );
    analogWrite( VERDE, 255 - ValVerde );
    delay(10);
  }

}

/**
   ========================================================================================================================
    ledBLUE() - Bright Blue Alert, RED change to BLUE.
*/
void ledBLUE() {
  int ValVermelho = 0;
  int ValAzul = 255;
  int ValVerde = 0;

  for ( int i = 0 ; i < 255 ; i += 1 ) {
    ValVermelho += 1;
    ValAzul -= 1;
    analogWrite( VERMELHO, 255 - ValVermelho );
    analogWrite( AZUL, 255 - ValAzul );
    delay(10);
  }

}

/**
   ========================================================================================================================
   ledOFF() - Led off.
*/
void ledOFF() {
  digitalWrite(VERDE, LOW);
  digitalWrite(AZUL, LOW);
  digitalWrite(VERMELHO, LOW);
}

/**
   ========================================================================================================================
   printCalibration - Shows calibration result, MAX and MIN values.
*/
void printCalibration() {

  Serial.println(">>>>>| Calibração de Sensores IR. |<<<<<");

  /* Displays MIN valore obtained. */
  for (int i = 0; i < QUANT_SENSORS; i++) {
    Serial.print(qtra.calibratedMinimumOn[i]);
    Serial.print(' ');
  }
  Serial.println();

  /* Displays MAX obitidos values. */
  for (int i = 0; i < QUANT_SENSORS; i++) {
    Serial.print(qtra.calibratedMaximumOn[i]);
    Serial.print(' ');
  }
  Serial.println();
  Serial.println("---------------------------------------");
}

