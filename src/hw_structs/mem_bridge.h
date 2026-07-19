// Hardware Registers for MEM_BRIDGE at Base 0xA1A04000
#ifndef _MEM_BRIDGE_H_
#define _MEM_BRIDGE_H_

#include <stdint.h>
#include "../../helpfull-headers/globals.h"

typedef struct
{
    volatile uint32_t                          FIFO_Ctrl;                    //0x00000000
    volatile uint32_t                          FIFO_Status;                  //0x00000004
    volatile uint32_t                          Monitor_Ctrl;                 //0x00000008
    volatile uint32_t                          Rom_Bist;                     //0x0000000C
    volatile uint32_t                          SRam_Bist;                    //0x00000010
    volatile uint32_t Reserved_00000014[59];                //0x00000014
    volatile uint32_t                          Rom_Patch[NB_ROM_PACHT];      //0x00000100
    volatile uint32_t Reserved_00000140[48];                //0x00000140
    volatile uint32_t Reserved_00000200[64];                //0x00000200
    volatile uint32_t Reserved_00000300[64];                //0x00000300
    volatile uint32_t                          EBC_Ctrl;                     //0x00000400
    volatile uint32_t                          EBC_Status;                   //0x00000404
    volatile uint32_t Reserved_00000408;                    //0x00000408
    /// This register is used to program the CS_Time register of One CS_Config at
    /// the same time as the CS_Mode regiter is set (so the two registers are always
    /// synchronized with each other).
    /// To update Chip Select n configuration, first write the timing value in this
    /// register, then write the CS_Mode configuration in the corresponding CS_Config[n].CS_Mode.
    volatile uint32_t                          CS_Time_Write;                //0x0000040C
    struct
    {
        /// Writing this register will also copy the content of CS_Time_Write to
        /// the CS_Time register.
        volatile uint32_t                      CS_Mode;                      //0x00000410
        /// This register cannot be written direclty. To update it, write to the
        /// CS_Time_write register, then to the CS_Mode register.
        volatile uint32_t                      CS_Time;                      //0x00000414
    } CS_Config[EBC_NB_CS];
    volatile uint32_t Reserved_00000438[50];                //0x00000438
    volatile uint32_t                          AHBM_Ctrl;                    //0x00000500
    volatile uint32_t                          AHBM_Status;                  //0x00000504
    volatile uint32_t Reserved_00000508[2];                 //0x00000508
    volatile uint32_t                          SpaceBase[EBC_NB_CS];         //0x00000510
} HWP_MEM_BRIDGE_T;

#define MEM_BRIDGE_BASE_ADDR 0xA1A04000
#define hw_mem_bridge ((HWP_MEM_BRIDGE_T*) MEM_BRIDGE_BASE_ADDR)

#endif // _MEM_BRIDGE_H_
