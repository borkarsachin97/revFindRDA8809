// Hardware Registers for SPI_FLASH at Base 0xA1A25000
#ifndef _SPI_FLASH_H_
#define _SPI_FLASH_H_

#include <stdint.h>

typedef struct
{
    volatile uint32_t                          spi_cmd_addr;                 //0x00000000
    volatile uint32_t                          spi_block_size;               //0x00000004
    volatile uint32_t                          spi_data_fifo_wo;             //0x00000008
    volatile uint32_t                          spi_data_fifo_ro;             //0x0000000C
    volatile uint32_t                          spi_read_back;                //0x00000010
    volatile uint32_t                          spi_config;                   //0x00000014
    volatile uint32_t                          spi_fifo_control;             //0x00000018
    volatile uint32_t                          spi_cs_size;                  //0x0000001C
    volatile uint32_t                          spi_read_cmd;                 //0x00000020
    volatile uint32_t                          spi_flash_24;                 //0x00000024
    volatile uint32_t                          spi_flash_28;                 //0x00000028
    volatile uint32_t                          spi_flash_2c;                 //0x0000002C
    volatile uint32_t                          spi_flash_30;                 //0x00000030
    volatile uint32_t                          spi_flash_34;                 //0x00000034
} HWP_SPI_FLASH_T;

#define SPI_FLASH_BASE_ADDR 0xA1A25000
#define hw_spi_flash ((HWP_SPI_FLASH_T*) SPI_FLASH_BASE_ADDR)

#endif // _SPI_FLASH_H_
