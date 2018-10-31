#include <SimpleSDAudio.h>
#include <Enerlib.h>

Energy energy;

#define L1 10
int A[6] = {A5, A4, A3, A2, A1, A0};
const int MainSwitch = 8;        //主要開關的腳位
const int VibrateSwitch = 7;     //微震動開關的腳位
const int ModeSwitch = 2;          //模式(敲擊震動)開關的腳位
//記憶卡腳位分別為Pin 4 11 12 13
//音訊輸出腳位為Pin 9
int x = 0, i = 0, k = 345, n = 0;
int MainSW, VibSW = 1, ModeSW;
int y = 0;
int Time;
int h = 0;
int mu = 0;

void setup()
{
  initialize();

  for (int i = 0; i < 6; i++) {
    pinMode(A[i], OUTPUT);
  }
  pinMode(ModeSwitch, INPUT_PULLUP);
  pinMode(VibrateSwitch, INPUT_PULLUP);
  pinMode(MainSwitch, INPUT_PULLUP);

  attachInterrupt(0, donothing, CHANGE);
}

void donothing() {}

void initialize() {

  Serial.begin(9600);
  // If your SD card CS-Pin is not at Pin 4, enable and adapt the following line:
  // SdPlay.setSDCSPin(10);

  //pinMode(9, OUTPUT);
  //tone(9, 800, 20);
  //delay(100);
  //tone(9, 1000, 20);
  //delay(100);             //Ready buzzer when system on
  //noTone(9);
  // Init SdPlay and set audio mode
  if (!SdPlay.init(SSDA_MODE_FULLRATE | SSDA_MODE_MONO)) {
    tone(9, 262, 500);
    delay(1000);
    tone(9, 262, 500);
    delay(1000);
    tone(9, 262, 500);
    Serial.print("Initialization Error");
    while (1); // Error while initialization of SD card -> stop and beep.
  }

}


//=========================================Start==============================================//
void Opening() {

  // Select opening file to play
  if (!SdPlay.setFile("OPEN.AFS")) {
    tone(9, 500, 100);
    delay(200);
    tone(9, 500, 100);
    Serial.print("File not found");
    while (1); // Error file not found -> stop and beep.
  }

  // Start playback
  SdPlay.play();

  // Let the worker work until playback is finished
  while (!SdPlay.isStopped()) {

    // Serial.println(i+=1);
    for (int i = 0; i < 12000; i++) {

      SdPlay.worker();

      if (i == (n * 1999)) {
        ///////Let LED light up///////
        digitalWrite(A[n], 1);
        Serial.println(n);
        n++;
      }
      if (n == 6)
        n = 0;
    }
  }
}
//============================================================================================//

void Rand() {

  int X;
  //randomSeed(analogRead(10));
  X = random(1, 8);
  switch (X) {
    case 1:
      SdPlay.setFile("HUM1.AFS");
      break;
    case 2:
      SdPlay.setFile("HUM2.AFS");
      break;
    case 3:
      SdPlay.setFile("HUM3.AFS");
      break;
    case 4:
      SdPlay.setFile("HUM4.AFS");
      break;
    case 5:
      SdPlay.setFile("HUM5.AFS");
      break;
    case 6:
      SdPlay.setFile("HUMS1.AFS");
      break;
    case 7:
      SdPlay.setFile("HUMS2.AFS");
      break;
  }
  Serial.print(X);
}

//============================================hit========================================//
int hit() {
  ModeSW = digitalRead(ModeSwitch);
  if (ModeSW == 0) {
    y = random(1, 3);
    if (y == 1) {
      SdPlay.setFile("HIT1.AFS");
      SdPlay.play();
      while (!SdPlay.isStopped()) {
        SdPlay.worker();
      }
      return 1;
    }
    else if (y == 2) {
      SdPlay.setFile("HIT2.AFS");
      SdPlay.play();
      while (!SdPlay.isStopped()) {
        SdPlay.worker();
      }
      return 1;
    }
    else {
      y = 0;
      return 0;
    }
  }
}
//=======================================================================================//

