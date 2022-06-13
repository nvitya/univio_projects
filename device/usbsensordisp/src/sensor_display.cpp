// file:     tft_disp_test.cpp
// brief:    TFT LCD Display Test
// created:  2022-03-04
// authors:  nvitya

#include "platform.h"

#include "hwpins.h"
#include "traces.h"
#include "clockcnt.h"
#include "mscounter.h" // has much wider range than the clockcnt
#include "board_pins.h"
#include "sensor_display.h"

TSensorDisplay  g_display;

#include "fonts/FreeSansBold9pt7b.h"
#include "fonts/FreeSansBold12pt7b.h"
#include "fonts/FreeSansBold18pt7b.h"
#include "fonts/FreeSansBold24pt7b.h"
#include "fonts/FreeSansBold32pt7b.h"
#include "fonts/FreeSansBold48pt7b.h"
#include "fonts/FreeSansBold40pt7b.h"

TGfxFont font_sans_9(&FreeSansBold9pt7b);
TGfxFont font_sans_18(&FreeSansBold18pt7b);
//TGfxFont font_sans_small(&FreeSansBold12pt7b);

TGfxFont font_sans_24(&FreeSansBold24pt7b);
TGfxFont font_sans_32(&FreeSansBold32pt7b);
TGfxFont font_sans_48(&FreeSansBold48pt7b);

void TSensorDisplay::Init()
{
  testid = START_TEST;
  last_update_ms = mscounter();

  ReDraw();
}

void TSensorDisplay::Run()
{
  ++idlecnt;

  unsigned curms = mscounter();

  if (curms - last_update_ms < 500)
  {
    return;
  }

  //ReDraw();

  last_update_ms = mscounter();
}

#if 0

void TSensorDisplay::ReDraw()
{
  disp.FillScreen(0);

  pos_y = 10;
  pos_x = 10;

  //dwidth = disp.width / 3;
  //dheight = disp.height - 20;

  disp.color = RGB16(255, 255, 255);

  disp.SetFont(&font_sans_big);
  disp.SetCursor(5, 5);
  disp.DrawString("13:49");

  disp.SetFont(&font_sans_small);
  disp.SetCursor(5, 80);
  disp.DrawString("2022-06-12, SUN");

  disp.SetFont(&font_sans_big);
  disp.color = RGB16(0x00, 0xFF, 0x00);
  disp.SetCursor(300, 12);
  disp.DrawString("26.7");

  disp.color = RGB16(0x00, 0xFF, 0xFF);
  disp.SetCursor(350, 112);
  disp.DrawString("41");

  disp.color = RGB16(0xFF, 0x40, 0xFF);
  //disp.color = RGB16(0xFF, 0xFF, 0x40);
  disp.SetCursor(300, 220);
  disp.DrawString("1008");

  disp.color = RGB16(0x40, 0x40, 0x40);
  disp.SetFont(&font_sans_small);
  disp.SetCursor(480 - 35, 60);
  disp.DrawString("*C");
  disp.SetCursor(480 - 35, 160);
  disp.DrawString("%");
  disp.SetCursor(480 - 60, 320 - 24);
  disp.DrawString("hPa");
}

#endif

#if 1

void TSensorDisplay::ReDraw()
{
  disp.FillScreen(0);

  pos_y = 10;
  pos_x = 10;

  //dwidth = disp.width / 3;
  //dheight = disp.height - 20;

#if 0

  disp.color = RGB16(0xF0, 0xF0, 0x40);
  disp.SetFont(&font_sans_48);
  disp.SetCursor(240, 1);
  disp.DrawString("13:49");

  disp.color = RGB16(255, 255, 255);
  disp.SetFont(&font_sans_18);
  disp.SetCursor(5, 7);
  disp.DrawString("2022-06-12");
  disp.SetCursor(5, 50);
  disp.DrawString("Sunday");


  disp.SetFont(&font_sans_32);
  disp.color = RGB16(0x00, 0xFF, 0x00);
  disp.SetCursor(34, 100);
  disp.DrawString("26.7");

  disp.color = RGB16(0x00, 0xFF, 0xFF);
  disp.SetCursor(90, 175);
  disp.DrawString("41");

  disp.color = RGB16(0xFF, 0x40, 0xFF);
  //disp.color = RGB16(0xFF, 0xFF, 0x40);
  disp.SetCursor(9, 254);
  disp.DrawString("1008");

  disp.color = RGB16(0x40, 0x40, 0x40);
  disp.SetFont(&font_sans_9);
  disp.SetCursor(168, 120);
  disp.DrawString("*C");
  disp.SetCursor(173, 200);
  disp.DrawString("%");
  disp.SetCursor(157, 276);
  disp.DrawString("hPa");

  disp.color = RGB16(0x20, 0x20, 0x20);
  disp.FillRect(194,  90, 280, 70, disp.color);
  disp.FillRect(194, 166, 280, 70, disp.color);
  disp.FillRect(194, 244, 280, 70, disp.color);

#else

  int16_t xs = 320;
  int16_t xs2 = 130;

  disp.color = RGB16(0xF0, 0xF0, 0x40);
  disp.SetFont(&font_sans_48);
  disp.SetCursor(20, 1);
  disp.DrawString("13:49");

  //disp.color = RGB16(255, 255, 255);
  disp.SetFont(&font_sans_18);
  disp.SetCursor(290, 7);
  disp.DrawString("2022-06-12");
  disp.SetCursor(320, 50);
  disp.DrawString("Sunday");

  disp.SetFont(&font_sans_32);
  disp.color = RGB16(0x00, 0xFF, 0x00);
  disp.SetCursor(34 + xs, 100);
  disp.DrawString("26.7");

  disp.color = RGB16(0x00, 0xFF, 0xFF);
  disp.SetCursor(90 + xs, 175);
  disp.DrawString("41");

  disp.color = RGB16(0xFF, 0x40, 0xFF);
  //disp.color = RGB16(0xFF, 0xFF, 0x40);
  disp.SetCursor(12 + xs, 254);
  disp.DrawString("1008");

  disp.color = RGB16(0x40, 0x40, 0x40);
  disp.SetFont(&font_sans_9);
  disp.SetCursor(165 + xs2, 122);
  disp.DrawString("*C");
  disp.SetCursor(168 + xs2, 202);
  disp.DrawString("%");
  disp.SetCursor(160 + xs2, 276);
  disp.DrawString("hPa");

  disp.color = RGB16(0x10, 0x10, 0x10);
  disp.FillRect(5,  92, 280, 70, disp.color);
  disp.FillRect(5, 168, 280, 70, disp.color);
  disp.FillRect(5, 244, 280, 70, disp.color);

  disp.color = RGB16(0x20, 0x20, 0x20);
  disp.FillRect(5,  92, 240, 70, disp.color);
  disp.FillRect(5, 168, 240, 70, disp.color);
  disp.FillRect(5, 244, 240, 70, disp.color);

#endif

}

#endif
