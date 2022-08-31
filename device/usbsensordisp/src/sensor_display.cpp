// file:     tft_disp_test.cpp
// brief:    TFT LCD Display Test
// created:  2022-03-04
// authors:  nvitya

#include "platform.h"
#include "mp_printf.h"

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

//TGfxFont font_sans_24(&FreeSansBold24pt7b);
TGfxFont font_sans_32(&FreeSansBold32pt7b);
TGfxFont font_sans_48(&FreeSansBold48pt7b);

void TSensorDisplay::Init()
{
  testid = START_TEST;
  last_update_ms = mscounter();

  hist_tc.color = color_tc;
  hist_tc.Init(0, 92);

  hist_rh.color = color_rh;
  hist_rh.Init(0, 168);

  hist_pa.color = color_pa;
  hist_pa.Init(0, 244);

  mp_snprintf(date_text, sizeof(date_text), "2022-08-18");
  mp_snprintf(day_text,  sizeof(date_text), "Thursday");

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

  DrawUnits();
  DrawDate();
  DrawClock();
  DrawValues();

  hist_tc.Draw();
  hist_rh.Draw();
  hist_pa.Draw();
}

void TSensorDisplay::DrawClock()
{
  disp.color = RGB16(0xF0, 0xF0, 0x40);
  disp.SetFont(&font_sans_48);
  disp.SetCursor(20, 1);

  int hours = v_seconds / 3600;
  int minutes = (v_seconds % 3600) / 60;

  disp.printf("%02d:%02d", hours, minutes);
}

void TSensorDisplay::DrawDate()
{
  disp.color = RGB16(0xF0, 0xF0, 0x40);

  disp.SetFont(&font_sans_18);
  disp.SetCursor(290, 7);
  disp.DrawString(date_text); // fix length

  uint16_t tw = disp.TextWidth(day_text);
  disp.SetCursor(290 + 90 - tw / 2, 50);
  disp.DrawString(day_text);  // variable length
}

void TSensorDisplay::DrawValues()
{
  int16_t xs = 478;
  int16_t ts = 335;
  char    txt[16];
  uint16_t tw;

  disp.SetFont(&font_sans_32);

  disp.color = color_tc;
  mp_snprintf(txt, sizeof(txt), "%4.1f", v_tc / 10.0);
  tw = disp.TextWidth(txt);
  disp.SetCursor(xs - tw, 100);
  disp.DrawString(txt);
  disp.FillRect(ts, 100, xs - tw - ts, 70, 0);

  disp.color = color_rh;
  mp_snprintf(txt, sizeof(txt), "%2d", v_rh);
  tw = disp.TextWidth(txt);
  disp.SetCursor(xs - tw, 175);
  disp.DrawString(txt);
  disp.FillRect(ts, 175, xs - tw - ts, 70, 0);

  disp.color = color_pa;
  mp_snprintf(txt, sizeof(txt), "%4d", v_pa);
  tw = disp.TextWidth(txt);
  disp.SetCursor(xs - tw, 254);
  disp.DrawString(txt);
  disp.FillRect(ts, 254, xs - tw - ts, 70, 0);
}

void TSensorDisplay::DrawUnits()
{
  int16_t xs = 140;
  int16_t py =  30 + y_data;

  disp.color = RGB16(0x40, 0x40, 0x40);
  disp.SetFont(&font_sans_9);
  disp.SetCursor(165 + xs, py + 0 * h_data);
  disp.DrawString("*C");
  disp.SetCursor(168 + xs, py + 1 * h_data);
  disp.DrawString("%");
  disp.SetCursor(160 + xs, py + 2 * h_data);
  disp.DrawString("hPa");
}