void MainRunning() {
  MainSW = 1;
  while (MainSW) {
    SdPlay.setFile("HUM.AFS");
    SdPlay.play();
    while (!SdPlay.isStopped()) {
      //=====================================wave========================================//
      hit();
      if (VibSW != digitalRead(VibrateSwitch)) {
        VibSW = digitalRead(VibrateSwitch);
        Rand();
        SdPlay.play();
        while (!SdPlay.isStopped()) {
          hit();
          MainSW = digitalRead(MainSwitch);
          if (MainSW)
            SdPlay.worker();
          else
            break;
        }
      }
      //=================================================================================//
        MainSW = digitalRead(MainSwitch);
        if (MainSW)
          SdPlay.worker();
        else
          break;
    }
    //MainSW = digitalRead(MainSwitch);
  }
  //==============================Strat Closing Effect===============================//
  SdPlay.setFile("SHUT.AFS");
  SdPlay.play();
  while (!SdPlay.isStopped()) {
    // Serial.println(i+=1);
    for (double i = 0; i < 15000; i++) SdPlay.worker();
    for (int i = 0; i < 12000; i++) {

      SdPlay.worker();

      if (i == n * 1999) {
        ///////Let LED light up///////
        digitalWrite(A[5 - n], 0);
        Serial.println(5 - n);
        n++;
      }
      if (n == 6)
        n = 0;
    }

  }
  //====================================================================================//
}

void loop() {

  for (Time = 0; Time < 6000; Time++) {
    n = 0;
    MainSW = digitalRead(MainSwitch);
    //Serial.print(MainSW);
    mode();
    if (h == 0) {
      while (MainSW == 0) {
        Time = 0;
        Opening();
        MainRunning();
        MainSW = 1;
      }
    }
    else if (h == 1) {
      gun();
    }
    if (MainSW == 0)
      Time--;
    else
      delay(10);
  }
  Serial.print("Powerdown");
  energy.PowerDown();
}

void gun() {

  if (!SdPlay.setFile("RELOAD.AFS")) {
    tone(9, 500, 100);
    delay(200);
    tone(9, 500, 100);
    Serial.print("File not found");
    while (1); // Error file not found -> stop and beep.
  }
  SdPlay.play();
  MainSW = digitalRead(MainSwitch);
  if (MainSW) {
    while (!SdPlay.isStopped()) {
      SdPlay.worker();
    }
    int ak = 0;
    //MainSW = digitalRead(MainSwitch);
    while (MainSW) {
      MainSW = digitalRead(MainSwitch);
      if (MainSW == 0) {
        ak++;
        SdPlay.setFile("BULLET.AFS");
        SdPlay.play();
        int s = 0;
        while (!SdPlay.isStopped()) {
          for (int i = 0; i < 7000; i++) {
            if (i == s * 1100) {
              digitalWrite(A[s], 1);
              digitalWrite(A[s - 1], 0);
              s++;
            }
            SdPlay.worker();
          }
        }
        if (ak == 7) {
          SdPlay.setFile("RELOAD1.AFS");
          SdPlay.play();
          while (!SdPlay.isStopped()) {
            SdPlay.worker();
          }
          ak = 0;
        }
      }
      ModeSW = digitalRead(ModeSwitch);
      if (ModeSW == 0) break;
      MainSW = 1;
    }
  }
}
void mode() {
  ModeSW = digitalRead(ModeSwitch);
  if (ModeSW == 0) {
    if (h == 0) {
      digitalWrite(A5, 1);
      delay(100);
      digitalWrite(A5, 0);
      h = 1;
    }
    else if (h == 1) {
      digitalWrite(A4, 1);
      delay(100);
      digitalWrite(A4, 0);
      h = 0;
    }
    delay(500);
  }
}
