// Hardware Registers for PAGE_SPY at Base 0xA1A0C000
#ifndef _PAGE_SPY_H_
#define _PAGE_SPY_H_

#include <stdint.h>

typedef struct
{
    volatile uint32_t                          enable;                       //0x00000000
    volatile uint32_t                          status;                       //0x00000004
    volatile uint32_t                          disable;                      //0x00000008
    volatile uint32_t Reserved_0000000C;                    //0x0000000C
    struct
    {
        volatile uint32_t                      start;                        //0x00000010
        volatile uint32_t                      end;                          //0x00000014
        volatile uint32_t                      master;                       //0x00000018
        volatile uint32_t                      addr;                         //0x0000001C
    } page[PAGE_SPY_NB_PAGE];
} HWP_PAGE_SPY_T;

#define PAGE_SPY_BASE_ADDR 0xA1A0C000
#define hw_page_spy ((HWP_PAGE_SPY_T*) PAGE_SPY_BASE_ADDR)

#endif // _PAGE_SPY_H_
