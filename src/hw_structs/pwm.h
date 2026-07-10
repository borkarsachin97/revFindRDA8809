// Hardware Registers for PWM at Base 0xA1A06000
#ifndef _PWM_H_
#define _PWM_H_

#include <stdint.h>
#include "../../helpfull-headers/globals.h"

typedef struct
{
    volatile uint32_t                          PWT_Config;                   //0x00000000
    volatile uint32_t                          LPG_Config;                   //0x00000004
    volatile uint32_t                          PWL0_Config;                  //0x00000008
    volatile uint32_t                          PWL1_Config;                  //0x0000000C
    volatile uint32_t                          TSC_DATA;                     //0x00000010
    volatile uint32_t                          GPADC_DATA;                   //0x00000014
} HWP_PWM_T;

#define PWM_BASE_ADDR 0xA1A06000
#define hw_pwm ((HWP_PWM_T*) PWM_BASE_ADDR)

#endif // _PWM_H_
