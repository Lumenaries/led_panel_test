#include "ESP32-HUB75-MatrixPanel-I2S-DMA.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

MatrixPanel_I2S_DMA *dma_display = nullptr;

#define PANEL_RES_Y 32
#define PANEL_RES_X 16

#define NUM_ROWS 1
#define NUM_COLS 1

#define R1_PIN 5
#define G1_PIN 42
#define B1_PIN 6
#define R2_PIN 7
#define G2_PIN 41
#define B2_PIN 15
#define A_PIN 16
#define B_PIN 40
#define C_PIN 17
#define LAT_PIN 39
#define OE_PIN 8
#define CLK_PIN 18

#define D_PIN 38
#define E_PIN -1

HUB75_I2S_CFG::i2s_pins _pins = {R1_PIN, G1_PIN,  B1_PIN, R2_PIN, G2_PIN,
                                 B2_PIN, A_PIN,   B_PIN,  C_PIN,  D_PIN,
                                 E_PIN,  LAT_PIN, OE_PIN, CLK_PIN};
void rgb_cycle() {
  int8_t red = 255;
  int8_t green = 0;
  int8_t blue = 0;

  while (true) {
    dma_display->fillScreenRGB888(red, green, blue);
    if (red) {
      red = 0;
      green = 255;
    } else if (green) {
      green = 0;
      blue = 255;
    } else if (blue) {
      blue = 0;
      red = 255;
    } else {
      red = 255;
      green = 0;
      blue = 0;
    }
    vTaskDelay(100);
  }
}

extern "C" void app_main() {
  HUB75_I2S_CFG mxconfig(PANEL_RES_X, PANEL_RES_Y, 1, _pins);

  dma_display = new MatrixPanel_I2S_DMA(mxconfig);
  dma_display->begin();
  dma_display->setBrightness8(5);
  dma_display->clearScreen();

  // dma_display->fillScreenRGB888(255, 255, 255);

  rgb_cycle();
  // dma_display->fillRect(1, 1, 5, 5, 0, 0, 255);
  // dma_display->drawPixelRGB888(0, 0, 255, 0, 0);
}
