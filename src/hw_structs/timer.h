// Hardware Registers for TIMER at Base 0xA1A02000
#ifndef _TIMER_H_
#define _TIMER_H_

#include <stdint.h>

typedef struct
{
    volatile uint32_t                          OSTimer_Ctrl;                 //0x00000000
    volatile uint32_t                          OSTimer_CurVal;               //0x00000004
    volatile uint32_t                          WDTimer_Ctrl;                 //0x00000008
    volatile uint32_t                          WDTimer_LoadVal;              //0x0000000C
    volatile uint32_t                          HWTimer_Ctrl;                 //0x00000010
    volatile uint32_t                          HWTimer_CurVal;               //0x00000014
    volatile uint32_t                          Timer_Irq_Mask_Set;           //0x00000018
    volatile uint32_t                          Timer_Irq_Mask_Clr;           //0x0000001C
    volatile uint32_t                          Timer_Irq_Clr;                //0x00000020
    volatile uint32_t                          Timer_Irq_Cause;              //0x00000024
} HWP_TIMER_T;

#define TIMER_BASE_ADDR 0xA1A02000
#define hw_timer ((HWP_TIMER_T*) TIMER_BASE_ADDR)

#endif // _TIMER_H_
