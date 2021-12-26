// Loosely based on https://github.com/wbphelps/FeatherCandle
#include <Adafruit_IS31FL3731.h>
#include "data-15x7.h"

// If you're using the full breakout...
//Adafruit_IS31FL3731 ledmatrix = Adafruit_IS31FL3731();
// If you're using the FeatherWing version
Adafruit_IS31FL3731_Wing ledmatrix = Adafruit_IS31FL3731_Wing();

#define PIXEL_W 15
#define PIXEL_H 7
#define FRAMES 100
#define FPS 12
#define MAX_BRIGHTNESS 40.0

int frameDelay = 1000/FPS;
float brightnessDivider = 255.0 / MAX_BRIGHTNESS;

uint8_t fire[PIXEL_W][PIXEL_H];
uint8_t page = 0; // Front/back buffer control
const uint8_t *ptr  = anim;
uint16_t ptrI = 0;
int i = 0;

void setup() {
  ledmatrix.begin();
  initPixels(0);
}

void loop() {
  if (ptrI == PIXEL_W * PIXEL_H * (FRAMES - 1)) {
    ptr = anim;
    ptrI = 0;
    i = 0;
  }
  fireFrame();
  drawFire();
  delay(frameDelay);
}

void initPixels(int v) {
  for (int y = 0; y < PIXEL_H; y++) {
    for (int x = 0; x < PIXEL_W; x++) {
      fire[x][y] = v;
    }
  }  
}

void fireFrame() {
  for (int y = 0; y < PIXEL_H; y++) {
    for (int x = 0; x < PIXEL_W; x++) {
      fire[x][y] = pgm_read_byte(ptr);
      ptr++;
      ptrI++;
    }
  }
}

void drawFire() {
  page ^= 1;
  ledmatrix.setFrame(page);
  for (int x = 0; x < PIXEL_W; x++) {
    for (int y = 0; y < PIXEL_H; y++) {
      ledmatrix.drawPixel(x, y, fire[x][y] / brightnessDivider);
    }
  }
  ledmatrix.displayFrame(page);
}
