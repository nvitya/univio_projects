/*
 * dayhist.h
 *
 *  Created on: Aug 18, 2022
 *      Author: vitya
 */

#ifndef SRC_DAYHIST_H_
#define SRC_DAYHIST_H_

#include "gfxbase.h"
#include "board_pins.h"

// one day is 96 values (every 15 min), three days

#define DAY_HIST_SIZE   96
#define MAX_HIST_DAYS    3
#define MAX_HIST_SIZE  (MAX_HIST_DAYS * DAY_HIST_SIZE)

class TDayHist
{
public:
  int16_t      x = 0;
  int16_t      y = 0;
  uint16_t     height = 64;

  uint16_t     bgcolor = RGB16(0x10, 0x10, 0x10);
  uint16_t     color   = RGB16(0x00, 0x00, 0xFF);

  uint8_t      data[MAX_HIST_SIZE];

  void         Init(int16_t ax, int16_t ay);
  void         Draw();
};

#endif /* SRC_DAYHIST_H_ */
