// Hardware Registers for DMA at Base 0xA1A08000
#ifndef _DMA_H_
#define _DMA_H_

#include <stdint.h>

typedef struct
{
    volatile uint32_t                          get_channel;                  //0x00000000
    volatile uint32_t                          status;                       //0x00000004
    volatile uint32_t                          control;                      //0x00000008
    volatile uint32_t                          src_addr;                     //0x0000000C
    volatile uint32_t                          dst_addr;                     //0x00000010
    volatile uint32_t                          sd_dst_addr;                  //0x00000014
    volatile uint32_t                          xfer_size;                    //0x00000018
    volatile uint32_t                          pattern;                      //0x0000001C
    volatile uint32_t                          gea_kc_low;                   //0x00000020
    volatile uint32_t                          gea_kc_high;                  //0x00000024
    volatile uint32_t                          gea_messkey;                  //0x00000028
    volatile uint32_t                          fcs;                          //0x0000002C
} HWP_DMA_T;

#define DMA_BASE_ADDR 0xA1A08000
#define hw_dma ((HWP_DMA_T*) DMA_BASE_ADDR)

#endif // _DMA_H_
