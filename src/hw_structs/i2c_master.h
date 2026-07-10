// Hardware Registers for I2C_MASTER at Base 0xA1A07000
#ifndef _I2C_MASTER_H_
#define _I2C_MASTER_H_

#include <stdint.h>

typedef struct
{
    volatile uint32_t                          CTRL;                         //0x00000000
    volatile uint32_t                          STATUS;                       //0x00000004
    volatile uint32_t                          TXRX_BUFFER;                  //0x00000008
    volatile uint32_t                          CMD;                          //0x0000000C
    volatile uint32_t                          IRQ_CLR;                      //0x00000010
} HWP_I2C_MASTER_T;

#define I2C_MASTER_BASE_ADDR 0xA1A07000
#define hw_i2c_master ((HWP_I2C_MASTER_T*) I2C_MASTER_BASE_ADDR)

#endif // _I2C_MASTER_H_
