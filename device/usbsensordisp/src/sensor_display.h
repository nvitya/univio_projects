// file:     sensor_display.cpp
// brief:    TFT LCD Display
// created:  2022-06-12
// authors:  nvitya

#ifndef SRC_TFT_DISP_TEST_H_
#define SRC_TFT_DISP_TEST_H_

#include "gfxbase.h"

#define TEST_CHANGE_MS  8000

#define TEST_COUNT   3
#define START_TEST   0
#define SINGLE_TEST  0

class TSensorDisplay
{
public:
  unsigned    framecnt = 0;
  unsigned    idlecnt = 0;

  unsigned    last_update_ms = 0;

  int         testid = -1;
  int16_t     pos_x = 0;
  int16_t     pos_y = 0;

  unsigned    last_update = 0;

  TGfxBase *  pdisp = nullptr;

  void Init();
  void InitTest();
  void Run();

  void ReDraw();
};

extern TSensorDisplay  g_display;

#endif /* SRC_TFT_DISP_TEST_H_ */
