![Logo](#)

Robot Bumblebee Library Program

![img]()

Especificações:


## Instalação

1. "Clone Download": ;
2. Descompacte e modifique o nome da pasta para: "bumblebee" (Remova o '-master');
3. Importe-a para sua pasta Library da sua IDE Arduino ou o diretório sketchbooks do Arduíno software;
4. Restart Arduino IDE.

## Sintáxe
```Arduino 
// Include lib.
#include <BUMBLEBEE.h>
```

## Sintáxe
```Arduino 
 // Sets pins where they are connected to the motors in arduino
 #define pin1MotorLeft 2
 #define pin2MotorLeft 4
 #define pinSpeedMotorLeft 3 //PWM
 #define pin1MotorRight 6
 #define pin2MotorRight 7
 #define pinSpeedMotorRight 5 //PWM

 /**
  * Instantiate an object of class BUMBLEBEE, passing parameters the connecting pins
  * where this on each pin of the H bridge L298N in Arduino doors.
  */ 
 BUMBLEBEE bee(pin1MotorLeft, pin2MotorLeft, pinSpeedMotorLeft, pin1MotorRight, pin2MotorRight, pinSpeedMotorRight);
```

```Arduino
// engine connecting through switch positive value towards clockwise rotation and 100 -> value of the rotation speed.
bee.move(100, 100);

// call parameter passing engine negative direction of clockwise rotation and anti -100 -> value of the rotation speed.
bee.move(-100, -100);

// off motor through parameter zero. 
bee.move(0, 0);

// off an engine through parameter zero.
bee.move(100, 0);

// off an engine through parameter zero.
bee.move(0, 100);

// off motor without parameter passing.
bee.stop();
```

## Examples
Here are some examples of how everything is more simple and streamlined:

## Reference Library

## Xredits
