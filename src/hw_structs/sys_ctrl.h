// Hardware Registers for SYS_CTRL at Base 0xA1A00000
#ifndef _SYS_CTRL_H_
#define _SYS_CTRL_H_

#include <stdint.h>

typedef struct
{
    /// <strong>This register is used to Lock and Unlock the protected registers.</strong>
    volatile uint32_t                          REG_DBG;                      //0x00000000
    /// This register is protected.
    volatile uint32_t                          Sys_Rst_Set;                  //0x00000004
    volatile uint32_t                          Sys_Rst_Clr;                  //0x00000008
    /// This register is protected.
    volatile uint32_t                          BB_Rst_Set;                   //0x0000000C
    volatile uint32_t                          BB_Rst_Clr;                   //0x00000010
    volatile uint32_t                          Clk_Sys_Mode;                 //0x00000014
    volatile uint32_t                          Clk_Sys_Enable;               //0x00000018
    /// This register is protected.
    volatile uint32_t                          Clk_Sys_Disable;              //0x0000001C
    volatile uint32_t                          Clk_Per_Mode;                 //0x00000020
    volatile uint32_t                          Clk_Per_Enable;               //0x00000024
    /// This register is protected.
    volatile uint32_t                          Clk_Per_Disable;              //0x00000028
    volatile uint32_t                          Clk_BB_Mode;                  //0x0000002C
    volatile uint32_t                          Clk_BB_Enable;                //0x00000030
    /// This register is protected.
    volatile uint32_t                          Clk_BB_Disable;               //0x00000034
    volatile uint32_t                          Clk_Other_Mode;               //0x00000038
    volatile uint32_t                          Clk_Other_Enable;             //0x0000003C
    /// This register is protected.
    volatile uint32_t                          Clk_Other_Disable;            //0x00000040
    /// Register protected by Write_Unlocked_H.
    volatile uint32_t                          Pll_Ctrl;                     //0x00000044
    /// This register is protected.
    volatile uint32_t                          Sel_Clock;                    //0x00000048
    volatile uint32_t                          Cfg_Clk_Sys;                  //0x0000004C
    volatile uint32_t                          Cfg_Clk_Mem_Bridge;           //0x00000050
    volatile uint32_t                          Cfg_Clk_Voc;                  //0x00000054
    /// This register is protected.
    volatile uint32_t                          Cfg_Clk_Out;                  //0x00000058
    volatile uint32_t                          Cfg_Clk_BB;                   //0x0000005C
    volatile uint32_t                          Cfg_Clk_AudioBCK_Div;         //0x00000060
    volatile uint32_t                          Cfg_Aif_Tx_Stb_Div;           //0x00000064
    volatile uint32_t                          Cfg_Clk_Uart[3];              //0x00000068
    volatile uint32_t                          Cfg_Clk_PWM;                  //0x00000074
    volatile uint32_t                          Cfg_Clk_Auxclk;               //0x00000078
    volatile uint32_t                          Cfg_Clk_Dbg_Div;              //0x0000007C
    volatile uint32_t                          Cfg_Clk_Camera_Out;           //0x00000080
    /// This register is protected.
    volatile uint32_t                          Cfg_AHB;                      //0x00000084
    /// This register is protected. Used to unsplit masters manualy.
    volatile uint32_t                          Ctrl_AHB;                     //0x00000088
    volatile uint32_t                          XCpu_Dbg_BKP;                 //0x0000008C
    volatile uint32_t                          XCpu_Dbg_Addr;                //0x00000090
    volatile uint32_t                          BCpu_Dbg_BKP;                 //0x00000094
    volatile uint32_t                          BCpu_Dbg_Addr;                //0x00000098
    volatile uint32_t                          Cfg_Cpus_Cache_Ram_Disable;   //0x0000009C
    volatile uint32_t                          Reset_Cause;                  //0x000000A0
    /// This register is protected.
    volatile uint32_t                          WakeUp;                       //0x000000A4
    /// This register is protected.
    volatile uint32_t                          Ignore_Charger;               //0x000000A8
    volatile uint32_t                          Cfg_Clk_SpiFlash;             //0x000000AC
    volatile uint32_t                          Cfg_Clk_Mpmc;                 //0x000000B0
    volatile uint32_t                          Clk_Sys_Enable2;              //0x000000B4
    volatile uint32_t                          Clk_Sys_Disable2;             //0x000000B8
    volatile uint32_t                          reserved[16];
    volatile uint32_t                          Cfg_Reserved;                 //0x000000FC
} HWP_SYS_CTRL_T;

#define SYS_CTRL_BASE_ADDR 0xA1A00000
#define hw_sys_ctrl ((HWP_SYS_CTRL_T*) SYS_CTRL_BASE_ADDR)

#endif // _SYS_CTRL_H_
