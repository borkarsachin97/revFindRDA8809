// Hardware Registers for RF_SPI at Base 0xA1A0E000
#ifndef _RF_SPI_H_
#define _RF_SPI_H_

#include <stdint.h>

typedef struct
{
    volatile uint32_t                          Ctrl;                         //0x00000000
    volatile uint32_t                          Status;                       //0x00000004
    volatile uint32_t                          Rx_Data;                      //0x00000008
    volatile uint32_t                          Command;                      //0x0000000C
    volatile uint32_t                          Cmd_Size;                     //0x00000010
    volatile uint32_t                          Cmd_Data;                     //0x00000014
    volatile uint32_t                          Gain_Size;                    //0x00000018
    volatile uint32_t                          Gain_Data;                    //0x0000001C
    volatile uint32_t                          IRQ;                          //0x00000020
    volatile uint32_t                          IRQ_Mask;                     //0x00000024
    volatile uint32_t                          IRQ_Threshold;                //0x00000028
    volatile uint32_t                          Divider;                      //0x0000002C
} HWP_RF_SPI_T;

#define RF_SPI_BASE_ADDR 0xA1A0E000
#define hw_rf_spi ((HWP_RF_SPI_T*) RF_SPI_BASE_ADDR)

#endif // _RF_SPI_H_
