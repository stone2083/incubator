#include <TTP229.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

const int TTP_SCL = 10; // TTP SCL
const int TTP_SDO = 11; // TTP SDO

const int SIM_TX = 13;  // sim tx
const int SIM_RX = 12;  // sim rx

const int LCD_RS = 2;   // lcd rs
const int LCD_E = 3;    // lcd enable
const int LCD_D4 = 4;   // lcd d4
const int LCD_D5 = 5;   // lcd d5
const int LCD_D6 = 6;   // lcd d6
const int LCD_D7 = 7;   // lcd d7

const char keys[] = {' ', '1', '2', '3', 'A', '4', '5', '6', 'B', '7', '8', '9', 'C', '*', '0', '#', 'D'};

char mobile[16];
char info[32];
int pos = -1;

TTP229 ttp229(TTP_SCL, TTP_SDO);
LiquidCrystal lcd(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
SoftwareSerial sim(SIM_TX, SIM_RX);

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  sim.begin(9600);
  sim.println("AT");
  debugSim();
}

void loop() {
  // 控制显示屏
  sprintf(info, "%s", mobile);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Tel: Made By JL");
  lcd.setCursor(0, 1);
  lcd.print(info);

  // 接收键盘信息
  char key = keys[ttp229.ReadKey16()];
  // 拨打
  switch (key) {
    case '#':
      Serial.println(">>> dia");
      char cmd[32];
      sprintf(cmd, "ATD+ +86%s;", mobile);
      sim.println(cmd);
      debugSim();
      break;
    case 'D':
      Serial.println(">>> hang up");
      sim.println("ATH");
      debugSim();
      mobile[0] = '\0';
      pos = -1;
      break;
    case '*':
      mobile[pos] = '\0';
      pos--;
      if (pos < 0) {
        pos = -1;
      }
      break;
    default:
      if (key == ' ' || pos > 11) {
        break;
      }
      pos++;
      mobile[pos] = key;
      mobile[pos + 1] = '\0';
  }

  delay(50);
}

void debugSim() {
  delay(50);
  while (Serial.available()) {
    sim.write(Serial.read());
  }
  while (sim.available()) {
    Serial.write(sim.read());
  }
}
