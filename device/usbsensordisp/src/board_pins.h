/*
 *  file:     board_pins.h
 *  brief:    Common board interface definition
 *  created:  2022-06-12
 *  authors:  nvitya
*/

#ifndef SRC_BOARD_PINS_H_
#define SRC_BOARD_PINS_H_

#include "hwpins.h"
#include "hwuart.h"

#define MAX_LEDS  4

extern TGpioPin   pin_led[MAX_LEDS];
extern unsigned   pin_led_count;

extern THwUart    conuart;  // console uart

#if SPI_SELF_FLASHING
  #include "spiflash.h"
  extern TSpiFlash  spiflash;
#endif

void board_pins_init();

//----------------------------------------------------------------------------------------------------
// SPI DISPLAYS with SMALLER MCUs
//----------------------------------------------------------------------------------------------------

#if    defined(DISP_IF_SPI)

#include "tftlcd_spi.h"

extern TTftLcd_spi  disp;

//----------------------------------------------------------------------------------------------------
// EMBEDDED LCD CONROLLER WITH FRAMEBUFFER
//----------------------------------------------------------------------------------------------------

#elif defined(DISP_IF_FRAMEBUFFER)

#include "framebuffer16.h"

extern TFrameBuffer16  disp;

//----------------------------------------------------------------------------------------------------
// PARALLEL DISPLAYS
//----------------------------------------------------------------------------------------------------

#elif defined(DISP_IF_MM16)

#include "tftlcd_mm16_impl.h"

extern TTftLcd_mm16_impl  disp;

#elif defined(DISP_IF_GP16)

#include "tftlcd_gp16_impl.h"

extern TTftLcd_gp16_impl  disp;

#else
  #error "Display type not defined."
#endif

#ifdef DISP_IF_SPI

void init_spi_display();

#endif

#endif /* SRC_BOARD_PINS_H_ */
