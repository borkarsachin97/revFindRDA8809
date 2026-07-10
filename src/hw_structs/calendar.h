// Hardware Registers for CALENDAR at Base 0xA1A0A000
#ifndef _CALENDAR_H_
#define _CALENDAR_H_

#include <stdint.h>

typedef struct
{
    volatile uint32_t                          Ctrl;                         //0x00000000
    volatile uint32_t                          Cmd;                          //0x00000004
    volatile uint32_t                          Status;                       //0x00000008
    volatile uint32_t                          Calendar_LoadVal_L;           //0x0000000C
    volatile uint32_t                          Calendar_LoadVal_H;           //0x00000010
    volatile uint32_t                          Calendar_CurVal_L;            //0x00000014
    volatile uint32_t                          Calendar_CurVal_H;            //0x00000018
    volatile uint32_t                          AlarmVal_L;                   //0x0000001C
    volatile uint32_t                          AlarmVal_H;                   //0x00000020
} HWP_CALENDAR_T;

#define CALENDAR_BASE_ADDR 0xA1A0A000
#define hw_calendar ((HWP_CALENDAR_T*) CALENDAR_BASE_ADDR)

#endif // _CALENDAR_H_
