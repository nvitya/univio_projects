/*
 *  file:     board.h
 *  brief:    Board / MCU definition
 *  created:  2021-11-18
 *  authors:  nvitya
 *  notes:
 *    the platform.h includes this very early, it must at least define the used MCU
*/

#ifndef BOARD_H_
#define BOARD_H_

#include "boards_builtin.h"

// UnivID Device settings

#define UIO_FW_ID   "USB-SENSOR"
#define UIO_FW_VER    ((0 << 24) | (5 << 16) | 0)
#define UIO_MEM_SIZE       0x8000 // for OBJ#0002

#define UIO_MAX_DATA_LEN     1024
#define UIO_MPRAM_SIZE       4096

#endif /* BOARD_H_ */
