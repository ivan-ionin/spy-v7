#include <Servo.h>

// LED подсветка
#define LED_PIN 13;

// Грузовой отсек
#define SERVO_CARGO_L_PIN 2;
#define SERVO_CARGO_R_PIN 3;

// Руль высоты
#define SERVO_ELEVATOR_PIN 4;

// Руль поворота
#define SERVO_CHATTER_PIN 5;

// Крылья
#define SERVO_WING_L_PIN 6;
#define SERVO_WING_R_PIN 7;

// Сервоприводы
Servo servoCargoL;
Servo servoCargoR;
Servo servoElevator;
Servo servoChatter;
Servo servoWingL;
Servo servoWingR;

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
  Serial.begin(9600);
  
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
  
}

// Итерация
void loop() {
  
}

void servosToStartPositions() {
  cargoHoldClose();
  elevatorToNormalPosition();
  chatterToNormalPosition();
  wingsToNormalPosition();
}

// Открыть грузовой отсек
void cargoHoldOpen() {
  servoCargoL.write(servoValueCargoL_opened);
  servoCargoR.write(servoValueCargoR_opened);
  Serial.println("Cargo hold open");
}

// Закрыть грузовой отсек
void cargoHoldClose() {
  servoCargoL.write(servoValueCargoL_closed);
  servoCargoR.write(servoValueCargoR_closed);
  Serial.println("Cargo hold close");
}

// Руль высоты в нулевую позицию
void elevatorToNormalPosition() {
  servoElevator.write(servoValueElevatorZero);
  Serial.println("Elevator to normal position");
}

// Руль высоты, поворот вверх
void elevatorToTurnUp() {
  servoElevator.write(servoValueElevatorZero + servoValueElevatorDelta);
  Serial.println("Elevator to turn up");
}

// Руль высоты, поворот вниз
void elevatorToTurnDown() {
  servoElevator.write(servoValueElevatorZero - servoValueElevatorDelta);
  Serial.println("Elevator to turn up");
}

// Руль рысканья в нулевую позицию
void chatterToNormalPosition() {
  servoChatter.write(servoValueChatterZero);
  Serial.println("Chatter to normal position");
}

// Руль рысканья, поворот влево
void chatterToTurnLeft() {
  servoChatter.write(servoValueChatterZero + servoValueChatterDelta);
  Serial.println("Chatter to turn left");
}

// Руль рысканья, поворот вправо
void chatterToTurnRight() {
  servoChatter.write(servoValueChatterZero - servoValueChatterDelta);
  Serial.println("Chatter to turn right");
}

// Крылья в нулевую позицию
void wingsToNormalPosition() {
  servoWingL.write(servoValueWingLZero);
  servoWingR.write(servoValueWingRZero);
  Serial.println("Wings to normal position");
}

// Крылья для поворота влево
void wingsToTurnLeft() {
  servoWingL.write(servoValueWingLZero + servoValueWingsDelta);
  servoWingR.write(servoValueWingRZero - servoValueWingsDelta);
  Serial.println("Wings to turn left");
}

// Крылья для поворота вправо
void wingsToTurnRight() {
  servoWingL.write(servoValueWingLZero - servoValueWingsDelta);
  servoWingR.write(servoValueWingRZero + servoValueWingsDelta);
  Serial.println("Wings to turn right");
}
