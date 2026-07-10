// Hardware Registers for SYS_COMREGS at Base 0xA1A0B000
#ifndef _SYS_COMREGS_H_
#define _SYS_COMREGS_H_

#include <stdint.h>

typedef struct
{
    volatile uint32_t                          Snapshot;                     //0x00000000
    volatile uint32_t                          Snapshot_Cfg;                 //0x00000004
    volatile uint32_t                          Cause;                        //0x00000008
    volatile uint32_t                          Mask_Set;                     //0x0000000C
    volatile uint32_t                          Mask_Clr;                     //0x00000010
    /// If accesses to ItReg_Set and ItReg_Clr registers are done simultaneously
    /// from both CPUs and affecting the same bits, the priority is given to set
    /// a bit.
    volatile uint32_t                          ItReg_Set;                    //0x00000014
    /// If accesses to ItReg_Set and ItReg_Clr registers are done simultaneously
    /// from both CPUs and affecting the same bits, the priority is given to set
    /// a bit.
    volatile uint32_t                          ItReg_Clr;                    //0x00000018
} HWP_COMREGS_T;

#define SYS_COMREGS_BASE_ADDR 0xA1A0B000
#define hw_sys_comregs ((HWP_COMREGS_T*) SYS_COMREGS_BASE_ADDR)

#endif // _SYS_COMREGS_H_
