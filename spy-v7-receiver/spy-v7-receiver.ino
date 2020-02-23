// Библиотеки
#include <SPI.h>
#include <Servo.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

// Определение роли
// transmitter / receiver
#define ROLE "receiver"

// Определение режима работы
// test / real
#define MODE "test"

// Определение режима тестирования кода
// true / false
#define DEBUG true

// Сигнальные трубы
// по ним приемник и передатчик
// будут общаться при включении в момент настройки и поиска канала связи
// должны быть указаны идентично на передатчике и приемнике
#define TX_PIPE 0;
#define RX_PIPE 1;

// Трубы
// определяются автоматически
// должны получить обратные значения на приемнике и передатчике
int t_pipe; // Сюда данный модуль будет передавать
int r_pipe; // Отсюда данный модуль будет читать

// Пины подключения NRF24L01+
RF24 radio(9, 10);

// LED подсветка
#define LED_PIN 2

// Грузовой отсек
#define SERVO_CARGO_L_PIN 4
#define SERVO_CARGO_R_PIN 3

// Руль высоты
#define SERVO_ELEVATOR_PIN 8

// Руль поворота
#define SERVO_RUDDER_PIN 7

// Крылья
#define SERVO_AILERON_L_PIN 6
#define SERVO_AILERON_R_PIN 5

// Сервоприводы
Servo servoCargoL;
Servo servoCargoR;
Servo servoElevator;
Servo servoRudder;
Servo servoAileronL;
Servo servoAileronR;

// Состояние готовности к реакции на управление
boolean readyToFly = false;

// Количество шагов тестирования и задержка положения
int testCount = 100;
int testDelay = 2000;

// Активация при тестировании
boolean ledTest = true;
boolean cargoTest = true;
boolean elevatorTest = true;
boolean rudderTest = true;
boolean aileronsTest = false;


// Границы движения сервоприводов

int servoValueCargoL_closed = 170;      // Грузовой отсек закрыт, левая серва
int servoValueCargoL_opened = 10;       // Грузовой отсек открыт, левая серва
int servoValueCargoR_closed = 10;       // Грузовой отсек закрыт, правая серва
int servoValueCargoR_opened = 170;      // Грузовой отсек открыт, правая серва

int servoValueElevatorZero = 90;        // Руль высоты, нулевая позиция
int servoValueElevatorToTurnUp = 0;     // Руль высоты, для полета вверх
int servoValueElevatorToTurnDown = 170;  // Руль высоты, для полета вниз

int servoValueRudderZero = 80;          // Руль поворота, нулевая позиция
int servoValueRudderToTurnLeft = 60;    // Руль поворота, для полета налево
int servoValueRudderToTurnRight = 110;  // Руль поворота, для полета направо

int servoValueAileronLZero = 90;        // Левое крыло, нулевая позиция
int servoValueAileronRZero = 90;        // Правое крыло, нулевая позиция
int servoValueAileronsDelta = 35;       // Крылья, дельта смещения

// Инициализация
void setup() {
  
  // Инициализация порта
  if (DEBUG) {
    Serial.begin(9600);
  }
  
  // Инициализация led-подсветки
  pinMode(LED_PIN, OUTPUT);
  
  // Инициализация сервоприводов
  servoCargoL.attach(SERVO_CARGO_L_PIN);
  servoCargoR.attach(SERVO_CARGO_R_PIN);
  servoElevator.attach(SERVO_ELEVATOR_PIN);
  servoRudder.attach(SERVO_RUDDER_PIN);
  servoAileronL.attach(SERVO_AILERON_L_PIN);
  servoAileronR.attach(SERVO_AILERON_R_PIN);
  
  // Установка стартовых положений сервоприводов
  servosToStartPositions();
  
  // Задержка перед началом тестирования системы
  delay(5000);
  
  // Тестирование системы
  if (MODE == "test") {
    debug("Test mode");
    tests();
    // Повторная установка стартовых положений сервоприводов
    servosToStartPositions();
  }

  // Готовность к полету
  readyToFly = true;
  debug("Ready to fly");
}

