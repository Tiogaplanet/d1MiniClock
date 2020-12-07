/*
  ARDUINO PLAYER PIANO - 12 keys + preset songs
*/
#pragma once

// Constants for notes
#define REST	 0
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define END_OF_TUNE 0xFFFF

#define DUR_8 0xE000
#define DUR_6 0xC000
#define DUR_4 0x8000
#define DUR_3 0x6000
#define DUR_2 0x4000
#define DUR_1 0x2000

// FüR ELISE (Initial tune)
const uint16_t melody_elise[] PROGMEM = {
  DUR_4 | NOTE_E5, DUR_4 | NOTE_DS5, DUR_4 | NOTE_E5, DUR_4 | NOTE_DS5, DUR_4 | NOTE_E5, DUR_4 | NOTE_B4, DUR_4 | NOTE_D5, DUR_4 | NOTE_C5, DUR_2 | NOTE_A4,
  DUR_8 | NOTE_C4, DUR_4 | NOTE_E3, DUR_4 | NOTE_A4, DUR_2 | NOTE_B4, DUR_8 | NOTE_E3, DUR_4 | NOTE_GS3, DUR_4 | NOTE_B4, DUR_2 | NOTE_C5,
  DUR_8 | NOTE_E3,
  DUR_4 | NOTE_E5, DUR_4 | NOTE_DS5, DUR_4 | NOTE_E5, DUR_4 | NOTE_DS5, DUR_4 | NOTE_E5, DUR_4 | NOTE_B4, DUR_4 | NOTE_D5, DUR_4 | NOTE_C5, DUR_1 | NOTE_A4,
  DUR_4 | REST, END_OF_TUNE
};

//forward references
void playNote(uint16_t noteRaw);

//Pin for speaker
uint8_t speaker = 0;

//Initialise Sound Routines
void initialiseSound(uint8_t pin)
{
  pinMode(pin, OUTPUT);
  speaker = pin;
}

//Play a melody array from SRAM
void playSong(uint16_t* melody)
{

  //Play each note in the melody until the END_OF_TUNE note is encountered
  int thisNote = 0;
  uint16_t noteRaw = pgm_read_word(&melody[thisNote++]);
  while (noteRaw != END_OF_TUNE)
  {
    playNote(noteRaw);
    noteRaw = pgm_read_word(&melody[thisNote++]);
  } //while

  delay(50);
}

void playNote(uint16_t noteRaw)
{
  // to calculate the note duration, take one second divided by the note type.
  // e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
  uint16_t frequency = noteRaw & 0x1FFF;
  uint8_t duration = (noteRaw & 0xE000) >> 13;
  if (duration == 7)
    duration = 8;
  uint16_t noteDuration = 1300 / duration;

  int led = 0;
  if (frequency != REST)
  {
    tone(speaker, frequency, noteDuration);
  }
}
