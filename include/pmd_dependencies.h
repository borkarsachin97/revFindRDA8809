#ifndef PMD_DEPENDENCIES_H
#define PMD_DEPENDENCIES_H

#include "../src/hw_structs/sys_ctrl.h"
#include "../src/hw_structs/pwm.h"
#include "../src/hw_structs/sys_irq.h"
#include "../src/hw_structs/gpio.h"

// -----------------------------------------------------------------------------
// Core primitive typedefs for nostdlib compliance
// -----------------------------------------------------------------------------
typedef unsigned char       UINT8;
typedef unsigned short      UINT16;
typedef unsigned int        UINT32;
typedef signed char         INT8;
typedef void                VOID;
typedef int                 bool;
typedef int                 BOOL;

#ifndef true
#define true                1
#endif

#ifndef false
#define false               0
#endif

#ifndef TRUE
#define TRUE                1
#endif

#ifndef FALSE
#define FALSE               0
#endif

#ifndef NULL
#define NULL                ((void*)0)
#endif

// -----------------------------------------------------------------------------
// Vendor Enums extracted from platform/edrv/pmd/include/pmd_m.h
// -----------------------------------------------------------------------------
typedef enum
{
    PMD_LOWPOWER,
    PMD_WAKEUPTRANSPOWER,
    PMD_IDLEPOWER,
    PMD_SLEEPTRANSPOWER,
} PMD_POWERMODE_T;

typedef enum
{
    PMD_POWER_MIC,
    PMD_POWER_CAMERA,
    PMD_POWER_AUDIO,
    PMD_POWER_STEREO_DAC,
    PMD_POWER_LOUD_SPEAKER,
    PMD_POWER_PA,
    PMD_POWER_USB,
    PMD_POWER_SDMMC,
    PMD_POWER_FM,
    PMD_POWER_EARPIECE,
    PMD_POWER_BT,
    PMD_POWER_CAMERA_FLASH,
    PMD_POWER_LCD,
    PMD_POWER_ID_QTY
} PMD_POWER_ID_T;

typedef enum
{
    PMD_LEVEL_SIM,
    PMD_LEVEL_KEYPAD,
    PMD_LEVEL_LCD,
    PMD_LEVEL_SUB_LCD,
    PMD_LEVEL_LED0,
    PMD_LEVEL_LED1,
    PMD_LEVEL_LED2,
    PMD_LEVEL_LED3,
    PMD_LEVEL_VIBRATOR,
    PMD_LEVEL_LOUD_SPEAKER,
    PMD_LEVEL_ID_QTY
} PMD_LEVEL_ID_T;

#endif // PMD_DEPENDENCIES_H
// -----------------------------------------------------------------------------
// Stubbed Subsystems (Audio, FM, SIM, Earpiece, etc.)
// -----------------------------------------------------------------------------
static inline void hal_AbbEnableUsbPhy(BOOL on) {}
static inline void pmd_EnableFmInLpMode(BOOL on) {}
static inline void hal_ConfigFmIo(BOOL on) {}
static inline void hal_ConfigBtIo(BOOL on) {}
static inline void aud_EnableEarpieceSetting(BOOL on) {}
static inline void aud_InitEarpieceSetting(void) {}
static inline void pmd_EnableClassKLoudSpeaker(void) {}
static inline void pmd_SetHighActivity(BOOL on) {}
static inline void hal_TcuClrTco(UINT32 id) {}
static inline void hal_TcuSetTco(UINT32 id) {}
static inline void clk_gate_config(unsigned int data, bool enable) {}
static inline void pmd_InitCoreVoltage(void) {}
static inline void pmd_SetMemVoltage(UINT32 v) {}
static inline void pmd_SetMmcVoltage(UINT32 v) {}
static inline void pmd_RegIrqSettingSet(UINT32 v) {}
static inline void pmd_RegHpDetectSettingSet(UINT32 v) {}
static inline void pmd_InitTouchScreen(void) {}
static inline void pmd_InitSpeakerPa(void) {}
static inline void pmd_InitCharger(void) {}
static inline void hal_PwmResourceMgmt(void) {}
static inline void pmd_SimSetVoltageClass(UINT32 level) {}
static inline void pmd_ChargerSetBgLpMode(UINT32 user, bool enable) {}
static inline void hal_AnaGpadcEnable(void) {}
