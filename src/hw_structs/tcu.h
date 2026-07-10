// Hardware Registers for TCU at Base 0xA1A0F000
#ifndef _TCU_H_
#define _TCU_H_

#include <stdint.h>

typedef struct
{
    volatile uint32_t                          Ctrl;                         //0x00000000
    volatile uint32_t                          Wrap_Val;                     //0x00000004
    volatile uint32_t                          Cur_Val;                      //0x00000008
    volatile uint32_t                          Latch;                        //0x0000000C
    volatile uint32_t                          Setup;                        //0x00000010
    volatile uint32_t                          Disable_Event;                //0x00000014
    volatile uint32_t                          Enable_Event;                 //0x00000018
    volatile uint32_t                          Set_TCO;                      //0x0000001C
    volatile uint32_t                          Clr_TCO;                      //0x00000020
    volatile uint32_t                          Cfg_Clk_Div;                  //0x00000024
    volatile uint32_t                          LPS_SF_Ctrl;                  //0x00000028
    volatile uint32_t                          LPS_SF_Status;                //0x0000002C
    volatile uint32_t                          LPS_SF_Frames;                //0x00000030
    volatile uint32_t                          LPS_SF_Restart_Time;          //0x00000034
    volatile uint32_t                          LPS_SF_Frame_Period;          //0x00000038
    /// The rate is the number of System Clocks per 32kHz Clocks.
    volatile uint32_t                          LPS_SF_Rate;                  //0x0000003C
    volatile uint32_t                          LPS_SF_Elapsed_Frames;        //0x00000040
    volatile uint32_t                          LPS_SF_Sys_Count;             //0x00000044
    volatile uint32_t                          LPS_PU_Ctrl;                  //0x00000048
    volatile uint32_t                          LPS_PU_Status;                //0x0000004C
    volatile uint32_t                          LPS_PU_Times;                 //0x00000050
    volatile uint32_t                          LPS_PU_IRQ;                   //0x00000054
    volatile uint32_t                          LPS_PU_IRQ_Mask;              //0x00000058
    volatile uint32_t Reserved_0000005C[41];                //0x0000005C
    volatile uint32_t                          Event[NB_TCU_PROG_EVENTS];    //0x00000100
} HWP_TCU_T;

#define TCU_BASE_ADDR 0xA1A0F000
#define hw_tcu ((HWP_TCU_T*) TCU_BASE_ADDR)

#endif // _TCU_H_
