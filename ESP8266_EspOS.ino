#include <ESPVGAX.h>
#include "fonts/monodos12.h"
#include <ESP8266WiFi.h>
#include <String.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(5, 8);

#define BTN_PIN D3              // Пин кнопки
#ifndef STASSID
#define STASSID "HUAWEI-aa5Y"
#define STAPSK "8x75wv5E"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

bool WifiModuleEnable = true;
bool sleep = 0;
bool debug = 0;
bool btn_state = 0;
int mouseX = 0;
int mouseY = 0;
int selectedTab = 1;
int yPos = 0;
int xPos = 0;
char * menu = " Main  Functions ";
String bottom = " EspOS by Clementine v.0.2 with Wi-Fi   Your IP: ";

ESPVGAX vga;

/*class ClDesktopManager {
  private:
    int lastid;
    char * Windows [6][] = { {},                              
                             {},
                             {},
                             {},
                             {},
                             {} };  
  public:
    void setupWindow(String Header) {

    }
};

ClDesktopManager DeskMan;*/

class ClDesktop {
  private:
    int OpenedWindow = 1;
    int WindowX = 0;
    int WindowY = 0;
    int Hy = 0;
  public:
    void Window(char * Header, int X, int Y, int Width, int Height) {
      //DeskMan.setupWindow(Header);
      vga.drawRect(X, Y, X+Width, Y+Height, 0, true);
      int cX = (X+Width)/2;
      int cY = (Y+Height)/2;
      int lH = strlen(Header);
      int cH = lH/2;
      WindowX = X;
      WindowY = Y;
      vga.print_P(Header, cX+cH, cY, true);
      OpenedWindow = 1;
    }
    void CloseWindow(int WindowID) {
      
    }
    void addText(int X, int Y, char * Text, int WindowID) {
      vga.print_P(Text, WindowX+X, WindowY+Y+25, true);
    }
};

ClDesktop Env;

void loop() {
    while (1) {
      btn_state = !digitalRead(BTN_PIN);
      if (debug == true) {
        Serial.print(BTN_PIN);                
        Serial.print(" is ");
        Serial.println(btn_state);
        Serial.print("sleep is ");
        Serial.println(sleep);
      }      
      if (btn_state == 1 && sleep == 0) {
        sleep = 1;
        vga.end();
      } else if (btn_state == 0 && sleep == 0) {
        vga.clear(0xFF);
        topMenu(menu, false);  
        bottomMenu((char *) bottom.c_str());
        
        Serial.write(5);
        /*mouseX = Serial.read();
        mouseY = Serial.read();*/
        vga.print_P("X: ", 477, 25, true);
        vga.print_P((char *) mouseX, 477, 25, true);
        vga.print_P("Y: ", 477, 38, true);
        vga.print_P((char *) mouseY, 477, 38, true);

        Env.Window("Information", 30, 87, 100, 100);
        Env.addText(10, 10, "EspOS", 1);
        Env.addText(10, 23, "ver.0.2 Alpha", 1);
        Env.addText(10, 36, "by Clementine", 1);
        while (1) {
          vga.delay(100);
          if(!digitalRead(BTN_PIN) == 1) {break;}
        };   
      } else if (btn_state == 1 && sleep == 1) {
        vga.begin();
        vga.delay(500);
        sleep = 0;
      }
      if (selectedTab <= 0) {
        selectedTab = 1;
      }
      if (selectedTab >= 3) {
        selectedTab = 2;
      }
      vga.delay(100);
    }
}

void setup() {
  vga.begin();
  vga.setBitmapFont((uint8_t*)img_monodos12_data, 12);
  xPos = 5;
  yPos = 12;
  vga.print_P("VGA lib setup...", xPos, yPos, true);
  vga.delay(2000);
  pinMode(BTN_PIN, INPUT_PULLUP);
  yPos = 25;
  vga.print_P("Pin Mode setup...", xPos, yPos, true);
  vga.delay(2000);
  Serial.begin(115200);
  Serial.write(1);
  yPos = 38;
  vga.print_P("Serial setup...", xPos, yPos, true);
  vga.delay(2000);
  yPos = 51;
  if (WifiModuleEnable) {
    vga.print_P("Wi-Fi Module setup...", xPos, yPos, true);
    vga.delay(1000);
    vga.end();
    setupWiFi();
  }
  vga.print_P("Text User Interface loading...", xPos, yPos, true);
  vga.delay(5000);
  vga.clear(0xFF);
  topMenu(menu, false);
  bottomMenu((char *) bottom.c_str());
  Serial.write(2);
  //Window("test", 30, 87, 100, 100);
}

void topMenu(char * m, bool e) {  // m - текст в меню; e - вкл./выкл. отображение
  if (e) {
    vga.drawRect(0, 0, 512, 25, 0, true);
    vga.setBitmapFont((uint8_t*)img_monodos12_data, 12);
    vga.print_P(m, 5, 25/2, true);
  }
}

void bottomMenu(char * m) {  // m - текст в меню
  vga.drawRect(0, 460, 512, 20, 0, true);
  vga.setBitmapFont((uint8_t*)img_monodos12_data, 12);
  vga.print_P(m, 5, 465, true);
}

void setupWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  yPos = 64;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.write(3);
  } 
  if (WiFi.status() == WL_CONNECTED) {
    Serial.write(4);
  }
  vga.begin();
  vga.print_P("Local IP Address:", xPos, yPos, true);
  yPos = 77;
  xPos = 15;
  String ipaddr = WiFi.localIP().toString();
  vga.print_P(ipaddr.c_str(), xPos, yPos, true);
  bottom += ipaddr;
  vga.delay(5000);
  yPos = 90;
  xPos = 5;
}

/*void readMouse() {
  //if (Serial.availableForWrite()) {
    Serial.write(5);
  //}
  if (Serial.available()) {
    vga.delay(50);
    mouseX = Serial.read();
    vga.delay(50);
    mouseY = Serial.read();
  }
  tempChar += mouseX;
  vga.print_P("X: ", 477, 25, true);
  vga.print_P(tempChar, 477, 25, true);
  tempChar = "";
  tempChar += mouseY;
  vga.print_P("Y: ", 477, 38, true);
  vga.print_P(tempChar, 477, 38, true);
}*/