#include "ESP32-HUB75-MatrixPanel-I2S-DMA.h"
#include "ESP32-VirtualMatrixPanel-I2S-DMA.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

MatrixPanel_I2S_DMA *dma_display = nullptr;
VirtualMatrixPanel* FourScanPanel = nullptr;

#define PANEL_RES_X 32
#define PANEL_RES_Y 16

#define NUM_ROWS 1
#define NUM_COLS 1

#define R1_PIN 32
#define G1_PIN 14
#define B1_PIN 15
#define R2_PIN 27
#define G2_PIN 33
#define B2_PIN 12
#define A_PIN 21
#define B_PIN 19
#define C_PIN 5
#define LAT_PIN 7
#define OE_PIN 8
#define CLK_PIN 13

#define D_PIN 4
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
  HUB75_I2S_CFG mxconfig(PANEL_RES_X * 2, PANEL_RES_Y / 2, 1, _pins);

  dma_display = new MatrixPanel_I2S_DMA(mxconfig);
  dma_display->begin();
  dma_display->setBrightness8(25);
  dma_display->clearScreen();

  FourScanPanel = new VirtualMatrixPanel((*dma_display), NUM_ROWS, NUM_COLS, PANEL_RES_X, PANEL_RES_Y);
  FourScanPanel->setPhysicalPanelScanRate(FOUR_SCAN_32PX_HIGH);

  while (true) {
    //FourScanPanel->drawPixelRGB888(10, 10, 100, 0, 100);
    for (int i = 0; i < PANEL_RES_X; i++) {
        for (int j = 0; j < PANEL_RES_Y; j++) {
            FourScanPanel->drawPixelRGB888(i, j, 100, 0, 100);
            ESP_LOGI("coordinate", "%d %d", i, j);
            vTaskDelay(100);
        }
    }
    //dma_display->drawPixel();
    /*
    dma_display->fillScreenRGB888(255, 255, 255);
    vTaskDelay(100);
    dma_display->fillScreenRGB888(100, 100, 100);
    */
  }

  //rgb_cycle();
  // dma_display->fillRect(1, 1, 5, 5, 0, 0, 255);
}
