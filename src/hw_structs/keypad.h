// Hardware Registers for KEYPAD at Base 0xA1A05000
#ifndef _KEYPAD_H_
#define _KEYPAD_H_

#include <stdint.h>
#include "../../helpfull-headers/globals.h"

typedef struct
{
    volatile uint32_t                          KP_DATA_L;                    //0x00000000
    volatile uint32_t                          KP_DATA_H;                    //0x00000004
    volatile uint32_t                          KP_STATUS;                    //0x00000008
    volatile uint32_t                          KP_CTRL;                      //0x0000000C
    volatile uint32_t                          KP_IRQ_MASK;                  //0x00000010
    volatile uint32_t                          KP_IRQ_CAUSE;                 //0x00000014
    volatile uint32_t                          KP_IRQ_CLR;                   //0x00000018
} HWP_KEYPAD_T;

#define KEYPAD_BASE_ADDR 0xA1A05000
#define hw_keypad ((HWP_KEYPAD_T*) KEYPAD_BASE_ADDR)

#endif // _KEYPAD_H_
