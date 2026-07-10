// Hardware Registers for SYS_IRQ at Base 0xA1A01000
#ifndef _SYS_IRQ_H_
#define _SYS_IRQ_H_

#include <stdint.h>

typedef struct
{
    /// If cause is not null and interrupt are enabled then the interrupt line 0
    /// is driven on the system CPU.
    /// The cause for the Irq sources, one bit for each module's irq source.
    /// The cause is the actual Irq source masked by the mask register.
    volatile uint32_t                          Cause;                        //0x00000000
    /// The status for the level Irq sources, one bit for each module's irq source.
    ///
    /// The status reflect the actual Irq source.
    volatile uint32_t                          Status;                       //0x00000004
    /// Writing '1' sets the corresponding bit in the mask register to '1'.
    /// Reading gives the value of the mask register.
    volatile uint32_t                          Mask_Set;                     //0x00000008
    /// Writing '1' clears the corresponding bit in the mask register to '0'.
    /// Reading gives the value of the mask register.
    volatile uint32_t                          Mask_Clear;                   //0x0000000C
    volatile uint32_t                          NonMaskable;                  //0x00000010
    volatile uint32_t                          SC;                           //0x00000014
    /// Each bit to '1' in that registers allows the correcponding interrupt to wake
    /// up the System CPU (i.e.: Reenable it's clock, see CLOCK_SYS_ENABLE and CLOCK_SYS_DISABLE
    /// registers in sys_ctrl registers section)
    volatile uint32_t                          WakeUp_Mask;                  //0x00000018
    volatile uint32_t                          Cpu_Sleep;                    //0x0000001C
    /// Writing '1' sets the corresponding bit in the mask register to '1'.
    /// Reading gives the value of the mask register.
    volatile uint32_t                          Pulse_Mask_Set;               //0x00000020
    /// Writing '1' clears the corresponding bit in the mask register to '0'.
    /// Reading gives the value of the mask register.
    volatile uint32_t                          Pulse_Mask_Clr;               //0x00000024
    /// Writing '1' clears the corresponding Pulse IRQ.
    /// Pulse IRQ are set by the modules and cleared here.
    volatile uint32_t                          Pulse_Clear;                  //0x00000028
    /// The status for the Pulse Irq sources, one bit for each module's irq source.
    ///
    /// The status reflect the actual Irq source.
    volatile uint32_t                          Pulse_Status;                 //0x0000002C
} HWP_SYS_IRQ_T;

#define SYS_IRQ_BASE_ADDR 0xA1A01000
#define hw_sys_irq ((HWP_SYS_IRQ_T*) SYS_IRQ_BASE_ADDR)

#endif // _SYS_IRQ_H_
