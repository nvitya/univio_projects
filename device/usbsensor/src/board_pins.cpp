/*
 *  file:     board_pins.cpp (i2c_eeprom)
 *  brief:    I2C EEPROM Test Board pins
 *  version:  1.00
 *  date:     2021-11-01
 *  authors:  nvitya
*/

#include "board_pins.h"
#include "clockcnt.h"

THwI2c         i2c;
THwDmaChannel  i2c_txdma;
THwDmaChannel  i2c_rxdma;

THwUart   conuart;  // console uart
unsigned  pin_led_count = 1;

TGpioPin  pin_led[MAX_LEDS] =
{
  TGpioPin(),
  TGpioPin(),
  TGpioPin(),
  TGpioPin()
};

/* NOTE:
     for direct GPIO pin definitions is simpler to define with port and pin number:

       TGpioPin  pin_mygpio(PORTNUM_C, 13, false);

     but don't forget to initialize it in the setup code:

       pin_mygpio.Setup(PINCFG_OUTPUT | PINCFG_GPIO_INIT_0);
*/


#if 0  // to use elif everywhere

//-------------------------------------------------------------------------------
// Risc-V (RV32I)
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
// ARM Cortex-M
//-------------------------------------------------------------------------------

// STM32

#elif defined(BOARD_MIN_F103)

void board_pins_init_target()
{
  pin_led_count = 1;
  pin_led[0].Assign(PORTNUM_C, 13, true);

  // USART1
  hwpinctrl.PinSetup(PORTNUM_A,  9,  PINCFG_OUTPUT | PINCFG_AF_7);  // USART1_TX
  hwpinctrl.PinSetup(PORTNUM_A, 10,  PINCFG_INPUT  | PINCFG_AF_7 | PINCFG_PULLUP);  // USART1_RX
  conuart.Init(1);

  // I2C1
  // open drain mode have to be used, otherwise it won't work
  // External pull-ups are required !
  hwpinctrl.PinSetup(PORTNUM_B,  6, PINCFG_AF_4 | PINCFG_OPENDRAIN | PINCFG_SPEED_FAST); // I2C1_SCL
  hwpinctrl.PinSetup(PORTNUM_B,  7, PINCFG_AF_4 | PINCFG_OPENDRAIN | PINCFG_SPEED_FAST); // I2C1_SDA
  i2c.speed = 100000; // 100 kHz
  i2c.Init(1); // I2C1

  #if 1 // use DMA
    i2c_txdma.Init(1, 6, 0);  // dma1,ch6 = I2C1 TX
    i2c_rxdma.Init(1, 7, 0);  // dma1,ch5 = I2C1 RX

    i2c.DmaAssign(true,  &i2c_txdma);
    i2c.DmaAssign(false, &i2c_rxdma);
  #endif

  // Other Pin inits is not necessary here, because all pins will be initialized later to passive
  // before the config loading happens

  // USB RE-CONNECT

  // The Blue Pill has a fix external pull-up on the USB D+ = PA12, which always signalizes a connected device
  // in order to reinit the device upon restart we pull this down:

  hwpinctrl.PinSetup(PORTNUM_A, 12, PINCFG_OUTPUT | PINCFG_OPENDRAIN | PINCFG_GPIO_INIT_0);
  delay_us(10000); // 10 ms
  hwpinctrl.PinSetup(PORTNUM_A, 12, PINCFG_AF_0);
}

#elif  defined(BOARD_MIN_F401) || defined(BOARD_MIN_F411) \
      //|| defined(BOARD_MIBO64_STM32F405)

