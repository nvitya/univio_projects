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

#include "fonts/FreeSansBold32pt7b.h"
#include "fonts/FreeSansBold48pt7b.h"
#include "fonts/FreeSansBold40pt7b.h"
TGfxFont font_sans_big(&FreeSansBold40pt7b);

#include "fonts/FreeSansBold12pt7b.h"
TGfxFont font_sans_small(&FreeSansBold12pt7b);

#if 0
#include "fonts/FreeSerifBold40pt7b.h"
TGfxFont font_serif_big(&FreeSerifBold40pt7b);

#include "fonts/FreeMonoBold40pt7b.h"
TGfxFont font_mono_big(&FreeMonoBold40pt7b);

#include "font_FreeSerifBold9pt7b.h"
TGfxFont font_serif(&FreeSerifBold9pt7b);

#endif

//#include "font_FreeMonoBold9pt7b.h"
//TGfxFont font_serif(&FreeMonoBold9pt7b);


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

#if 0
  disp.SetFont(&font_mono_big);
  disp.SetCursor(20, 150);
  disp.DrawString("13:49");

  disp.SetFont(&font_serif_big);
  disp.SetCursor(20, 240);
  disp.DrawString("13:49");
#endif
}
