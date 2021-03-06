#ifndef MENU_CLOCKFACE_H
#define MENU_CLOCKFACE_H

#include <RTClib.h>
#include "Menu.h"
#include "Clockface.h"

class ClockfaceMenu : public Menu {
  public:
    ClockfaceMenu();
    ~ClockfaceMenu();

    bool update();
    void draw(Adafruit_GFX* display) const;
    void button1();
    void button2();
  private:
    void changeMenu();
    Clockface* face;
    uint8_t faceType;
};

#endif
