#include <ESPVGAX.h>
#include "fonts/monodos12.h"
#include <ESP8266WiFi.h>
#include <String.h>
#include <PS2Keyboard.h>

PS2Keyboard keyboard;
ESPVGAX vga;

#ifndef STASSID
#define STASSID "HUAWEI-aa5Y"
#define STAPSK "8x75wv5E"
#endif
const char* ssid = STASSID;
const char* password = STAPSK;

const int DataPin = 6;
const int IRQpin =  8;

class set {
  public:
  bool radio = 0; // Wi-Fi Module
  bool debug = 1; // Debug Mode
  bool mouse = 0; // Mouse Enable
  bool keybd = 1; // Keyboard Enable
};

class pos {
  public:
  int x = 0;
  int y = 0;
};

pos p;
set s;

/*class text {

};*/

void setup() {
  vga.begin();
  vga.setBitmapFont((uint8_t*)img_monodos12_data, 12);
  p.x = 5;
  p.y = 12;
  vga.print_P("VGA lib setup...", p.x, p.y, true);
  vga.delay(2000);
  p.y += 13;
  vga.print_P("Serial setup...", p.x, p.y, true);
  Serial.begin(115200);
  p.y += 13;
  if (s.mouse) { // Mouse Setup, soon...
  p.y += 13;
  }
  if (s.keybd) { // Keyboard Setup
  vga.print_P("Keyboard setup...", p.x, p.y, true);
    keyboard.begin(DataPin, IRQpin);
    p.y += 13;
  }
  if (s.radio) { // Wi-Fi Module Setup
    //vga.print_P("Wi-Fi Module setup...", p.x, p.y, true);
    //vga.end();
    //setupWiFi();
    p.y += 13;
  }
  vga.print_P("Text User Interface loading...", p.x, p.y, true);
  vga.delay(5000);
}

void loop() {
  if (keyboard.available()) {
    char c = keyboard.read();
    Serial.print(c);
  }
}
