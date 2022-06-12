/*
 *  file:     board_pins_f407vg.cpp
 *  brief:    Board interface
 *  created:  2022-06-12
 *  authors:  nvitya
*/

#include "board_pins.h"
#include "hwsdram.h"
#include "hwlcdctrl.h"
#include "clockcnt.h"

THwUart   conuart;  // console uart
unsigned  pin_led_count = 1;

TGpioPin  pin_led[MAX_LEDS] =
{
  TGpioPin(),
  TGpioPin(),
  TGpioPin(),
  TGpioPin()
};

void board_pins_init_leds()
{
  for (unsigned n = 0; n < pin_led_count; ++n)
  {
    pin_led[n].Setup(PINCFG_OUTPUT | PINCFG_GPIO_INIT_0);
  }
}

TTftLcd_mm16_impl  disp;

TGpioPin  lcd_reset(PORTNUM_C, 6, false);

void board_pins_init()
{
  pin_led_count = 1;
  pin_led[0].Assign(PORTNUM_E, 0, false);
  board_pins_init_leds();

  lcd_reset.Setup(PINCFG_OUTPUT | PINCFG_GPIO_INIT_0);
  lcd_reset.Set0();

  delay_ms(10);

  lcd_reset.Set1(); // remove reset

  delay_ms(20);

  // USART1
  hwpinctrl.PinSetup(PORTNUM_A,  9,  PINCFG_OUTPUT | PINCFG_AF_7);  // USART1_TX
  hwpinctrl.PinSetup(PORTNUM_A, 10,  PINCFG_INPUT  | PINCFG_AF_7);  // USART1_RX
  conuart.Init(1);

  disp.mirrorx = true;
  //disp.Init(LCD_CTRL_HX8357B, 480, 320);
  //disp.Init(LCD_CTRL_UNKNOWN, 480, 320);
  disp.Init(LCD_CTRL_HX8357B, 320, 480);
  disp.SetRotation(3);
}

