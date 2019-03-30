//**************************************************DEF*********************************************
#include <SoftwareSerial.h>
#include <Wire.h>
SoftwareSerial BT(11, 10);
#include "Adafruit_VL53L0X.h"

#define XSHUT1 42
#define XSHUT2 46
#define XSHUT3 48

//Addresser för sensorerna, välj tal mellan 0x30 och 0x3F
const int Address1 = 0x30;    //FL
const int Address2 = 0x31;    //FR
const int Address3 = 0x32;    //BR

Adafruit_VL53L0X sensFL = Adafruit_VL53L0X();
Adafruit_VL53L0X sensFR = Adafruit_VL53L0X();
Adafruit_VL53L0X sensFF = Adafruit_VL53L0X();

VL53L0X_RangingMeasurementData_t FLmeasure;
VL53L0X_RangingMeasurementData_t FRmeasure;
VL53L0X_RangingMeasurementData_t FFmeasure;

int disFL = 0;
int disFR = 0;
int disFF = 0;
//-----------------------------
int logicPin3 = 4;
int logicPin4 = 5;
int logicPinReverse1 = 3;
int logicPinReverse2 = 6;
//-----------------------------
const int LED0 = 28;
const int LED1 = 30;
const int LED2 = 32;

const int RGB0 = 7;
const int RGB1 = 8;
const int RGB2 = 9;
//-----------------------------
const int pResistor = A14;
const int batteryVoltage = A10;
//-----------------------------



//-----------------------------
int speed1 = 100;
//-----------------------------
char number[50];
int BC = 0;
int BD = 0;
char BCommands[10] = {0};
int BCounter = 0;
//-----------------------------
String QR = "Q";
int TaskCounter = 0;
int DT = 0;
int DT2 = 0;
int onceInTick = 0;
String messageFromPI = "Q";
int StockholmV = 0;
int counter = 0;
int stopOnce = 0;
//-----------------------------

