/*
 *  file:     board.h (F407VG)
 *  brief:    STM32F407VG Board definition for the usbsensordisp project
 *  created:  2022-06-12
 *  authors:  nvitya
*/

#ifndef BOARD_H_
#define BOARD_H_

#define PRINTF_SUPPORT_FLOAT

// CPU Defintion
#define BOARD_NAME "STM32F407VG Minimal Board"
#define MCU_STM32F407VG
#define EXTERNAL_XTAL_HZ   8000000

//#define DISP_IF_SPI
//#define DISP_IF_FRAMEBUFFER
//#define DISP_IF_GP16
#define DISP_IF_MM16

#define DISP_WIDTH     480
#define DISP_HEIGHT    320
#define DISP_ROTATION    1

#endif /* BOARD_H_ */