void board_pins_init_target()
{
  pin_led_count = 1;
  pin_led[0].Assign(PORTNUM_C, 13, false);
  board_pins_init_leds();

  // USART1
  hwpinctrl.PinSetup(PORTNUM_A,  9,  PINCFG_OUTPUT | PINCFG_AF_7);  // USART1_TX
  hwpinctrl.PinSetup(PORTNUM_A, 10,  PINCFG_INPUT  | PINCFG_AF_7 | PINCFG_PULLUP);  // USART1_RX
  conuart.Init(1);

  // I2C1
  // open drain mode have to be used, otherwise it won't work
  hwpinctrl.PinSetup(PORTNUM_B,  6, PINCFG_OUTPUT | PINCFG_AF_4 | PINCFG_OPENDRAIN); // I2C1_SCL
  hwpinctrl.PinSetup(PORTNUM_B,  7, PINCFG_OUTPUT | PINCFG_AF_4 | PINCFG_OPENDRAIN); // I2C1_SDA
  i2c.speed = 100000; // 100 kHz
  i2c.Init(1); // I2C1

  #if 1
    i2c_txdma.Init(1, 6, 1);  // dma1,stream6,ch1 = I2C1 TX
    i2c_rxdma.Init(1, 5, 1);  // dma1,stream5,ch1 = I2C1 RX

    i2c.DmaAssign(true,  &i2c_txdma);
    i2c.DmaAssign(false, &i2c_rxdma);
  #endif
}

#elif defined(BOARD_MIBO48_STM32G473)

void board_pins_init_target()
{
  pin_led_count = 1;
  pin_led[0].Assign(PORTNUM_C, 13, false);
  board_pins_init_leds();


  // USART1
  hwpinctrl.PinSetup(PORTNUM_A,  9,  PINCFG_OUTPUT | PINCFG_AF_7);  // USART1_TX
  hwpinctrl.PinSetup(PORTNUM_A, 10,  PINCFG_INPUT  | PINCFG_AF_7 | PINCFG_PULLUP);  // USART1_RX
  conuart.Init(1);

  // I2C1
  // open drain mode have to be used, otherwise it won't work

  // WARNING: the B6 pin did not work (was always low after enabling the AF_4)
  //          this is due the USB-C power delivery pull down functionality
  //          turning off did not work for me this way:
  //  RCC->APB1ENR1 |= RCC_APB1ENR1_PWREN;
  //  PWR->CR3 |= PWR_CR3_UCPD_DBDIS;
  //  hwpinctrl.PinSetup(PORTNUM_B,  6, PINCFG_OUTPUT | PINCFG_AF_4 | PINCFG_OPENDRAIN); // I2C1_SCL
  //  hwpinctrl.PinSetup(PORTNUM_B,  7, PINCFG_OUTPUT | PINCFG_AF_4 | PINCFG_OPENDRAIN); // I2C1_SDA

  // Warning: PB8 = boot pin ! must be high at reset otherwise the Flash code does not start
  hwpinctrl.PinSetup(PORTNUM_B,  8, PINCFG_OUTPUT | PINCFG_AF_4 | PINCFG_OPENDRAIN); // I2C1_SCL
  hwpinctrl.PinSetup(PORTNUM_B,  9, PINCFG_OUTPUT | PINCFG_AF_4 | PINCFG_OPENDRAIN); // I2C1_SDA

  i2c.speed = 100000; // 100 kHz
  i2c.Init(1); // I2C1

  #if 1
    i2c_txdma.Init(1, 6, 17);  // dmamux17 = I2C1 TX
    i2c_rxdma.Init(1, 5, 16);  // dmamux16 = I2C1 RX

    i2c.DmaAssign(true,  &i2c_txdma);
    i2c.DmaAssign(false, &i2c_rxdma);
  #endif
}

// ATSAM

#elif defined(BOARD_ARDUINO_DUE)

