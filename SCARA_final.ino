#include <AccelStepper.h>
#include <Servo.h>
#include <math.h>
#include <ArduinoJson.h>

#define _CRT_SECURE_NO_WARNINGS

// Задаем пины для концевиков
#define limitSwitch1 11
#define limitSwitch2 10
#define limitSwitch3 9
#define limitSwitch4 A3

// Задаем пины для двигателей
AccelStepper stepper1(1, 2, 5); // (Type:driver, STEP, DIR)
AccelStepper stepper2(1, 3, 6);
AccelStepper stepper3(1, 4, 7);
AccelStepper stepper4(1, 12, 13);

// Создаем экзмепляр серводвигателя
Servo gripperServo; 

int point[4];

// Задаем количество переменных у образцов
int semple_list[25][4] = {
                  {-1400, 3500, 2500, 17200},
                  {-2800, 3700, 2920, 17000},
                  {-3000, 2000, 2920, 17000}
                  };


//Задаем команды
char command_start[] = "start";

int i;
int count;
int count_circles;



//-------------------------------НАСТРОЙКА ПРОГРАММЫ----------------------------------------


//Задаем начальное количество семплов
const int semples = 2; 

//Задаем начальное количество циклов
int circles[semples] = {
  
  2,
  1
  
};

//Задаем рецепты образцов
int recipe[semples][4] = {
 
 {2,1,2,3},
 {1,2,0,0}
 
 };

//Задаем время для каждого стакана
int tube_time[semples][20][2] = {

{{60,10},{60,120},{60,10},{100,120}},
{{120,10},{100,120},{0,0},{0,0}}
                          
 };

 //----------------------------------НАСТРОЙКА ЗАКОНЧИЛАСЬ---------------------------------------------------

//Начинаем настройку робота
void setup() {
  Serial.begin(115200); //Определяем мост через который будем общаться
  
  //Определяем pinMode для концевиков
  pinMode(limitSwitch1, INPUT_PULLUP); 
  pinMode(limitSwitch2, INPUT_PULLUP);
  pinMode(limitSwitch3, INPUT_PULLUP);
  pinMode(limitSwitch4, INPUT_PULLUP);

  // Задаем максимальную скорость для двигателей
  stepper1.setMaxSpeed(6000);
  stepper1.setAcceleration(3000);
  stepper2.setMaxSpeed(6000);
  stepper2.setAcceleration(3000);
  stepper3.setMaxSpeed(6000);
  stepper3.setAcceleration(3000);
  stepper4.setMaxSpeed(6000);
  stepper4.setAcceleration(3000);

  // Задаем пин для сероводвигателя
  gripperServo.attach(A0);

  gripperServo.write(175);

}

//Робот начинает работу
void loop() {
   //Старт программы
   start_programm();
}

void start_programm(){
  //Проверяем поступает ли что-то на порт
   if (Serial.available() > 0) {
    //Ловим необходимое слово
    if (Serial.find(command_start)) {
      homing();
     for (i = 0; i < semples; i++) {
     count_circles = circles[i];
     Serial.print("Для образца ");
     Serial.println(i+1);
     Serial.print("Получаем такое количество циклов: ");
     Serial.println(count_circles);
     
     Serial.println("Начало работы");
     //Начало работы алгоритма
     work();
     }
    }
   }
  }
 

//Калибровка
void homing() {
  Serial.println("Калибровка");
  
  // Калибровка Stepper4
  int limitSwitch4Val = digitalRead(limitSwitch4);
  while (limitSwitch4Val != 1) {
    stepper4.setSpeed(1500);
    stepper4.runSpeed();
    stepper4.setCurrentPosition(17000); // When limit switch pressed set position to 0 steps
    limitSwitch4Val = digitalRead(limitSwitch4);
    delay(1);
    
  }
  delay(20);
  stepper4.moveTo(15000);
  while (stepper4.currentPosition() != 15000) {
    stepper4.run();
  }

  // Калибровка Stepper3
  int limitSwitch3Val = digitalRead(limitSwitch3);
  while (limitSwitch3Val != 1) {
    stepper3.setSpeed(-1100);
    stepper3.runSpeed();
    stepper3.setCurrentPosition(-1662); // When limit switch pressed set position to 0 steps
    limitSwitch3Val = digitalRead(limitSwitch3);
    delay(1);
  }
  delay(20);
  stepper3.moveTo(0);
  while (stepper3.currentPosition() != 0) {
    stepper3.run();
  }

  // Калибровка Stepper2
  int limitSwitch2Val = digitalRead(limitSwitch2);
  while (limitSwitch2Val != 1) {
    stepper2.setSpeed(-1300);
    stepper2.runSpeed();
    stepper2.setCurrentPosition(-5420); // When limit switch pressed set position to -5440 steps
    limitSwitch2Val = digitalRead(limitSwitch2);
    delay(1);
  }
  delay(20);
  stepper2.moveTo(0);
  while (stepper2.currentPosition() != 0) {
    stepper2.run();
  }

  // Калибровка Stepper1
  int limitSwitch1Val = digitalRead(limitSwitch1); 
  while (limitSwitch1Val != 1) {
    stepper1.setSpeed(-1200);
    stepper1.runSpeed();
    stepper1.setCurrentPosition(-3955); // When limit switch pressed set position to 0 steps
    limitSwitch1Val = digitalRead(limitSwitch1);
    delay(1);
  }
  delay(20);
  stepper1.moveTo(0);
  while (stepper1.currentPosition() != 0) {
    stepper1.run();
  }
}

