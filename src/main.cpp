#include <TFT_eSPI.h>
#include "font_50.h"

// TFT Pins has been set in the TFT_eSPI library in the User Setup file TTGO_T_Display.h
// #define TFT_MOSI            19
// #define TFT_SCLK            18
// #define TFT_CS              5
// #define TFT_DC              16
// #define TFT_RST             23
// #define TFT_BL              4   // Display backlight control pin

TFT_eSPI tft = TFT_eSPI(135, 240);

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

}