void board_pins_init_target()
{
  pin_led_count = 1;
  pin_led[0].Assign(PORTNUM_B, 27, false);
  board_pins_init_leds();

  // UART - On the Arduino programmer interface
  hwpinctrl.PinSetup(PORTNUM_A, 8, PINCFG_INPUT | PINCFG_AF_0);  // UART_RXD
  hwpinctrl.PinSetup(PORTNUM_A, 9, PINCFG_OUTPUT | PINCFG_AF_0); // UART_TXD
  conuart.Init(0);  // UART

  #define TWI_NUM  1

  #if 0 == TWI_NUM
    // TWI0 (I2C0), the first two pins closest to the reset button
    hwpinctrl.PinSetup(PORTNUM_A, 17, PINCFG_AF_A);  // TWI0_SDA (TWD)
    hwpinctrl.PinSetup(PORTNUM_A, 18, PINCFG_AF_A);  // TWI0_SCL (TWCK)
    i2c.speed = 100000; // 100 kHz
    i2c.Init(0);

    #if 1
      i2c_txdma.Init(2, 7);
      i2c_rxdma.Init(3, 8);

      i2c.DmaAssign(true,  &i2c_txdma);
      i2c.DmaAssign(false, &i2c_rxdma);
    #endif

  #elif 1 == TWI_NUM
    // TWI1 (I2C1), the pins 20, 21
    hwpinctrl.PinSetup(PORTNUM_B, 12, PINCFG_AF_A);  // TWI1_SDA (TWD)
    hwpinctrl.PinSetup(PORTNUM_B, 13, PINCFG_AF_A);  // TWI1_SCL (TWCK)
    i2c.speed = 100000; // 100 kHz
    i2c.Init(1);

    #if 1
      i2c.PdmaInit(true,  &i2c_txdma);
      i2c.PdmaInit(false, &i2c_rxdma);
    #endif
  #else
    #error "invalid TWI / I2C"
  #endif
}

#elif defined(BOARD_MIBO64_ATSAM4S)

void board_pins_init_target()
{
  pin_led_count = 1;
  pin_led[0].Assign(PORTNUM_A, 1, false);
  board_pins_init_leds();

  hwpinctrl.PinSetup(PORTNUM_A,  9,  PINCFG_INPUT  | PINCFG_AF_0);  // UART0_RX
  hwpinctrl.PinSetup(PORTNUM_A, 10,  PINCFG_OUTPUT | PINCFG_AF_0);  // UART0_TX
  conuart.Init(0);
}

#elif defined(BOARD_MIBO64_ATSAME5X)

void board_pins_init_target()
{
  pin_led_count = 1;
  pin_led[0].Assign(PORTNUM_A, 1, false);
  board_pins_init_leds();

  // SERCOM0
  hwpinctrl.PinSetup(PORTNUM_A, 4, PINCFG_OUTPUT | PINCFG_AF_3);  // PAD[0] = TX
  hwpinctrl.PinSetup(PORTNUM_A, 5, PINCFG_INPUT  | PINCFG_AF_3);  // PAD[1] = RX
  conuart.Init(0);

  // SERCOM4
  hwpinctrl.PinSetup(PORTNUM_B,  8, PINCFG_AF_D | PINCFG_PULLUP); // SERCOM4/PAD0 = SDA
  hwpinctrl.PinSetup(PORTNUM_B,  9, PINCFG_AF_D | PINCFG_PULLUP); // SERCOM4/PAD1 = SCL
  i2c.Init(4); // SERCOM4

  #if 1
    i2c_txdma.Init(14, SERCOM4_DMAC_ID_TX);
    i2c_rxdma.Init(15, SERCOM4_DMAC_ID_RX);

    i2c.DmaAssign(true,  &i2c_txdma);
    i2c.DmaAssign(false, &i2c_rxdma);
  #endif
}

#else
  #error "Define board_pins_init here"
#endif



void board_pins_init()
{
  board_pins_init_target();

  // init LEDS
  for (unsigned n = 0; n < pin_led_count; ++n)
  {
    pin_led[n].Setup(PINCFG_OUTPUT | PINCFG_GPIO_INIT_0);
    pin_led[n].Set0();
  }
}
