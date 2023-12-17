#include <Keyboard.h>
#include <Mouse.h>

const int pin1 = 4; // Pin del número 1
const int pin2 = 6; // Pin del número 2
const int pin3 = 7; // Pin del número 3
const int pin4 = 8; // Pin del número 4
const int pin5 = 9; // Pin del número 5
const int joystickVRx1 = A0;
const int joystickVRy1 = A1;
const int joystickButton1 = 5;
unsigned long lastTime = 5;

void setup() {
  Serial.begin(115200);
  pinMode(pin1, INPUT_PULLUP);
  pinMode(pin2, INPUT_PULLUP);
  pinMode(pin3, INPUT_PULLUP);
  pinMode(pin4, INPUT_PULLUP);
  pinMode(pin5, INPUT_PULLUP);
  pinMode(joystickButton1, INPUT_PULLUP);
  Keyboard.begin();
  Mouse.begin();
}

void loop() {
  if ((millis() - lastTime) > 5) {
    joystickMovementControl(joystickVRx1, joystickVRy1, joystickButton1);
    lastTime = millis();
  }
  checkKeypad(pin1, '1', 0);
  checkKeypad(pin2, '2', 1);
  checkKeypad(pin3, '3', 2);
  checkKeypad(pin4, '4', 3);
  checkKeypad(pin5, '5', 4);
}

void joystickMovementControl(int joystickvrx, int joystickvry, int joystickbutton) {
  int VRxlecture = analogRead(joystickvrx);
  int VRylecture = analogRead(joystickvry);

  static int lastVRx = 0;
  static int lastVRy = 0;

  // Zona muerta para el joystick (centro +/- 50)
  if (abs(VRxlecture - 512) < 50) {
    VRxlecture = 512;
  }
  if (abs(VRylecture - 512) < 50) {
    VRylecture = 512;
  }

  // CONTROL DEL EJE Y (Hacia delante o hacia atrás)
  if (VRylecture > 550 && VRylecture != lastVRy) {
    Keyboard.write('U'); // Indica movimiento hacia arriba
    lastVRy = VRylecture;
  } else if (VRylecture < 400 && VRylecture != lastVRy) {
    Keyboard.write('D'); // Indica movimiento hacia abajo
    lastVRy = VRylecture;
  }

  // CONTROL DEL EJE X (Izquierda o derecha)
  if (VRxlecture > 550 && VRxlecture != lastVRx) {
    Keyboard.write('L'); // Indica movimiento hacia la izquierda
    lastVRx = VRxlecture;
  } else if (VRxlecture < 400 && VRxlecture != lastVRx) {
    Keyboard.write('R'); // Indica movimiento hacia la derecha
    lastVRx = VRxlecture;
  }
}

void checkKeypad(int pin, char key, int index) {
  bool currentState = digitalRead(pin);
  
  static bool keyState[] = {true, true, true, true, true}; // Estado inicial de las teclas

  if (currentState != keyState[index]) {
    if (currentState == LOW) {
      Keyboard.press(key);
    } else {
      Keyboard.release(key);
    }
    keyState[index] = currentState;
    delay(50); // Delay para debounce
  }
}
