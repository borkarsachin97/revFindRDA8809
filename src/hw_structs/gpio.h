// Hardware Registers for GPIO at Base 0xA1A03000
#ifndef _GPIO_H_
#define _GPIO_H_

#include <stdint.h>

typedef struct
{
    volatile uint32_t                          gpio_oen_val;                 //0x00000000
    volatile uint32_t                          gpio_oen_set_out;             //0x00000004
    volatile uint32_t                          gpio_oen_set_in;              //0x00000008
    volatile uint32_t                          gpio_val;                     //0x0000000C
    volatile uint32_t                          gpio_set;                     //0x00000010
    volatile uint32_t                          gpio_clr;                     //0x00000014
    volatile uint32_t                          gpint_ctrl_set;               //0x00000018
    volatile uint32_t                          gpint_ctrl_clr;               //0x0000001C
    volatile uint32_t                          int_clr;                      //0x00000020
    volatile uint32_t                          int_status;                   //0x00000024
    volatile uint32_t                          chg_ctrl;                     //0x00000028
    volatile uint32_t                          chg_cmd;                      //0x0000002C
    volatile uint32_t                          gpo_set;                      //0x00000030
    volatile uint32_t                          gpo_clr;                      //0x00000034
} HWP_GPIO_T;

#define GPIO_BASE_ADDR 0xA1A03000
#define hw_gpio ((HWP_GPIO_T*) GPIO_BASE_ADDR)

#endif // _GPIO_H_