// Итерация
void loop() {
  if (readyToFly) {
    /* TODO */
  }
}

// Отладка
void debug(String str) {
  if (DEBUG) {
    Serial.println(str);
  }
}

// Тестирование
void tests() {
  for (int i = 0; i < testCount; i++) {
    debug(String(i + 1));
    if (cargoTest) cargoHoldOpen();
    if (elevatorTest) elevatorToTurnUp();
    if (rudderTest) rudderToTurnLeft();
    if (aileronsTest) aileronsToTurnLeft();
    if (ledTest) ledLightTurnOff();
    delay(testDelay / 2);
    if (cargoTest) cargoHoldClose();
    if (elevatorTest) elevatorToTurnDown();
    if (rudderTest) rudderToTurnRight();
    if (aileronsTest) aileronsToTurnRight();
    if (ledTest) ledLightTurnOn();
    delay(testDelay / 2);
  }
  debug("Tests completed");
  for (int i = 0; i <= 100; i++) {
    if (!((i + 1) % 2)) {
      ledLightTurnOff();
    } else {
      ledLightTurnOn();
    }
    delay(200);
  }
}

// Стартовое состояние
void servosToStartPositions() {
  cargoHoldClose();
  elevatorToNormalPosition();
  rudderToNormalPosition();
  aileronsToNormalPosition();
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
  servoElevator.write(servoValueElevatorToTurnUp);
  debug("Elevator to turn up");
}

// Руль высоты, поворот вниз
void elevatorToTurnDown() {
  servoElevator.write(servoValueElevatorToTurnDown);
  debug("Elevator to turn up");
}

// Руль рысканья в нулевую позицию
void rudderToNormalPosition() {
  servoRudder.write(servoValueRudderZero);
  debug("Rudder to normal position");
}

// Руль рысканья, поворот влево
void rudderToTurnLeft() {
  servoRudder.write(servoValueRudderToTurnLeft);
  debug("Rudder to turn left");
}

// Руль рысканья, поворот вправо
void rudderToTurnRight() {
  servoRudder.write(servoValueRudderToTurnRight);
  debug("Rudder to turn right");
}

// Крылья в нулевую позицию
void aileronsToNormalPosition() {
  servoAileronL.write(servoValueAileronLZero);
  servoAileronR.write(servoValueAileronRZero);
  debug("Ailerons to normal position");
}

// Крылья для поворота влево
void aileronsToTurnLeft() {
  servoAileronL.write(servoValueAileronLZero + servoValueAileronsDelta);
  servoAileronR.write(servoValueAileronRZero - servoValueAileronsDelta);
  debug("Ailerons to turn left");
}

// Крылья для поворота вправо
void aileronsToTurnRight() {
  servoAileronL.write(servoValueAileronLZero - servoValueAileronsDelta);
  servoAileronR.write(servoValueAileronRZero + servoValueAileronsDelta);
  debug("Ailerons to turn right");
}


// Крылья для взлета
void aileronsToTurnUp() {
  servoAileronL.write(servoValueAileronLZero - servoValueAileronsDelta);
  servoAileronR.write(servoValueAileronRZero - servoValueAileronsDelta);
  debug("Ailerons to turn right");
}

// Крылья для посадки
void aileronsToTurnDown() {
  servoAileronL.write(servoValueAileronLZero + servoValueAileronsDelta);
  servoAileronR.write(servoValueAileronRZero + servoValueAileronsDelta);
  debug("Aileronss to turn right");
}

void ledLightTurnOn() {
  digitalWrite(LED_PIN, HIGH);
  debug("Led light turn on");
}

void ledLightTurnOff() {
  digitalWrite(LED_PIN, LOW);
  debug("Led light turn off");
}
