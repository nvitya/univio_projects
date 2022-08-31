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
 *  file:     uio_sensor.cpp
 *  brief:    UNIVIO Sensor Device Main Class
 *  version:  1.00
 *  date:     2022-02-13
 *  authors:  nvitya
*/

#include "string.h"
#include "board_pins.h"
#include "hwintflash.h"

#include <uio_sensor.h>

#include "sensor_display.h"

TAht10            aht10;
TBmp280           bmp280;

bool TUioSensor::InitDevice()
{
  basecfg.usb_vendor_id  = 0xDEAD;
  basecfg.usb_product_id = 0xA5E2;
  strncpy(basecfg.manufacturer, "UNIVIO", sizeof(basecfg.manufacturer));
  strncpy(basecfg.device_id, "Meteo Display", sizeof(basecfg.device_id));

  hwintflash.Init();

  bmp280.Init(&i2c, 0x76);

  aht10.Init(&i2c, 0x38);

  return true;
}

bool TUioSensor::HandleDeviceRequest(TUnivioRequest * rq)
{
  unsigned    n;
  uint32_t    rv32;
  uint16_t    rv16;
  uint16_t    err;

  uint16_t addr = rq->address;

  ++query_counter;

  // IO Control

  if ((0x2000 <= addr) && (addr < 0x3000)) // Sensor read out
  {
    if (rq->iswrite)
    {
      return ResponseError(rq, UIOERR_READ_ONLY);
    }

    switch (addr)
    {
      case 0x2000:  return ResponseBin(rq, &bmp280_results, sizeof(TBmp280Results));
      case 0x2001:  return ResponseU32(rq, bmp280_results.measure_count);
      case 0x2002:  return ResponseU32(rq, bmp280_results.p_pascal);
      case 0x2003:  return ResponseI32(rq, bmp280_results.t_deg_x100);

      case 0x2100:  return ResponseBin(rq, &aht10_results, sizeof(TAht10Results));
      case 0x2101:  return ResponseU32(rq, aht10_results.measure_count);
      case 0x2102:  return ResponseU32(rq, aht10_results.rh_percent_x100);
      case 0x2103:  return ResponseI32(rq, aht10_results.t_deg_x100);
    }

    return ResponseError(rq, UIOERR_WRONG_ADDR);
  }
  else if ((0x3000 <= addr) && (addr < 0x4000)) // display control
  {
    switch (addr)
    {
      case 0x3000:
        err = HandleRw(rq, &g_display.v_seconds, sizeof(g_display.v_seconds));
        g_display.DrawClock();
        return err;
      case 0x3001:
        if (rq->iswrite)  memset(&g_display.date_text, 0, sizeof(g_display.date_text));
        err = HandleRw(rq, &g_display.date_text, sizeof(g_display.date_text));
        g_display.DrawDate();
        return err;
      case 0x3002:
        if (rq->iswrite)  memset(&g_display.date_text, 0, sizeof(g_display.day_text));
        err = HandleRw(rq, &g_display.day_text, sizeof(g_display.day_text));
        g_display.DrawDate();
        return err;
    }

    return ResponseError(rq, UIOERR_WRONG_ADDR);
  }
  else if ((0x8000 <= addr) && (addr + rq->length <= 0x8000 + sizeof(TBmp280Results))) // sensor result structs
  {
    uint8_t * u8ptr = (uint8_t *)&bmp280_results;
    return HandleRw(rq, u8ptr + (addr - 0x8000), rq->length);
  }
  else if ((0x8100 <= addr) && (addr + rq->length <= 0x8100 + sizeof(TAht10Results))) // sensor result structs
  {
    uint8_t * u8ptr = (uint8_t *)&aht10_results;
    return HandleRw(rq, u8ptr + (addr - 0x8100), rq->length);
  }
  else if ((0x8200 <= addr) && (addr + rq->length <= 0x8200 + sizeof(g_display.hist_tc.data)))
  {
    uint8_t * u8ptr = (uint8_t *)&g_display.hist_tc.data[0];
    err = HandleRw(rq, u8ptr + (addr - 0x8200), rq->length);
    g_display.hist_tc.Draw();
    return err;
  }
  else if ((0x8400 <= addr) && (addr + rq->length <= 0x8400 + sizeof(g_display.hist_rh.data)))
  {
    uint8_t * u8ptr = (uint8_t *)&g_display.hist_rh.data[0];
    err = HandleRw(rq, u8ptr + (addr - 0x8400), rq->length);
    g_display.hist_rh.Draw();
    return err;
  }
  else if ((0x8600 <= addr) && (addr + rq->length <= 0x8600 + sizeof(g_display.hist_pa.data)))
  {
    uint8_t * u8ptr = (uint8_t *)&g_display.hist_pa.data[0];
    err = HandleRw(rq, u8ptr + (addr - 0x8600), rq->length);
    g_display.hist_pa.Draw();
    return err;
  }

  return false;
}

void TUioSensor::SaveSetup()
{
  // no permanent storate
}

void TUioSensor::LoadSetup()
{
  // no permanent storate
}

void TUioSensor::SetRunMode(uint8_t arunmode)
{
}

void TUioSensor::Run()
{
  i2c.Run();

  aht10.Run();
  if (aht10.measure_count != aht10.prev_measure_count)
  {
    aht10_results.ic_status = aht10.ic_status;
    aht10_results.measure_count = aht10.measure_count;
    aht10_results.t_deg_x100 = aht10.t_deg_x100;
    aht10_results.rh_percent_x100 = aht10.rh_percent_x100;

    //g_display.v_tc = ((aht10_results.t_deg_x100 + 5) / 10);
    g_display.v_rh = ((aht10_results.rh_percent_x100 + 50) / 100);
    g_display.DrawValues();

    aht10.prev_measure_count = aht10.measure_count;
  }

  bmp280.Run();
  if (bmp280.measure_count != bmp280.prev_measure_count)
  {
    bmp280_results.ic_status = bmp280.ic_status;
    bmp280_results.ic_control = bmp280.ic_control;
    bmp280_results.ic_config = bmp280.ic_config;
    bmp280_results.measure_count = bmp280.measure_count;
    bmp280_results.t_deg_x100 = bmp280.t_celsius_x100;
    bmp280_results.p_pascal = bmp280.p_pascal;

    g_display.v_tc = ((bmp280_results.t_deg_x100 + 5) / 10);
    g_display.v_pa = (bmp280_results.p_pascal + 50) / 100;
    g_display.DrawValues();

    bmp280.prev_measure_count = bmp280.measure_count;
  }
}
