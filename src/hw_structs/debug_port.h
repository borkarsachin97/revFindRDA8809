// Hardware Registers for DEBUG_PORT at Base 0xA1A0D000
#ifndef _DEBUG_PORT_H_
#define _DEBUG_PORT_H_

#include <stdint.h>
#include "../../helpfull-headers/globals.h"

typedef struct
{
    /// All this register is locked by the host_lock_reg signal
    volatile uint32_t                          Debug_Port_Mode;              //0x00000000
    /// All this register is locked by the host_lock_reg signal
    volatile uint32_t                          Pxts_Exl_Cfg;                 //0x00000004
    volatile uint32_t                          WD_Addr_cfg;                  //0x00000008
    /// <table border="1" > <!-- border should be in css--> <tr> <th>Pin</th> <th>MODE_SYS</th>
    /// <th>MODE_IPSPI</th> <th>MODE_IPSIG</th> <th>MODE_RF</th> </tr> <tr> <td>DBG_BUS_STB</td>
    /// <td colspan="4" >FInt_H</td> </tr> <tr> <td>DBG_BUS_0</td> <td colspan="3"
    /// >GPO_8</td> <td>RxTxEN</td> </tr> <tr> <td>DBG_BUS_1</td> <td>?</td> <td>PMU_SPI3_CS</td>
    /// <td>WAKEUP</td> <td>RxTxDATA</td> </tr> <tr> <td>DBG_BUS_2</td> <td>Xcpu_Main_irq_L</td>
    /// <td>ABB_SPI3_CS</td> <td>CHG MASK</td> <td>STROBE</td> </tr> <tr> <td>DBG_BUS_3</td>
    /// <td>Bcpu_Main_irq_L</td> <td>FM_SPI3_CS</td> <td>LP_MODE</td> <td>RFSPI_CLK</td>
    /// </tr> <tr> <td>DBG_BUS_4</td> <td>Sys_IntEnabled_H</td> <td>SPI3_CLK</td>
    /// <td>PLL_PD</td> <td>RFSPI_CS</td> </tr> <tr> <td>DBG_BUS_5</td> <td>BB_IntEnabled_H</td>
    /// <td>SPI3_DO</td> <td>RF_SYSCLKEN</td> <td>RFSPI_DIO</td> </tr> <tr> <td>DBG_BUS_6</td>
    /// <td colspan="3" >Signal_Select0</td> <td>RF_PDN</td> </tr> <tr> <td>DBG_BUS_7</td>
    /// <td colspan="3" >Signal_Select1</td> <td>RX_DEC_ON</td> </tr> </table>
    volatile uint32_t                          Signal_Spy_Cfg;               //0x0000000C
    volatile uint32_t                          Dbg_Clk_Cfg;                  //0x00000010
    volatile uint32_t                          Dbg_Watch_Set;                //0x00000014
    volatile uint32_t                          Dbg_Watch_Clr;                //0x00000018
    volatile uint32_t Reserved_0000001C[9];                 //0x0000001C
    volatile uint32_t                          Pxts_tag[16];                 //0x00000040
} HWP_DEBUG_PORT_T;

#define DEBUG_PORT_BASE_ADDR 0xA1A0D000
#define hw_debug_port ((HWP_DEBUG_PORT_T*) DEBUG_PORT_BASE_ADDR)

#endif // _DEBUG_PORT_H_