void goToSteps(int s1, int s2, int s3, int s4)
{
  stepper1.moveTo(s1);
  stepper2.moveTo(s2);
  stepper3.moveTo(s3);
  stepper4.moveTo(s4);

  while (stepper1.currentPosition() != s1 || stepper2.currentPosition() != s2 || stepper3.currentPosition() != s3 || stepper4.currentPosition() != s4) {
    stepper1.run();
    stepper2.run();
    stepper3.run();
    stepper4.run();
  }
}


void work() {
  start_prog(i);
  
  int tube_recipe = i;
  
  for (int b=0; b < count_circles; b++) {
    Serial.print("Цикл " + String(b+1));
      for (int e=0; e < 4; e++) {
        int tube = recipe[tube_recipe][e];
        int into_tube = tube_time[tube_recipe][e][0];
        int dry_tube = tube_time[tube_recipe][e][1];
          switch(tube) {
            case 0:
              break;
          
           case 1:
            tube1(into_tube,dry_tube);
            break;
        
          case 2:
            tube2(into_tube,dry_tube);
            break;

          case 3:
            tube3(into_tube,dry_tube);
            break;
        }  
      }
  }
  finished(i);
}


void start_prog(int semple) {
  Serial.println("Старт программы");
  gripperServo.write(175);
  goToSteps(semple_list[semple][0],semple_list[semple][1], semple_list[semple][2], semple_list[semple][3]);

  point[3] = -10000;
  goToSteps(semple_list[semple][0],semple_list[semple][1], semple_list[semple][2], point[3]);
  gripperServo.write(0);

  point[3] = 17500;
  goToSteps(semple_list[semple][0],semple_list[semple][1], semple_list[semple][2], point[3]);
  
}

void tube1(int into_tube, int dry_tube) {  
  point[0] = 1200;
  point[1] = 2900;
  point[2] = 2920;
  goToSteps(point[0], point[1], point[2], point[3]);

  point[3] = 0;
  goToSteps(point[0], point[1], point[2], point[3]);
  for(count = 0; count < into_tube ; count++)
  {
    delay(1000);
  }

  point[3] = 17000;
  goToSteps(point[0], point[1], point[2], point[3]);

  for(count = 0; count < dry_tube ; count++)
  {
    delay(1000);
  }
}

void tube2(int into_tube, int dry_tube) {
  point[0] = 3200;
  point[1] = 2700;
  goToSteps(point[0], point[1], point[2], point[3]);

  point[3] = 0;
  goToSteps(point[0], point[1], point[2], point[3]);
  for(count = 0; count < into_tube ; count++)
  {
    delay(1000);
  }

  point[3] = 17000;
  goToSteps(point[0], point[1], point[2], point[3]);

  for(count = 0; count < dry_tube ; count++)
  {
    delay(1000);
  }
}

void tube3(int into_tube, int dry_tube) {
  point[0] = 5200;
  point[1] = 2500;
  goToSteps(point[0], point[1], point[2], point[3]);

  point[3] = 0;
  goToSteps(point[0], point[1], point[2], point[3]);
  for(count = 0; count < into_tube ; count++)
  {
    delay(1000);
  }

  point[3] = 17000;
  goToSteps(point[0], point[1], point[2], point[3]);

  for(count = 0; count < dry_tube ; count++)
  {
    delay(1000);
  }
}



void finished(int semple) {
  goToSteps(semple_list[semple][0],semple_list[semple][1], semple_list[semple][2], semple_list[semple][3]);

  point[3] = -10000;
  goToSteps(semple_list[semple][0],semple_list[semple][1], semple_list[semple][2], point[3]);

  gripperServo.write(180);

  point[3] = 17000;
  goToSteps(semple_list[semple][0],semple_list[semple][1], semple_list[semple][2], point[3]);

  gripperServo.write(0);
  
  homing();
  Serial.println("Закончил работу");
}







// Математика, которая потом, возможно, понадобится
//double x = 10.0;
//double y = 10.0;
//double L1 = 228; // L1 = 228mm
//double L2 = 136.5; // L2 = 136.5mm
//double theta1, theta2, phi, z;
//
//int stepper1Position, stepper2Position, stepper3Position, stepper4Position;
//
//const float theta1AngleToSteps = 44.444444;
//const float theta2AngleToSteps = 35.555555;
//const float phiAngleToSteps = 10;
//const float zDistanceToSteps = 100;
//
//byte inputValue[5];
//int k = 0;
//
//String content = "";
//int data[10];
//
//int theta1Array[100];
//int theta2Array[100];
//int phiArray[100];
//int zArray[100];
//int gripperArray[100];
//int positionsCounter = 0;
