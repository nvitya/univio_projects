/* -----------------------------------------------------------------------------
 * This file is a part of the UNIVIO projects: https://github.com/nvitya/univio_projects
 * Copyright (c) 2022 Viktor Nagy, nvitya
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from
 * the use of this software. Permission is granted to anyone to use this
 * software for any purpose, including commercial applications, and to alter
 * it and redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software in
 *    a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source distribution.
 * --------------------------------------------------------------------------- */
/*
 *  file:     uio_sensor.h
 *  brief:    UNIVIO Sensor Device Main Class
 *  version:  1.00
 *  date:     2022-02-13
 *  authors:  nvitya
*/

#pragma once

#include "uio_dev_base.h"

// the sensors:
#include "aht10.h"
#include "bmp280.h"
#include "i2cmanager.h"

#define UIOCFG_SIGNATURE   0xAA66CF55

typedef struct
{
  uint32_t          signature;

  uint32_t          base_length;
  uint32_t          base_csum;

  TUioDevBaseCfg    basecfg;

  uint64_t          _tail_pad;  // this must be the last, ensuring 8 byte size alignment
//
} TUioCfgStb;

struct TAht10Results
{
  uint8_t           ic_status;
  uint8_t           _reserved[3];
  uint32_t          measure_count;
  int               t_deg_x100;
  uint32_t          rh_percent_x100;
};

struct TBmp280Results
{
  uint8_t           ic_status;
  uint8_t           ic_control;
  uint8_t           ic_config;
  uint8_t           _reserved[1];
  uint32_t          measure_count;
  uint32_t          p_pascal;
  uint32_t          t_deg_x100;
};

class TUioSensor : public TUioDevBase
{
public:
  uint32_t          query_counter = 0;
  uint32_t          query_counter_prev = 0;

  TAht10Results     aht10_results;
  TBmp280Results    bmp280_results;

public:  // base class mandatory implementations
  virtual bool      InitDevice();
  virtual bool      HandleDeviceRequest(TUnivioRequest * rq);
  virtual void      SaveSetup();
  virtual void      LoadSetup();
  virtual void      SetRunMode(uint8_t arunmode);

public:
  void              Run(); // run idle parts

};


extern TAht10   aht10;
extern TBmp280  bmp280;
