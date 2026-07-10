// Hardware Registers for SYS_IFC at Base 0xA1A09000
#ifndef _SYS_IFC_H_
#define _SYS_IFC_H_

#include <stdint.h>
#include "../../helpfull-headers/globals.h"

typedef struct
{
    volatile uint32_t                          get_ch;                       //0x00000000
    volatile uint32_t                          dma_status;                   //0x00000004
    volatile uint32_t                          debug_status;                 //0x00000008
    volatile uint32_t Reserved_0000000C;                    //0x0000000C
    struct
    {
        volatile uint32_t                      control;                      //0x00000010
        volatile uint32_t                      status;                       //0x00000014
        volatile uint32_t                      start_addr;                   //0x00000018
        volatile uint32_t                      tc;                           //0x0000001C
    } std_ch[SYS_IFC_STD_CHAN_NB];
    // volatile uint32_t Reserved_00000080;                 //0x00000080
    volatile uint32_t                          ch_rfspi_control;             //0x00000080
    volatile uint32_t                          ch_rfspi_status;              //0x00000084
    volatile uint32_t                          ch_rfspi_start_addr;          //0x00000088
    volatile uint32_t                          ch_rfspi_end_addr;            //0x0000008C
    volatile uint32_t                          ch_rfspi_tc;                  //0x00000090
} HWP_SYS_IFC_T;

#define SYS_IFC_BASE_ADDR 0xA1A09000
#define hw_sys_ifc ((HWP_SYS_IFC_T*) SYS_IFC_BASE_ADDR)

#endif // _SYS_IFC_H_
