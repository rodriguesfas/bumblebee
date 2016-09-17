/**
   ==========================================================================================
   @ Software: Bumblebee_Simple_Line_Follower
   @ Description: Simple Line follower
   @ Version: 0.0.1
   @ Data: 14 de setember de 2016
   @ Developer Rodrigues F. A. S.
   @ Site: rodriguesfas.com.br
   @ E-mail: franciscosouzaacer@gmail.com
   @ Translate from Portuguese to English with Google Translate
   @ Website Lib. QTRSensors.h: <https://github.com/pololu/qtr-sensors-arduino/releases>
   ==========================================================================================
*/
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
  actionMotor();
}

/**
  ========================================================================================================================

   actionMotor -

*/
void actionMotor() {
  int value[5];
  qtra.readLine(sensorValues);

  for (int x = 0; x < 5; x++) {
    value[x] = sensorValues[x];
  }

  /**
     ==== Status sensor line ====
     [0] BRANCO
     [1] PRETO

     ==== ID sensor line ====
     [R] sensor line da direita
     [C] sensor line do centro
     [L] sensor line da esquerda

     ===== TABLE ====
     R0 R0 C0 L0 L0 = move GO 0
     R0 R0 C1 L0 L0 = move GO 1
     R0 R1 C0 L0 L0 = move RIGHT 1
     R1 R1 C0 L0 L0 = move RIGHT 2
     R1 R0 C0 L0 L0 = move RIGHT 3
     R1 R1 C1 L0 L0 = Err
     R1 R1 C1 L1 L0 = Err
     R1 R1 C1 L1 L1 = STOP
     R0 R0 C0 L1 L0 = move LEFT 1
     R0 R0 C0 L1 L1 = move LEFT 2
     R0 R0 C0 L0 L1 = move LEFT 3
     R0 R0 C1 L1 L1 = Err
     R0 R1 C1 L1 L1 = Err

  */

  if (value[0] == 0 && value[1] == 0 && value[2] == 1 && value[3] == 0 && value[4] == 0) { /* move GO 0 */
    bee.move(100, 100);
  }
  else if (value[0] == 0 && value[1] == 0 && value[2] == 1 && value[3] == 0 && value[4] == 0) { /* move GO 1 */
    bee.move(80, 80);
  }
  /******************************************************************************************************************/
  else if (value[0] == 0 && value[1] == 1 && value[2] == 0 && value[3] == 0 && value[4] == 0) { /* move RIGHT 1 */
    bee.move(100, 80);
  }
  else if (value[0] == 1 && value[1] == 1 && value[2] == 0 && value[3] == 0 && value[4] == 0) { /* move RIGHT 2 */
    bee.move(120, 80);
  }
  else if (value[0] == 0 && value[1] == 0 && value[2] == 0 && value[3] == 0 && value[4] == 0) { /* move RIGHT 3 */
    bee.move(150, 80);
  }
  /******************************************************************************************************************/
  else if (value[0] == 0 && value[1] == 0 && value[2] == 0 && value[3] == 1 && value[4] == 0) { /* move LEFT 1 */
    bee.move(100, 80);
  }
  else if (value[0] == 0 && value[1] == 0 && value[2] == 0 && value[3] == 1 && value[4] == 1) { /* move LEFT 2 */
    bee.move(120, 80);
  }
  else if (value[0] == 0 && value[1] == 0 && value[2] == 0 && value[3] == 0 && value[4] == 1) { /* move LEFT 3 */
    bee.move(150, 80);
  }
  /******************************************************************************************************************/
  else if (value[0] == 1 && value[1] == 1 && value[2] == 1 && value[3] == 1 && value[4] == 1) { /* STOP */
    bee.stop();
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

