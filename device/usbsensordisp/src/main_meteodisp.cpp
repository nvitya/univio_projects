// file:     main_meteodisp.cpp
// brief:    USB Meteo Display Main
// created:  2022-03-04
// authors:  nvitya

#include "platform.h"
#include "cppinit.h"
#include "clockcnt.h"

#include "hwclk.h"
#include "hwpins.h"
#include "hwuart.h"
#include "traces.h"

#include "board_pins.h"
#include "hwsdram.h"
#include "mscounter.h"
#include "sensor_display.h"
#include "uio_device.h"
#include "usb_univio.h"

#if SPI_SELF_FLASHING
  #include "spi_self_flashing.h"
#endif

volatile unsigned hbcounter = 0;

extern "C" __attribute__((noreturn)) void _start(unsigned self_flashing)  // self_flashing = 1: self-flashing required for RAM-loaded applications
{
  // after ram setup and region copy the cpu jumps here, with probably RC oscillator
  mcu_disable_interrupts();

  // Set the interrupt vector table offset, so that the interrupts and exceptions work
  mcu_init_vector_table();

  // run the C/C++ initialization (variable initializations, constructors)
  cppinit();

  if (!hwclk_init(EXTERNAL_XTAL_HZ, MCU_CLOCK_SPEED))  // if the EXTERNAL_XTAL_HZ == 0, then the internal RC oscillator will be used
  {
    while (1)
    {
      // error
    }
  }

	mcu_enable_fpu();    // enable coprocessor if present
	mcu_enable_icache(); // enable instruction cache if present

	clockcnt_init();

	// go on with the hardware initializations
	board_pins_init();
	traces_init();

	TRACE("\r\n--------------------------------------\r\n");
	TRACE("USB Sensor with Display\r\n");
	TRACE("Board: %s\r\n", BOARD_NAME);
	TRACE("SystemCoreClock: %u\r\n", SystemCoreClock);

  #if SPI_SELF_FLASHING
    if (spiflash.initialized)
    {
      TRACE("SPI Flash ID CODE: %08X, size = %u\r\n", spiflash.idcode, spiflash.bytesize);
      if (self_flashing)
      {
        spi_self_flashing(&spiflash);
      }
    }
    else
    {
      TRACE("Error initializing SPI Flash !\r\n");
    }
  #endif

  mcu_enable_interrupts();

  mscounter_init();

	g_display.Init();

  g_uiodev.Init();

  usb_device_init();

  TRACE("Starting main loop.\r\n");

	unsigned hbclocks = SystemCoreClock / 20;  // start blinking fast

	unsigned t0, t1;

	t0 = CLOCKCNT;

	// Infinite loop
	while (1)
	{
		t1 = CLOCKCNT;

    usb_device_run();

    g_uiodev.Run();

    tracebuf.Run();

		g_display.Run();

		if (t1-t0 > hbclocks)
		{
			++hbcounter;

      for (unsigned n = 0; n < pin_led_count; ++n)
      {
        pin_led[n].SetTo((hbcounter >> n) & 1);
      }

			t0 = t1;

			if (hbcounter > 20)  hbclocks = SystemCoreClock / 2;  // slow down to 0.5 s
		}
	}
}

// ----------------------------------------------------------------------------
