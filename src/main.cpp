#include <TFT_eSPI.h>
#include "font_50.h"
// Include I2S driver
#include <driver/i2s.h>

// Connections to INMP441 I2S microphone
#define I2S_WS 25
#define I2S_SD 33
#define I2S_SCK 32

// Use I2S Processor 0
#define I2S_PORT I2S_NUM_0

// Define input buffer length
#define bufferLen 16
int32_t sBuffer[bufferLen];

// TFT Pins has been set in the TFT_eSPI library in the User Setup file TTGO_T_Display.h
// #define TFT_MOSI            19
// #define TFT_SCLK            18
// #define TFT_CS              5
// #define TFT_DC              16
// #define TFT_RST             23
// #define TFT_BL              4   // Display backlight control pin

TFT_eSPI tft = TFT_eSPI(135, 240);

void i2s_install() {

  const i2s_config_t i2s_config = {
        .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX), // Receive, not transfer
        .sample_rate = 16000,                              // 16KHz 44100
        .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,      // could only get it to work with 32bits
        .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,      // although the SEL config should be left, it seems to transmit on right
        .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1, // Interrupt level 1
        .dma_buf_count = 2,                       // number of buffers
        .dma_buf_len = bufferLen                 // samples per buffer
    };

  i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
}

void i2s_setpin() {
  // Set I2S pin configuration
  const i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_SCK,
    .ws_io_num = I2S_WS,
    .data_out_num = -1,
    .data_in_num = I2S_SD
  };

  i2s_set_pin(I2S_PORT, &pin_config);
}

void setup()
{
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    tft.setTextSize(2);
    uint16_t yellow = tft.color565(255, 255, 0);//进行RGB565颜色 转换
    tft.setTextColor(yellow); 
    tft.setCursor(0, 0);
    tft.setTextDatum(MC_DATUM);

   /*
    tft.setSwapBytes(true);
    tft.pushImage(0, 0,  240, 135, logo);
    delay(5000);
    */
   //半透明
   for (uint16_t a = 0; a < 255; a++) // Alpha 0 = 100% background, alpha 255 = 100% foreground
  {
    //tft.drawFastHLine(192, a, 12, tft.alphaBlend(a, TFT_BLACK, TFT_WHITE));
    tft.drawFastHLine(204, a, 12, tft.alphaBlend(a, TFT_RED,   TFT_WHITE));
    tft.drawFastHLine(216, a, 12, tft.alphaBlend(a, TFT_GREEN, TFT_WHITE));
    tft.drawFastHLine(228, a, 12, tft.alphaBlend(a, TFT_BLUE,  TFT_WHITE));
  }

  tft.drawFastHLine(104, 12, 60,TFT_RED);//画一条红色线


  // Set up Serial Monitor
  Serial.begin(115200);
  Serial.println(" ");

  delay(1000);

  // Set up I2S
  i2s_install();
  i2s_setpin();
  i2s_start(I2S_PORT);

  delay(500);
    
}

void loop()
{
  tft.drawString("Moonbeam ", 75, 120);
  tft.loadFont(font_50); //指定tft屏幕对象载入font_50字库
  tft.drawString("赵凯",60,20); //在坐标0,0位置输出汉字,就可以在tft显示出来了
  tft.drawString("撸起袖子",60,60); //在坐标0,0位置输出汉字,就可以在tft显示出来了
  tft.drawString("加油干！",60,100);
  tft.unloadFont(); //释放字库文件,节省资源
  delay(2000);


  size_t bytesIn = 0;
                
  i2s_read(I2S_PORT, &sBuffer, bufferLen, &bytesIn, portMAX_DELAY);

  for (int i = 0; i < bytesIn / 4; i++) 
  {
      int32_t sample = sBuffer[i];
      sample >>= 14;
      int16_t out = sample;
   
      //CONVERTED TO SIGNED 16 BIT SAMPLE, save in same array
      ((int16_t *)sBuffer)[i] = out;
      Serial.println(out);    
  }
  bytesIn = bytesIn / 2; //reduce array size, since int16_t now

}
