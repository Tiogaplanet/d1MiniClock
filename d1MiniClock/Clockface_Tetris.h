#ifndef CLOCKFACE_TETRIS_H
#define CLOCKFACE_TETRIS_H

#include <Adafruit_GFX.h>
#include "Clockface.h"

class ClockfaceTetris : public Clockface {
  public:
    ClockfaceTetris();
    void update(uint8_t hour, uint8_t minute);
    void draw(Adafruit_GFX* display) const;
  private:
    // Check to see if the current piece would collide with the board
    // if the position was modified by "xd" and "yd".
    bool checkCollision(int8_t xd, int8_t yd, uint8_t r) const;
    // For when the piece hits the bottom, make it stick.
    void tileToBoard();
    // AI.
    // Called at the start of a piece to decide where to place it.
    void decideMove();
    uint8_t getPieceHeight(uint8_t piece, uint8_t r) const;
    uint8_t blockingMetric(int8_t xd, int8_t yd, uint8_t r) const;
    // Check the board for lines to clear out after putting a
    // new piece on.
    void clearLines();

    uint16_t board[20];
    uint8_t piece, rotation;
    int8_t x, y;
    uint16_t score, highscore;
    int8_t targetX;
    uint8_t targetRotation;
};

#endif
