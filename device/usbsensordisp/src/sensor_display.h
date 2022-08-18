// file:     sensor_display.cpp
// brief:    TFT LCD Display
// created:  2022-06-12
// authors:  nvitya

#ifndef SRC_TFT_DISP_TEST_H_
#define SRC_TFT_DISP_TEST_H_

#include "gfxbase.h"
#include "dayhist.h"

#define TEST_CHANGE_MS  8000

#define TEST_COUNT   3
#define START_TEST   0
#define SINGLE_TEST  0

class TSensorDisplay
{
public:  // actual values
  int         v_seconds = 72180;
  int         v_tc = 281;
  int         v_rh = 47;
  int         v_pa = 1007;

  char        date_text[16];
  char        day_text[16];

public:
  unsigned    framecnt = 0;
  unsigned    idlecnt = 0;

  unsigned    last_update_ms = 0;

  unsigned    y_data = 92;
  unsigned    h_data = 76;

  int         testid = -1;
  int16_t     pos_x = 0;
  int16_t     pos_y = 0;

  unsigned    last_update = 0;

  uint16_t    color_tc = RGB16(0x00, 0xFF, 0x00);
  uint16_t    color_rh = RGB16(0x00, 0xFF, 0xFF);
  uint16_t    color_pa = RGB16(0xFF, 0x40, 0xFF);

  TDayHist    hist_tc;
  TDayHist    hist_rh;
  TDayHist    hist_pa;

  void Init();
  void Run();

  void ReDraw();

public:
  void DrawClock();
  void DrawValues();
  void DrawDate();
  void DrawUnits();
};

extern TSensorDisplay  g_display;

#endif /* SRC_TFT_DISP_TEST_H_ */