void setup() {
  Serial.println("STARTING SETUP");
  pinMode(batteryVoltage, INPUT);
  pinMode(pResistor, INPUT);
  Wire.begin();
  Serial.begin(115200);
  //------------------LED------------------
  pinMode(RGB0, OUTPUT); //gul
  pinMode(RGB1, OUTPUT);
  pinMode(RGB2, OUTPUT);
  pinMode(LED0, OUTPUT); //gul
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  digitalWrite(LED0, HIGH);
  //------------------SENSOR------------------
  pinMode(XSHUT1, OUTPUT);
  pinMode(XSHUT2, OUTPUT);
  pinMode(XSHUT3, OUTPUT);

  digitalWrite(XSHUT1, LOW);
  digitalWrite(XSHUT2, LOW);
  digitalWrite(XSHUT3, LOW);
  delay(10);
  digitalWrite(XSHUT1, HIGH);
  digitalWrite(XSHUT2, HIGH);
  digitalWrite(XSHUT3, HIGH);
  delay(10);
  digitalWrite(XSHUT2, LOW);
  digitalWrite(XSHUT3, LOW);
  delay(10);
  sensFL.begin(Address1);
  delay(10);
  digitalWrite(XSHUT2, HIGH);
  delay(10);
  sensFR.begin(Address2);
  digitalWrite(XSHUT3, HIGH);
  delay(10);
  sensFF.begin(Address3);
  delay(10);
  //------------------MOTOR------------------
  pinMode(logicPin3, OUTPUT);
  pinMode(logicPin4, OUTPUT);
  pinMode(logicPinReverse1, OUTPUT);
  pinMode(logicPinReverse2, OUTPUT);
  drivingStop();
  //------------------BLUETOOTH------------------
  BT.begin(9600);
  delay(500);
  BT.print("AT+NAMETHE-OSKAR-HC06");
  //BT.print("AT+NAMEJY-MCU-HC06");
  delay(500);
  Serial.println("SETUP DONE");
  digitalWrite(LED0, LOW);


}
//**************************************************MAIN*********************************************
void loop() {
  analogWrite(RGB2, 200);
  TaskCounter = 0;

//  while (true) {
//    counter = 0;
//    while (true) {
//
//      counter = tick(counter);
//
//    }
//  }
    //
//      BCommands[0] = 'G';
//      BCommands[1] = 'G';
//      BCommands[2] = 'A';
//      BCommands[3] = 'G';
//      BCommands[4] = 'A';
//      BCommands[5] = 'A';
//      BCommands[6] = 'A';
//      BCommands[7] = 'T';
//      BC = 1;
//      BD = 1;

    //  ----------------------PREP----------------------
    while ( BC == 0 ) {
      MakeConnection();
    }
    while (BD == 0 ) {
      ReadingTasks();
    }
    //  ----------------------PREP----------------------

    while (DT == 0 ) {
      FirstTime();
    }

    while (DT2 == 0) {
      digitalWrite(LED1, HIGH);
      if (BCommands[TaskCounter] == 'A') {
        executeFram();
      }
      else if (BCommands[TaskCounter] == 'C'  ) {
        executeHoger();
      }
      else if (BCommands[TaskCounter] == 'G' ) {
        executeVanster();
      }
      else if (BCommands[TaskCounter] == 'U') {
        executeUsvang();
      }
      if (BCommands[TaskCounter] == 'T' ) {
        executeT();
      }
    }
    DT2 = 0;
  }
  //**************************************************FUNC*********************************************
  void akutForbil() {
    readSensor(sensFF, FFmeasure, disFF);
    Serial.println(disFF);
    if (disFF < 300 && stopOnce == 0) {
      stopOnce = 1;
      drivingStop();
      analogWrite(RGB0, 0);
      analogWrite(RGB1, 0);
      analogWrite(RGB2, 0);
      digitalWrite(LED1, HIGH);
      analogWrite(RGB1, 200);
      delay(500);
      digitalWrite(LED1, LOW);
      analogWrite(RGB1, 0);
      delay(500);
      digitalWrite(LED1, HIGH);
      analogWrite(RGB1, 200);
      delay(500);
      digitalWrite(LED1, LOW);
      analogWrite(RGB1, 0);
      delay(500);
      digitalWrite(LED1, HIGH);
      analogWrite(RGB1, 200);
      delay(500);
      digitalWrite(LED1, LOW);
      analogWrite(RGB1, 0);
    }
  }

  void executeT() {
    digitalWrite(LED0, HIGH);
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED1, HIGH);
    analogWrite(RGB0, 200);
    delay(1000);
    analogWrite(RGB0, 0);
    delay(1000);
    analogWrite(RGB0, 200);
    delay(1000);
    analogWrite(RGB0, 0);
    delay(1000);
    analogWrite(RGB0, 200);
    delay(1000);
    analogWrite(RGB0, 0);
    digitalWrite(LED0, LOW);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);

    BD = 0;
    DT = 1;
    DT2 = 1;
    TaskCounter = 0;
    BCounter = 0;
  }

  void executeUsvang() {
    stopOnce = 0;
    int loopForOnce = 0;
    int stopMoreThenOne = 0;
    int thresh = 110;
    int upper = 500;
    int upper2 = 800;
    int extra = 7;

    while (loopForOnce == 0) {
      readSensor(sensFR, FRmeasure, disFR);
      readSensor(sensFL, FLmeasure, disFL);
      if (((disFL > upper2) && stopMoreThenOne == 0 ) || ((disFR > upper) && (stopMoreThenOne == 0 ))) {
        digitalWrite(LED0, HIGH);
        int counter = 0;
        while (counter < 24) {
          drivingHardRight();
          counter = tick(counter);
        }
        counter = 0;
        while (counter < 7) {
          drivingFram();
          counter = tick(counter);
        }
        counter = 0;
        while (counter < 10) {
          drivingHardRight();
          counter = tick(counter);
        }
        readSensor(sensFR, FRmeasure, disFR);
        while (disFR > 300 ) {
          readSensor(sensFR, FRmeasure, disFR);
          drivingNew3();
        }
        digitalWrite(LED0, LOW);
        drivingStop();
        stopMoreThenOne = 1;
      }
      else {
        FollowWall(thresh, upper, extra, disFR);
      }
      messageFromPI = ReadQR();
      if (messageFromPI != "Q") {
        TaskCounter++;
        loopForOnce = 1;
      }
    }
  }

  void executeVanster() {
    stopOnce = 0;
    int thresh = 110;
    int upper = 500;
    int upper2 = 800;
    int extra = 7;
    int toWall = 350;
    int thresh2 = 450;
    int distanceToWall = 310;
    int loopForOnce = 0;
    int stopMoreThenOne = 0;

    while (loopForOnce == 0) {
      readSensor(sensFR, FRmeasure, disFR);
      readSensor(sensFF, FFmeasure, disFF);
      readSensor(sensFL, FLmeasure, disFL);
      if (((disFL > upper2) && stopMoreThenOne == 0 ) || ((disFR > upper) && (stopMoreThenOne == 0 ))) {
        digitalWrite(LED0, HIGH);
        drivingRight();
        delay(100);
        drivingStop();
        if (disFF < upper2 ) {
          digitalWrite(LED2, HIGH);
          while (disFF > distanceToWall) {
            readSensor(sensFF, FFmeasure, disFF);
            drivingFram();
          }
          drivingStop();
          counter = 0;
          while (counter < 22) {
            drivingRight();
            counter = tick(counter);
          }
          readSensor(sensFR, FRmeasure, disFR);
          while (disFR > 250) {
            readSensor(sensFR, FRmeasure, disFR);
            drivingRight();
          }
        }
        else {
          if (StockholmV != 2) {
            counter = 0;
            while (counter < 22) {
              drivingNew3();
              counter = tick(counter);
            }
            counter = 0;
            while (counter < 6) {
              drivingHardRight();
              counter = tick(counter);
            }
            readSensor(sensFR, FRmeasure, disFR);
            while (disFR > 200 ) {
              readSensor(sensFR, FRmeasure, disFR);
              drivingNew4();
            }
          }
          if (StockholmV == 2) {
            counter = 0;
            while (counter < 38) {
              drivingFram();
              counter = tick(counter);
            }
            counter = 0;
            while (counter < 2) {
              drivingHardRight();
              counter = tick(counter);
            }

            readSensor(sensFR, FRmeasure, disFR);
            while (disFR > toWall - 200) {
              drivingNew4();
              readSensor(sensFR, FRmeasure, disFR);
            }
          }
        }
        stopMoreThenOne = 1;
        drivingStop();
        digitalWrite(LED0, LOW);
        digitalWrite(LED2, LOW);
      }
      else {
        FollowWall(thresh, upper, extra, disFR);
      }
      messageFromPI = ReadQR();
      if (messageFromPI != "Q") {

        TaskCounter++;
        loopForOnce = 1;
      }
    }
  }

  void executeHoger() {
    readSensor(sensFR, FRmeasure, disFR);
    int thresh = 110;
    int upper = 500;
    int extra = 7;

    FollowWall(thresh, upper, extra, disFR);
    if (disFR > upper) {
      digitalWrite(LED0, HIGH);
      while (disFR > 200) {
        readSensor(sensFR, FRmeasure, disFR);
        drivingNew2();
      }
      digitalWrite(LED0, LOW);
    }
    //  if (StockholmV == 1) {
    //    TaskCounter++;
    //    StockholmV = 0;
    //  }
    messageFromPI = ReadQR();
    if (messageFromPI != "Q") {
      TaskCounter++;
    }
  }

  void executeFram() {
    readSensor(sensFR, FRmeasure, disFR);
    int thresh = 110;
    int upper = 500;
    int upper2 = 800;
    int extra = 7;
    int toWall = 350;
    int thresh2 = 450;

    if (StockholmV == 0 || StockholmV == 2 ) {
      FollowWall(thresh, upper, extra, disFR);
    }
    if (disFR > upper || StockholmV == 1) {
      Serial.println("NOTHERE1");
      digitalWrite(LED0, HIGH);
      drivingStop();
      onceInTick = 0;
      counter = 0;
      int x = 16;
      if (StockholmV == 1 ) {
        drivingHardLeft();
        delay(70);
        drivingStop();

        x = 33;
      }
      while (counter < x) {
        drivingFram();
        counter = tick(counter);
      }
      while (true) {
        drivingFram();
        readSensor(sensFL, FLmeasure, disFL);
        readSensor(sensFR, FRmeasure, disFR);
        if (StockholmV == 0) {
          if (disFR < toWall) {
            break;
          }
        }
        if (StockholmV == 1  && disFL > upper2) {
          Serial.println("HERE");

          while (disFR > toWall) {
            readSensor(sensFL, FLmeasure, disFL);
            readSensor(sensFR, FRmeasure, disFR);
            //FollowWall(thresh2, upper2, extra, disFL);
            Serial.println("HERE2");
          }
          break;
        }
        if (StockholmV == 2  && disFR > upper) {
          Serial.println("NOTHERE2");
          while (disFR > toWall - 150) {
            drivingNew5();
            readSensor(sensFR, FRmeasure, disFR);
          }
          break;
        }
        break;
      }
      StockholmV = 0;
      digitalWrite(LED0, LOW);
    }
    messageFromPI = ReadQR();
    if (messageFromPI != "Q") {
      TaskCounter++;
    }
  }
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++
  void FollowWall(int T, int U, int E, int D) {
    akutForbil();
    if (D < T + E && D > T - E ) {
      drivingFram();
    }
    else if ( D > T + E && D < U ) {
      speed1 = 75;
      drivingLeft();
    }
    else if (D < T - E ) {
      speed1 = 75;
      drivingRight();
    }
    speed1 = 100;
  }
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++
  String ReadQR() {
    if (Serial.available() > 0) {
      digitalWrite(LED2, HIGH);
      drivingStop();
      String QR = Serial.readString();
      Serial.println(QR);
      if (QR == "36" || QR == "30" ) {
        StockholmV = 1;
      }
      if (QR == "33" || QR == "31" ) {
        StockholmV = 2;
      }
      if (QR == "STOP") {
        delay(5000);
        QR = "Q";
      }
      digitalWrite(LED2, LOW);
      if (BT.available()) {
        BT.println(QR);
      }
      BT.println(QR);
      return QR;
    }
    else {
      QR = "Q";
    }
    return QR;
  }
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++
  int tick(int count) {
    int x = Sensor2();
    if (x < 200) {
      if (onceInTick == 0) {
        count++;
        onceInTick = 1;
      }
    }
    if (x > 250) {
      if (onceInTick == 1) {
        count++;
        onceInTick = 0;
      }
    }
    return count;
  }
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++
  void FirstTime() {
    digitalWrite(LED1, HIGH);
    int i = 0;
    while (i == 0) {
      drivingFram();
      messageFromPI = ReadQR();
      if ( messageFromPI != "Q") {
        drivingStop();
        i = 1;
        DT = 1;
      }
      digitalWrite(LED1, LOW);
    }
  }
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++
  void readSensor(Adafruit_VL53L0X & sens, VL53L0X_RangingMeasurementData_t measure, int &result) {
    sens.rangingTest(&measure, false);
    if (measure.RangeStatus != 4) {
      if (measure.RangeMilliMeter < 30) {
        measure.RangeMilliMeter = 1000;
      }
    }
    else {
      measure.RangeMilliMeter = 1000;
    }
    result = (result + measure.RangeMilliMeter) / 2;
  }

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++
  void ReadingTasks() {
    digitalWrite(LED2, HIGH);
    if (BT.available()) {
      char BRead = BT.read();
      Serial.println(BRead);
      if (BRead == 'A') {
        BCommands[BCounter] = BRead;
        BCounter++;
      }
      if (BRead == 'C') {
        BCommands[BCounter] = BRead;
        BCounter++;
      }
      if (BRead == 'G') {
        BCommands[BCounter] = BRead;
        BCounter++;
      }
      if (BRead == 'U') {
        BCommands[BCounter] = BRead;
        BCounter++;
      }
      if (BRead == 'T') {
        BCommands[BCounter] = BRead;
        BCounter++;
      }
      if (BRead == 'K') {
        BD = 1;
        Serial.println("COMMANDS READY & STARTING");
        BT.println("DONE");
        digitalWrite(LED2, LOW);
      }

    }
  }

  void MakeConnection() {
    digitalWrite(LED2, HIGH);
    if (BT.available()) {
      int BRead = BT.read();
      Serial.print(BRead);
      if (BRead == 'M') {
        BC = 1;
        BT.println("NN");
      }
    }
  }
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++


  void drivingNew6() {
    int speed2 = 90;
    analogWrite(logicPin3, speed2);
    analogWrite(logicPin4, speed1);
    digitalWrite(logicPinReverse1, HIGH);
    digitalWrite(logicPinReverse2, HIGH);

  }



  void drivingNew5() {
    int speed2 = 50;
    analogWrite(logicPin3, speed1);
    analogWrite(logicPin4, speed2);
    digitalWrite(logicPinReverse1, HIGH);
    digitalWrite(logicPinReverse2, HIGH);

  }



  void drivingNew4() {
    int speed2 = 60;
    analogWrite(logicPin3, speed2);
    analogWrite(logicPin4, speed1);
    digitalWrite(logicPinReverse1, HIGH);
    digitalWrite(logicPinReverse2, HIGH);

  }




  void drivingNew3() {
    int speed2 = 55;
    analogWrite(logicPin3, speed2);
    analogWrite(logicPin4, speed1);
    digitalWrite(logicPinReverse1, HIGH);
    digitalWrite(logicPinReverse2, HIGH);

  }





  void drivingNew2() {
    int speed2 = 5;
    analogWrite(logicPin3, speed1);
    analogWrite(logicPin4, speed2);
    digitalWrite(logicPinReverse1, HIGH);
    digitalWrite(logicPinReverse2, HIGH);

  }


  void drivingNew() {
    int speed2 = 40;
    analogWrite(logicPin3, speed2);
    analogWrite(logicPin4, speed1);
    digitalWrite(logicPinReverse1, HIGH);
    digitalWrite(logicPinReverse2, HIGH);

  }

  void drivingFram() {
    analogWrite(logicPin3, speed1);
    analogWrite(logicPin4, speed1);
    digitalWrite(logicPinReverse1, HIGH);
    digitalWrite(logicPinReverse2, HIGH);

  }

  void drivingBak() {
    analogWrite(logicPin3, speed1);
    analogWrite(logicPin4, speed1);
    digitalWrite(logicPinReverse1, LOW);
    digitalWrite(logicPinReverse2, LOW);
  }

  void drivingRight() {
    analogWrite(logicPin3, 0);
    analogWrite(logicPin4, speed1);
    digitalWrite(logicPinReverse1, HIGH);
    digitalWrite(logicPinReverse2, HIGH);

  }

  void drivingHardRight() {
    analogWrite(logicPin3, 0.5 * speed1);
    analogWrite(logicPin4, speed1);
    digitalWrite(logicPinReverse1, LOW);
    digitalWrite(logicPinReverse2, HIGH);

  }

  void drivingLeft() {
    analogWrite(logicPin3, speed1);
    analogWrite(logicPin4, 0);
    digitalWrite(logicPinReverse1, HIGH);
    digitalWrite(logicPinReverse2, HIGH);

  }

  void drivingHardLeft() {
    analogWrite(logicPin3, speed1);
    analogWrite(logicPin4, speed1);
    digitalWrite(logicPinReverse1, HIGH);
    digitalWrite(logicPinReverse2, LOW);

  }

  void drivingStop() {
    analogWrite(logicPin3, 0);
    analogWrite(logicPin4, 0);
    digitalWrite(logicPinReverse1, HIGH);
    digitalWrite(logicPinReverse2, HIGH);
  }

  int Sensor2() {
    int value;
    value = analogRead(pResistor);
    Serial.print(" ");
    Serial.println(value);
    return value;
  }

  int readBattery() {
    int value;
    value = analogRead(batteryVoltage);
    double Volt = value / 42.34;
    Serial.println(Volt);
    return;
  }

