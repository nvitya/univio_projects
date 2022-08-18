/*
 * dayhist.cpp
 *
 *  Created on: Aug 18, 2022
 *      Author: vitya
 */

#include <dayhist.h>

void TDayHist::Init(int16_t ax, int16_t ay)
{
  x = ax;
  y = ay;

  // init some test data
  for (unsigned n = 0; n < sizeof(data); ++n)
  {
    data[n] = n;
  }
}

void TDayHist::Draw()
{
  int xp = x;
  for (int i = 0; i < sizeof(data); ++i)
  {
    if ((i > 0) && (0 == (i % DAY_HIST_SIZE)))
    {
      // add a gap between the days
      disp.FillRect(xp+1,  y, 4, height, RGB16(0,0,0));
      xp += 4;
    }

    uint8_t vh = (data[i] & 0x3F);
    disp.FillRect(xp, y,           1, height-vh, bgcolor);
    disp.FillRect(xp, y+height-vh, 1, vh,        color);

    ++xp;
  }
}
