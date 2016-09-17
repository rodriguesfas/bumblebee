/**
  ==========================================================================================
  @ Software: Bumblebee_Line_Follower_PID
  @ Description: Line follower pid in Scren OLed U8GLIB_SSD1306_128X64
  @ Version: 0.0.1
  @ Data: 15 de setember de 2016
  @ Developer Rodrigues F. A. S.
  @ Site: rodriguesfas.com.br
  @ E-mail: franciscosouzaacer@gmail.com
  @ Translate from Portuguese to English with Google Translate
  @ Website Lib. U8glib.h: <https://github.com/olikraus/u8glib>
  @              QTRSensors.h: <https://github.com/pololu/qtr-sensors-arduino/releases>
  @ Website Tutorial: <http://blog.filipeflop.com/display/como-conectar-display-oled-arduino.html>
  @                   <http://blog.filipeflop.com/sensores/bafometro-com-arduino.html>
  ==========================================================================================
*/

/* importe da lib. */
#include <BUMBLEBEE.h>
#include <QTRSensors.h>
#include <U8glib.h>

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

int last_proportional;
int integral;

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);  // Display which does not send ACK

/* config button */
#define pinButton 12
int statusButton = 0;
int inputButton = 0;
int status = 0;

boolean statusU8G = false;

int option = 0;

/**
  ========================================================================================================================
  configU8G -
*/
void configU8G() {
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255, 255, 255);
  }
}

/**
  ========================================================================================================================
  calibration - Performs the necessary instructions to calibrate the IR sensor.
*/
void calibration() {
  startU8G();
  
  /* Move robo in turning around themselves, for sensor calibration. Note .: Position the robot on the line. */
  bee.move(-115, 115);

  /* Performs calibration in seconds. (Approximately 10s) */
  for (int i = 0; i < 100; i++) { //400
    qtra.calibrate(); /* Read all the sensors 10 times 2.5ms. (25ms for each call). */
  }
  bee.stop();
}

/**
  ========================================================================================================================
  setup - configuration initial.
*/
void setup() {
  Serial.begin(9600);
  pinMode(pinButton, INPUT);
  configU8G();
  calibration();
}

/**
  ========================================================================================================================
  loop -
*/
void loop() {
  inputButton = digitalRead(pinButton);

  if (inputButton == HIGH && statusButton == LOW) { // our statusButton == HIGH
    status = 1 - status;
    delay(20);
  }
  statusButton = inputButton;
  
  if(status == 1){
    option = 1;
    startU8G();
    statusU8G = false;
    //Serial.println("OFF line");
  }
  else {
    option = 2;
    if(statusU8G == false) startU8G();
    linePID();
    //Serial.println("ON line");
  }
}

/**
  ========================================================================================================================
  startU8G -
*/
void startU8G() {
  u8g.firstPage();

  do {
    switch (option) {
      case 0: // read sensor value
        bee.stop();
        drawU8GCalibration();
        break;
      case 1:
        drawU8GReadSensorValue();
        break;
      case 2:
        drawU8GFollowerLine();
        statusU8G = true;
        break;
    }
  } while (u8g.nextPage());
}

/**
 * ======================================================================================================================== 
 * linePID - 
 *
 * BRANCO tende a 0
 * PRETO tende a 1024
 */
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
     M1 - M2. If a positive number, the robot will turn to
     Right. If it is a negative number, the robot will turn left
     And magnetude of numbers determine the acuteness with which will make the curves / turns
  */
  int power_difference = proportional / 10 + integral / 10000 + derivative * 3 / 2;

  /**
    Calculates the current configuration of the engines. We will never set
     A motor with a negative value
  */
  const int max = 140;

  if (power_difference > max) power_difference = max;
  if (power_difference < -max) power_difference = -max;

  if (power_difference < 0)
    controlMotors(max + power_difference, max);
  else
    controlMotors(max, max - power_difference);

}

/**
  ========================================================================================================================
  controlMotors - Driving the motors
*/
void controlMotors(int left_speed, int right_speed) {
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
  drawU8GReadSensorValue - Shows calibration result, MAX and MIN values.
*/
void drawU8GReadSensorValue() {

  u8g.setFont(u8g_font_8x13B);
  u8g.drawStr(5, 15, "Read sensor");//number cont sensor
   
  int value[5];
  qtra.readLine(sensorValues);

  for (int x = 0; x < 5; x++) value[x] = sensorValues[x];

  for (int x = 10; x <= 20; x++)u8g.drawVLine(x, map(sensorValues[0], 90, 950, 100, 30), 100); //graphic sensor 1
  for (int x = 32; x <= 42; x++)u8g.drawVLine(x, map(sensorValues[1], 90, 950, 100, 30), 100); //graphic sensor 2
  for (int x = 57; x <= 67; x++)u8g.drawVLine(x, map(sensorValues[2], 90, 950, 100, 30), 100); //graphic sensor 3
  for (int x = 82; x <= 92; x++)u8g.drawVLine(x, map(sensorValues[3], 90, 950, 100, 30), 100); //graphic sensor 4
  for (int x = 107; x <= 117; x++)u8g.drawVLine(x, map(sensorValues[4], 90, 950, 100, 30), 100); //graphic sensor 5

      drawRFrame();
      drawSensor();

}

/**
  ========================================================================================================================
  drawU8GCalibration - Shows calibration result, MAX and MIN values.
*/
void drawU8GCalibration() {
      u8g.setFont(u8g_font_8x13B);
      u8g.drawStr(5, 15, "Bumblebee PID");
      u8g.drawStr(10, 57, "Calibrating..");

      drawRFrame();
      drawSensor();

}

/**
  ========================================================================================================================
  drawU8GFollowerLine - Shows calibration result, MAX and MIN values.
*/
void drawU8GFollowerLine(){
  u8g.setFont(u8g_font_8x13B);
  u8g.drawStr(5, 15, "Bumblebee PID");
  u8g.drawStr(5, 57, "Follower Line..");

      drawRFrame();
      drawSensor();
}

/**
  ========================================================================================================================
  drawSensor
*/
void drawSensor(){
        //number cont sensor
      u8g.setFont(u8g_font_5x7);
      u8g.drawStr(13, 28, "1");
      u8g.drawStr(36, 28, "2");
      u8g.drawStr(60, 28, "3");
      u8g.drawStr(86, 28, "4");
      u8g.drawStr(110, 28, "5");
}
/**
  ========================================================================================================================
  drawRFrame
*/
void drawRFrame(){
  //Watch frame
  u8g.drawRFrame(0, 18, 128, 46, 4);

  //battery design
  u8g.drawRFrame(105, 3, 20, 12 , 0);
  u8g.drawBox(125, 6, 2, 6);
  u8g.drawBox(107, 5, 4, 8);
  u8g.drawBox(114, 5, 4, 8);

}

/**
  ========================================================================================================================
  printSerialMonitorCalibration - Shows calibration result, MAX and MIN values.
*/
void printSerialMonitorCalibration() {

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
