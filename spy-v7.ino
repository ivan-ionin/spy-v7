#include <Servo.h>

// LED подсветка
#define LED_PIN 13

// Грузовой отсек
#define SERVO_CARGO_L_PIN 2
#define SERVO_CARGO_R_PIN 3

// Руль высоты
#define SERVO_ELEVATOR_PIN 4

// Руль поворота
#define SERVO_CHATTER_PIN 5

// Крылья
#define SERVO_WING_L_PIN 6
#define SERVO_WING_R_PIN 7

// Сервоприводы
Servo servoCargoL;
Servo servoCargoR;
Servo servoElevator;
Servo servoChatter;
Servo servoWingL;
Servo servoWingR;

// Отладка [!!!!!] снять флаг при боевой прошивке
boolean stateDebug = true;

// Состояние готовности к реакции на управление
boolean stateReady = false;

// Количество шагов тестирования и задержка положения
int testCount = 10;
int testDelay = 1000;

// Границы движения сервоприводов
int servoValueCargoL_closed = 10;       // Грузовой отсек закрыт, левая серва
int servoValueCargoL_opened = 170;      // Грузовой отсек открыт, левая серва
int servoValueCargoR_closed = 10;       // Грузовой отсек закрыт, правая серва
int servoValueCargoR_opened = 170;      // Грузовой отсек открыт, правая серва
int servoValueElevatorZero = 0;         // Руль высоты, нулевая позиция
int servoValueElevatorDelta = 45;       // Руль высоты, дельта смещения
int servoValueChatterZero = 0;          // Руль рысканья, нулевая позиция
int servoValueChatterDelta = 45;        // Руль рысканья, дельта смещения
int servoValueWingLZero = 0;            // Левое крыло, нулевая позиция
int servoValueWingRZero = 0;            // Правое крыло, нулевая позиция
int servoValueWingsDelta = 45;          // Крылья, дельта смещения

// Инициализация
void setup() {
  
  // Инициализация порта
  if (stateDebug) {
    Serial.begin(9600);
  }
  
  // Инициализация led-подсветки
  pinMode(LED_PIN, OUTPUT);
  
  // Инициализация сервоприводов
  servoCargoL.attach(SERVO_CARGO_L_PIN);
  servoCargoR.attach(SERVO_CARGO_R_PIN);
  servoElevator.attach(SERVO_ELEVATOR_PIN);
  servoChatter.attach(SERVO_CHATTER_PIN);
  servoWingL.attach(SERVO_WING_L_PIN);
  servoWingR.attach(SERVO_WING_R_PIN);
  
  // Установка стартовых положений сервоприводов
  servosToStartPositions();
  
  // Задержка перед началом тестирования системы
  delay(5000);
  
  // Тестирование системы
  tests();

  // Повторная установка стартовых положений сервоприводов
  servosToStartPositions();
}

// Итерация
void loop() {
  if (stateReady) {
    /* TODO */
  }
}

// Отладка
void debug(String str) {
  if (stateDebug) {
    Serial.println(str);
  }
}

// Тестирование
void tests() {
  for (int i = 0; i < testCount; i++) {
    cargoHoldOpen();
    elevatorToTurnUp();
    chatterToTurnLeft();
    wingsToTurnLeft();
    ledLightTurnOff();
    delay(testDelay / 2);
    cargoHoldClose();
    elevatorToTurnDown();
    chatterToTurnRight();
    wingsToTurnRight();
    ledLightTurnOn();
    delay(testDelay / 2);
  }
  debug("Tests completed");
  for (int i = 0; i <= 20; i++) {
    if (!((i + 1) % 2)) {
      ledLightTurnOff();
    } else {
      ledLightTurnOn();
    }
    delay(200);
  }
  stateReady = true;
  debug("Ready to fly");
}

// Стартовое состояние
void servosToStartPositions() {
  cargoHoldClose();
  elevatorToNormalPosition();
  chatterToNormalPosition();
  wingsToNormalPosition();
  ledLightTurnOn();
}

// Открыть грузовой отсек
void cargoHoldOpen() {
  servoCargoL.write(servoValueCargoL_opened);
  servoCargoR.write(servoValueCargoR_opened);
  debug("Cargo hold open");
}

// Закрыть грузовой отсек
void cargoHoldClose() {
  servoCargoL.write(servoValueCargoL_closed);
  servoCargoR.write(servoValueCargoR_closed);
  debug("Cargo hold close");
}

// Руль высоты в нулевую позицию
void elevatorToNormalPosition() {
  servoElevator.write(servoValueElevatorZero);
  debug("Elevator to normal position");
}

// Руль высоты, поворот вверх
void elevatorToTurnUp() {
  servoElevator.write(servoValueElevatorZero + servoValueElevatorDelta);
  debug("Elevator to turn up");
}

// Руль высоты, поворот вниз
void elevatorToTurnDown() {
  servoElevator.write(servoValueElevatorZero - servoValueElevatorDelta);
  debug("Elevator to turn up");
}

// Руль рысканья в нулевую позицию
void chatterToNormalPosition() {
  servoChatter.write(servoValueChatterZero);
  debug("Chatter to normal position");
}

// Руль рысканья, поворот влево
void chatterToTurnLeft() {
  servoChatter.write(servoValueChatterZero + servoValueChatterDelta);
  debug("Chatter to turn left");
}

// Руль рысканья, поворот вправо
void chatterToTurnRight() {
  servoChatter.write(servoValueChatterZero - servoValueChatterDelta);
  debug("Chatter to turn right");
}

// Крылья в нулевую позицию
void wingsToNormalPosition() {
  servoWingL.write(servoValueWingLZero);
  servoWingR.write(servoValueWingRZero);
  debug("Wings to normal position");
}

// Крылья для поворота влево
void wingsToTurnLeft() {
  servoWingL.write(servoValueWingLZero + servoValueWingsDelta);
  servoWingR.write(servoValueWingRZero - servoValueWingsDelta);
  debug("Wings to turn left");
}

// Крылья для поворота вправо
void wingsToTurnRight() {
  servoWingL.write(servoValueWingLZero - servoValueWingsDelta);
  servoWingR.write(servoValueWingRZero + servoValueWingsDelta);
  debug("Wings to turn right");
}

void ledLightTurnOn() {
  digitalWrite(LED_PIN, HIGH);
  debug("Led light turn on");
}

void ledLightTurnOff() {
  digitalWrite(LED_PIN, LOW);
  debug("Led light turn off");
}
