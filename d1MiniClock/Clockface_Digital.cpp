/*
 * Digital Clockface
 *
 * Shows the hours:minutes in big text, and has a bar underneath showing
 * the seconds.
 * It also has the date on the right hand side of the screen.
 */
#include <Adafruit_GFX.h>
#include "Clockface_Digital.h"
#include "State.h"

#define WHITE 1
#define BLACK 0
#define WIDTH 128
#define HEIGHT 64

void ClockfaceDigital::draw(Adafruit_GFX* display) const {
  char buff[9];

  uint8_t hour = state.now.hour();
  if (!state.mode24h) {
    hour = hour % 12;
    if (hour == 0) hour = 12;
  }
  snprintf_P(buff, 9, PSTR("%02u:%02u")
    , hour
    , state.now.minute()
    );

  // Hour:Minute
  display->setTextColor(WHITE);
  display->setTextSize(2);
  display->setCursor(10, 24);
  display->print(buff);

  // am/pm suffix
  display->setTextSize(1);
  if (!state.mode24h) {
    display->setCursor(70, 31);
    if (state.now.hour() > 11) {
      display->print(F("pm"));
    } else {
      display->print(F("am"));
    }
  }

  // Seconds meter
  display->fillRect(0, 41, map(state.now.second(), 0, 60, 0, WIDTH), 3, WHITE);

  // Day of week
  display->setCursor(106, 16);
  display->print(state.getDayStr(state.now.dayOfTheWeek()));

  // Date
  display->setCursor(88, 24);
  snprintf_P(buff, 9, PSTR("%2u"), state.now.day());
  display->print(buff);
  display->print(' ');
  display->print(state.getMonthStr(state.now.month()));
  display->setCursor(100, 32);
  display->print(state.now.year());
}
