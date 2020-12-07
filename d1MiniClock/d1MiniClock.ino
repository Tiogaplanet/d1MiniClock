/*
   https://create.arduino.cc/projecthub/john-bradnam/retro-gamer-clock-c2ceec?ref=search&ref_id=alarm%20clock%20oled%20ds3231%20buttons&offset=0
   https://sites.google.com/site/0miker0/pongclock

   Gamer Glock by jbrad2089@gmail.com
   Modified for DS3231 RTC
   Pong Clock Code by mic159.

   Requirements:
   Bounce2 https://github.com/mic159/Bounce-Arduino-Wiring
   Adafruit GFX https://github.com/adafruit/Adafruit-GFX-Library
   Adafruit SSD1306 https://github.com/adafruit/Adafruit_SSD1306
   RTClib https://github.com/mic159/RTClib
*/

#include <Bounce2.h>
#include <SPI.h>
#include <Wire.h>
#include <EEPROM.h>
#include <RTClib.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "Menu.h"
#include "Menu_Settings.h"
#include "Menu_Settings_24.h"
#include "Menu_Settings_Time.h"
#include "Menu_Settings_Date.h"
#include "Menu_Settings_Brightness.h"
#include "Menu_Clockface.h"
#include "State.h"
#include "Sound.h"

#define OLED_RESET -1
#define SPEAKER D5
#define RANDOM_SEED_PIN A0
#define MINUTE_PIN D0
#define HOUR_PIN D7

#define SSD1306_LCDWIDTH 128
#define SSD1306_LCDHEIGHT 64
#undef SSD1306_128_32
#define SSD1306_128_64

// Set this to enable printing debug stats to the screen
//#define DEBUG_STATS

#define BUTTON_ACTIVE LOW
Bounce btn1;
Bounce btn2;
Adafruit_SSD1306 display(SSD1306_LCDWIDTH, SSD1306_LCDHEIGHT, &Wire, OLED_RESET);

RTC_DS3231 RTC;

State state;
Menu* menu = NULL;

void setup(void) {

  // Suppress the BUILTIN_LED so it won't flash.
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  Serial.begin(74880);

  Serial.println("\nClock started.");

  randomSeed(analogRead(RANDOM_SEED_PIN));
  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);
  display.clearDisplay();
  display.setRotation(0);

  // First time init, set to code compile date.
  if (RTC.lostPower()) {
    RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // Setup buttons
  pinMode(MINUTE_PIN, INPUT_PULLUP);
  pinMode(HOUR_PIN, INPUT_PULLUP);
  btn1.attach(HOUR_PIN);
  btn2.attach(MINUTE_PIN);
  btn1.interval(20);
  btn2.interval(20);

  initialiseSound(SPEAKER);

//  extern const uint8_t MarilynMonroe[];
//  display.clearDisplay();
//  display.drawBitmap(0, 0,  MarilynMonroe, 128, 64, WHITE);
//  display.display();

//  extern const uint8_t Beethoven[];
//  display.clearDisplay();
//  display.drawBitmap(35, 0,  Beethoven, 58, 63, WHITE);
//  display.display();
//  playSong(melody_elise);

    extern const uint8_t GamerLogo[];
    display.clearDisplay();
    display.drawBitmap(0, 0,  GamerLogo, 128, 64, WHITE);
    display.display();

  // Show the logo screen for X amount of time.
  delay(3000);

  // Load things from state
  display.dim(state.dim);

  state.update();
  switchMenu(MENU_CLOCK);
}

void loop() {
  // As an optimisation, we only draw the display
  // when we really need to. Drawing the display
  // every time is wasteful if nothing has changed.
  bool draw = false;
#ifdef DEBUG_STATS
  unsigned long timer = millis();
#endif

  // Buttons
  if (btn1.update() && btn1.read() == BUTTON_ACTIVE) {
    menu->button1();
    draw = true;
  }
  if (btn2.update() && btn2.read() == BUTTON_ACTIVE) {
    menu->button2();
    draw = true;
  }

  // Switch menu if indicated.
  updateMenuSelection();

  // Update
  state.update();
  if (menu->update()) {
    draw = true;
  }

  // Display
  if (draw) {
    display.clearDisplay();
    menu->draw(&display);

#ifdef DEBUG_STATS
    display.setTextSize(1);
    display.setTextColor(WHITE, BLACK);
    display.setCursor(80, HEIGHT - 10);
    display.print(freeRam());
    display.setCursor(109, HEIGHT - 10);
    display.print(millis() - timer);
#endif

    display.display();
  }

}

#ifdef DEBUG_STATS
int freeRam () {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}
#endif
