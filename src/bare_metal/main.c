
// ====================================================================
// 2. DATA STRUCTURE DEFINITIONS
// ====================================================================

struct list_head {
    struct list_head *next, *prev;
};

static inline void list_add_tail(struct list_head *new, struct list_head *head) {
    new->next = head;
    new->prev = head->prev;
    head->prev->next = new;
    head->prev = new;
}

struct clk {
    unsigned long req_par_rate;
    int (*set_rate)(struct clk *c, unsigned long r);
    unsigned char usecount;
    struct clk *parent;
    unsigned long flags;
    unsigned int gate_data;
    unsigned long rate;
    unsigned long (*recalc)(struct clk *c);
};

struct sw_clk_consumer {
    unsigned long four_cc;
    unsigned long req_rate;
    struct list_head node;
};

#define MAX_SW_CLK_USER_NUM 8


#include "../../include/pmd_dependencies.h"
extern struct sw_clk_consumer g_consumers[MAX_SW_CLK_USER_NUM];
extern struct list_head g_sw_consumers;
extern struct clk fake_clk_sw;
extern struct clk *g_clk_sw;



// ====================================================================
// 1. STANDARD TYPE DEFINITIONS & HARDWARE CONSTANTS


// ====================================================================
#define PUBLIC
#define PROTECTED
#define INLINE              inline
#define PUBLIC_VOID         void
#define PMD_FUNC_INTERNAL   __attribute__((section(".sramtext")))
#define HAL_BOOT_FUNC_INTERNAL __attribute__((section(".boottext")))
#define HAL_BOOT_FUNC       __attribute__((section(".boottext")))
#define HAL_FUNC_INTERNAL   __attribute__((section(".internaltext")))

// Global Assertion and Trace Stubs
#define PMD_ASSERT(cond, ...)  do { if(!(cond)) { while(1); } } while(0)
#define HAL_ASSERT(cond, ...)  do { if(!(cond)) { while(1); } } while(0)
#define PMD_TRACE(...)         do {} while(0)
#define TSTDOUT                0
#define PMD_DBG_TRC            0
#define PMD_INFO_TRC           0
#define POSSIBLY_UNUSED        __attribute__((unused))

// Hardware Identity and Bitwise Flags
#define HAL_GPIO_NONE       0
#define HAL_GPO_NONE        0
#define HAL_GPIO_2          2
#define HAL_ANA_GPADC_CHAN_1 1
#define HAL_ANA_GPADC_CHAN_6 6
#define HAL_ANA_GPADC_CHAN_7 7

#define PMD_LDO_NONE        0
#define PMD_LDO_MIC         (1 << 0)
#define PMD_LDO_CAM         (1 << 1)
#define PMD_LDO_RF          (1 << 2)
#define PMD_LDO_ABB         (1 << 3)
#define PMD_LDO_USB         (1 << 4)
#define PMD_LDO_MMC         (1 << 5)
#define PMD_LDO_LCD         (1 << 6)
#define PMD_LDO_VIBR        (1 << 7)
#define PMD_LDO_BLLED       (1 << 8)

#define PMD_LEVEL_TYPE_NONE      0
#define PMD_LEVEL_TYPE_OPAL      1
#define PMD_LEVEL_TYPE_LDO       2
#define PMD_LEVEL_TYPE_BACKLIGHT 3
#define PMD_LEVEL_TYPE_PWL0      4
#define PMD_LEVEL_TYPE_PWL1      5
#define PMD_LEVEL_TYPE_LPG       6

#define PMD_LDO_ID_TYPE_IO       1
#define PMD_LDO_ID_TYPE_OPAL     2
#define PMD_LDO_ID_TYPE_INVALID  0

#define PMD_CHARGER_400MA   400
#define HOURS               * 3600

// PMD Internal Register Allocations
#define RDA_ADDR_MISC_CONTROL           0x18
#define RDA_ADDR_CALIBRATION_SETTING2   0x17
#define RDA_ADDR_LDO_BUCK1              0x2D
#define RDA_ADDR_LED_SETTING2           0x1A
#define RDA_ADDR_LED_SETTING4           0x38
#define RDA_ADDR_LED_SETTING5           0x3E
#define RDA_ADDR_THERMAL_CALIBRATION    0x36
#define RDA_ADDR_LDO_SETTINGS           0x10
#define RDA_ADDR_LDO_ACTIVE_SETTING1    0x11
#define RDA_ADDR_LDO_ACTIVE_SETTING2    0x12
#define RDA_ADDR_LDO_ACTIVE_SETTING3    0x13
#define RDA_ADDR_LDO_LP_SETTING3        0x14
#define RDA_PMU_PD_CHARGE_LDO           (1 << 7)
#define RDA_PMU_BL_OFF_ACT              (1 << 0)

// Masking Macros for Power Execution Loops
#define RDA_PMU_LED_R_IBIT_MASK         (0x7 << 0)
#define RDA_PMU_LED_G_IBIT_MASK         (0x7 << 3)
#define RDA_PMU_LED_B_IBIT_MASK         (0x7 << 6)
#define RDA_PMU_LED_R_IBIT(v)           ((v & 0x7) << 0)
#define RDA_PMU_LED_G_IBIT(v)           ((v & 0x7) << 3)
#define RDA_PMU_LED_B_IBIT(v)           ((v & 0x7) << 6)
#define RDA_PMU_LED_R_OFF_ACT           (1 << 9)
#define RDA_PMU_LED_R_OFF_LP            (1 << 10)
#define RDA_PMU_LED_G_OFF_ACT           (1 << 11)
#define RDA_PMU_LED_G_OFF_LP            (1 << 12)
#define RDA_PMU_LED_B_OFF_ACT           (1 << 13)
#define RDA_PMU_LED_B_OFF_LP            (1 << 14)
#define RDA_PMU_BL_IBIT_ACT             (0xF << 4)
#define RDA_PMU_LDO_EN_MASK             0xFFFF
#define RDA_PMU_VPAD_IBIT               (0xF << 0)
#define RDA_PMU_PU_CLK_4M_DR            (1 << 0)
#define RDA_PMU_PD_LDO_AVDD3_DR         (1 << 1)
#define RDA_PMU_PD_LDO_AVDD3_REG        (1 << 2)
#define RDA_PMU_PU_CLK_4M_REG           (1 << 3)

#define PMU_SET_BITFIELD(reg, mask, val) (((reg) & ~(mask)) | (((val) << 4) & (mask)))
#define GET_BITFIELD(reg, mask)          (((reg) & (mask)) >> 0)

// System Abstraction Struct Hooks & Call Registers
typedef int PMD_BG_LP_MODE_USER_ID_T;
typedef int PMD_CLK4M_AVDD3_USER_ID_T;
#define PMD_BG_LP_MODE_USER_LED0        0
#define PMD_BG_LP_MODE_USER_LED1        1
#define PMD_BG_LP_MODE_USER_LED2        2
#define PMD_BG_LP_MODE_USER_LED3        3
#define PMD_BG_LP_MODE_USER_KEYPAD_LED  4
#define PMD_CLK4M_AVDD3_USER_LED0       0
#define PMD_CLK4M_AVDD3_USER_LED1       1
#define PMD_CLK4M_AVDD3_USER_LED2       2
#define PMD_CLK4M_AVDD3_USER_LED3       3
#define PMD_CLK4M_AVDD3_USER_KEYPAD_LED 4

#define LEVEL_LDO_USER(id)              (1 << (id))
#define POWER_LDO_USER(id)              (1 << (id))
#define HAL_PWL_0                       0
#define HAL_PWL_1                       1

// Macro stubs for compatibility
#define SWAP_8808S_GPIO_6_AND_13(gpio)  do {} while(0)
#define SWAP_FP_GPIO_24_AND_25(gpio)    do {} while(0)
#define SXS_GET_RA(addr)                do { addr = 0; } while(0)

// FourCC String Compression Macro Engine
#define FOURCC(a,b,c,d) ((UINT32)(a) | ((UINT32)(b) << 8) | ((UINT32)(c) << 16) | ((UINT32)(d) << 24))

// Clock Frequency Target Layout Enumerations
enum
{
    HAL_CLK_RATE_32K = 32768,
    HAL_CLK_RATE_13M = 13000000,
    HAL_CLK_RATE_26M = 26000000,
    HAL_CLK_RATE_39M = 39000000,
    HAL_CLK_RATE_48M = 48000000,
    HAL_CLK_RATE_52M = 52000000,
    HAL_CLK_RATE_78M = 78000000,
    HAL_CLK_RATE_89M = 89142857,
    HAL_CLK_RATE_104M = 104000000,
    HAL_CLK_RATE_113M = 113454545,
    HAL_CLK_RATE_125M = 124800000,
    HAL_CLK_RATE_139M = 138666667,
    HAL_CLK_RATE_156M = 156000000,
    HAL_CLK_RATE_178M = 178285714,
    HAL_CLK_RATE_208M = 208000000,
    HAL_CLK_RATE_250M = 249600000,
    HAL_CLK_RATE_312M = 312000000,
    HAL_CLK_RATE_UNKNOWN = 0,
};

enum
{
    FOURCC_UNKNOWN = 0,
    FOURCC_PAL  = FOURCC('P', 'A', 'L', ' '),
    FOURCC_TEST = FOURCC('T', 'E', 'S', 'T'),
    FOURCC_TOOL = FOURCC('T', 'O', 'O', 'L'),
    FOURCC_PWM  = FOURCC('P', 'W', 'M', ' '),
    FOURCC_ISPI = FOURCC('I', 'S', 'P', 'I'),
};

typedef enum {
    HAL_SYS_CHIP_PROD_ID,
    HAL_SYS_CHIP_BOND_ID,
    HAL_SYS_CHIP_METAL_ID
} HAL_SYS_CHIP_ID_T;

typedef enum {
    HAL_SYS_RESET_CAUSE_NORMAL,
    HAL_SYS_RESET_CAUSE_WATCHDOG,
    HAL_SYS_RESET_CAUSE_HOST_DEBUG,
    HAL_SYS_RESET_CAUSE_ALARM,
    HAL_SYS_RESET_CAUSE_RESTART
} HAL_SYS_RESET_CAUSE_T;

typedef enum {
    HAL_ISPI_CS_PMU
} HAL_ISPI_CS_T;

typedef int HAL_GPIO_GPIO_ID_T;
typedef int HAL_ANA_GPADC_CHAN_T;
typedef int HAL_ANA_GPADC_ATP_T;
typedef int HAL_PWL_ID_T;
typedef int RDA_REG_MAP_T;

#define HAL_SYS_CHIP_ID_QTY 3
#define HAL_ANA_GPADC_CHAN_QTY 8
#define HAL_PWL_NONE 0xFF
#define PMD_PROFILE_MODE_QTY 2
#define PMD_LDO_QTY 9
#define PMD_CLK4M_AVDD3_USER_ID_QTY 5

#define CFG_REGS_PROD_ID  0xFFFF0000
#define CFG_REGS_BOND_ID  0x0000FFF0
#define CFG_REGS_METAL_ID 0x0000000F

#define SYS_SHUTDOWN_MAGIC_NUM            0xDEADBEEF
#define SYS_CTRL_WATCHDOG_RESET_HAPPENED  (1 << 0)
#define SYS_CTRL_HOSTDEBUG_RESET_HAPPENED (1 << 1)
#define SYS_CTRL_ALARMCAUSE_HAPPENED      (1 << 2)
#define SYS_CTRL_GLOBALSOFT_RESET_HAPPENED (1 << 3)

#define SPI_CS(id)           ((id) << 28)
#define SPI_TX_SPACE         (0xF << 4)
#define SPI_ACTIVE_STATUS    (1 << 0)
#define SPI_TX_FIFO_SIZE     16

#define PWM_PWL1_SET_OE      (1 << 0)
#define PWM_PWL1_FORCE_L     (1 << 1)
#define PWM_PWL1_FORCE_H     (1 << 2)
#define PWM_PWL1_EN_H        (1 << 3)
#define PWM_PWL1_THRESHOLD(l) ((l) << 8)
#define PWM_PWL0_SET_OE      (1 << 0)
#define PWM_PWL0_FORCE_L     (1 << 1)
#define PWM_PWL0_FORCE_H     (1 << 2)
#define PWM_PWL0_EN_H        (1 << 3)
#define PWM_PWL0_THRESHOLD(l) ((l) << 8)
#define PWM_PWL_MIN(l)       ((l) << 8)
#define GATE_MANUAL          (1 << 0)

#define HAL_GPIO_TYPE_NONE   0
#define HAL_GPIO_TYPE_IO     1
#define HAL_GPIO_TYPE_O      2
#define HAL_GPIO_TYPE_TCO    3

typedef struct {
    UINT32 gpioId;
    UINT32 type;
    UINT32 id;
    UINT32 gpoId;
} HAL_APO_ID_T;

typedef HAL_APO_ID_T HAL_GPIO_GPIO_ID_STRUCT_T;



// ====================================================================
// 2. DATA STRUCTURE DEFINITIONS


// ====================================================================


    #define LIST_HEAD_INIT(name) { &(name), &(name) }



typedef struct {
    UINT32 opal;
    HAL_APO_ID_T pin;
    UINT32 type;
    UINT32 id;
} PMD_LDO_ID_T;

typedef struct {
    PMD_LDO_ID_T ldo;
    UINT32       polarity;
    UINT32       shared;
    UINT32       powerOnState;
} PMD_POWER_CONFIG_T;

typedef struct {
    UINT32       type;
    PMD_LDO_ID_T ldo;
    UINT32       powerOnState;
} PMD_LEVEL_CONFIG_T;

typedef struct {
    PMD_POWER_CONFIG_T power[14];
    PMD_LEVEL_CONFIG_T level[10];
    UINT32             lightLevelToBacklight[8];
    UINT32             lightLevelToPwm0[8];
    UINT32             lightLevelToPwm1[8];
    UINT32             ldoEnableNormal;
    UINT32             ldoEnableLowPower;
    UINT32             ldoCamIs2_8;
    UINT32             ldoLcdIs2_8;
    UINT32             ldoMMCIs2_8;
    UINT32             ldoIbrIs2_8;
    UINT32             ldoRfIs2_8;
    UINT32             batteryGpadcChannel;
    UINT32             batteryLevelChargeTermMV;
    UINT32             batteryLevelRechargeMV;
    UINT32             batteryLevelFullMV;
    UINT32             batteryChargeTimeout;
    UINT32             batteryOffsetHighActivityMV;
    UINT32             batteryOffsetPowerDownChargeMV;
    UINT32             powerOnVoltageMV;
    UINT32             powerDownVoltageMV;
    UINT32             batteryChargeCurrent;
    UINT32             chargerGpadcChannel;
    UINT32             chargerLevelUpperLimit;
    UINT32             chargerOffsetBackToNormal;
    UINT32             batteryMVScreenAntiFlicker;
    UINT32             batteryOffsetScreenNormal;
    UINT32             earpieceDetectGpio;
    UINT32             earpieceGpadcChannel;
} PMD_CONFIG_T;

typedef struct {
    UINT32 levelInfo[10];
    UINT32 powerInfo[14];
} PMD_MAP_ACCESS_T;

typedef enum {
    PMD_PROFILE_MODE_NORMAL,
    PMD_PROFILE_MODE_LOWPOWER
} PMD_PROFILE_MODE_T;

// Hardware register access overlays
typedef struct { volatile UINT32 CHIP_ID; } CONFIG_REGS_T;
typedef struct { volatile UINT32 SC; } SYS_IRQ_T;
typedef struct { volatile UINT32 status; volatile UINT32 rxtx_buffer; } ISPI_T;
typedef struct { volatile UINT32 gpio_oen_set_out; volatile UINT32 gpio_clr; volatile UINT32 gpo_clr; volatile UINT32 gpio_set; volatile UINT32 gpo_set; } GPIO_REGS_T;
typedef struct { volatile UINT32 PWL1_Config; volatile UINT32 PWL0_Config; } PWM_REGS_T;

#define hwp_configRegs ((CONFIG_REGS_T*)0xA1A00000)
#define hwp_sysIrq     ((SYS_IRQ_T*)0xA1A01000)
#define hwp_ispi       ((ISPI_T*)0xA1A02000)
#define hwp_gpio       ((GPIO_REGS_T*)0xA1A03000)
#define hwp_pwm        ((PWM_REGS_T*)0xA1A04000)

typedef struct { UINT32 reg; UINT32 fmEnable; UINT32 btEnable; } PMD_LDO_SETTINGS_T;
typedef struct { UINT32 reg; } PMD_LDO_PROFILE_SETTING1_T;
typedef struct { UINT32 reg; BOOL rgbLedOff; BOOL vPadIs1_8; BOOL vIbrIs1_8; BOOL vMmcIs1_8; BOOL vLcdIs1_8; BOOL vCamIs1_8; BOOL vRfIs1_8; } PMD_LDO_PROFILE_SETTING2_T;
typedef struct { UINT32 pwmCfg; } HAL_CFG_T;

// Corrected macro traversal path targeting struct link offsets smoothly
#define list_for_each_entry(pos, head, member) \
    for (pos = (void*)(head)->next; (struct list_head*)(pos) != (head); pos = (void*)(pos)->member.next)



// ====================================================================
// 3. TARGET SYSTEM REGISTER LOOKUP MAP STRUCTURES


// ====================================================================
#define HAL_MAP_ID_COUNT           0x0000001A
#define HAL_MAP_TABLE_LOC   __attribute__((section(".sramucbss")))

typedef struct {
    UINT32 revision;
    UINT32 number;
    UINT32 date;
    INT8* string;
} HAL_MAP_VERSION_T;

typedef struct {
    HAL_MAP_VERSION_T* version;
    VOID* access;
} HAL_MAP_MODULE_T;

typedef struct {
    HAL_MAP_MODULE_T modules[HAL_MAP_ID_COUNT];
} HAL_MAP_TABLE_T;

#define UNCACHED(n) (UINT32*)(((UINT32)n)|0xa0000000)



// ====================================================================
// 4. THE COMPLETED MULTILINE TARGET PMU PROFILE DATA MACRO


// ====================================================================
#define TGT_PMD_CONFIG                                                  \
    {                                                                   \
        .power =                                                        \
        {                                                               \
            { .ldo = { .opal = PMD_LDO_MIC }, .polarity = TRUE, .shared = TRUE, .powerOnState = FALSE },         \
            { .ldo = { .opal = PMD_LDO_CAM|PMD_LDO_RF }, .polarity = TRUE, .shared = FALSE, .powerOnState = FALSE }, \
            { .ldo = { .opal = PMD_LDO_ABB }, .polarity = TRUE, .shared = FALSE, .powerOnState = FALSE },        \
            { .ldo = { .opal = PMD_LDO_NONE }, .polarity = TRUE, .shared = FALSE, .powerOnState = FALSE },       \
            { .ldo = { .pin = {.gpioId = HAL_GPIO_NONE}}, .polarity = TRUE, .shared = FALSE, .powerOnState = FALSE }, \
            { .ldo = { .opal = PMD_LDO_NONE }, .polarity = TRUE, .shared = FALSE, .powerOnState = FALSE },       \
            { .ldo = { .opal = PMD_LDO_USB }, .polarity = TRUE, .shared = FALSE, .powerOnState = FALSE },        \
            { .ldo = { .opal = PMD_LDO_MMC }, .polarity = TRUE, .shared = FALSE, .powerOnState = FALSE },        \
            { .ldo = { .opal = PMD_LDO_ABB }, .polarity = TRUE, .shared = FALSE, .powerOnState = FALSE },        \
            { .ldo = { .opal = PMD_LDO_MIC }, .polarity = TRUE, .shared = TRUE, .powerOnState = FALSE },         \
            { .ldo = { .opal = PMD_LDO_NONE }, .polarity = TRUE, .shared = FALSE, .powerOnState = FALSE },       \
            { .ldo = { .pin = {.gpioId = HAL_GPIO_NONE}}, .polarity = TRUE, .shared = FALSE, .powerOnState = FALSE }, \
            { .ldo = { .opal = PMD_LDO_LCD }, .polarity = TRUE, .shared = FALSE, .powerOnState = FALSE },        \
            { .ldo = { .opal = PMD_LDO_LCD|PMD_LDO_CAM}, .polarity = TRUE, .shared = FALSE, .powerOnState = FALSE }  \
        },                                                              \
        .level =                                                        \
        {                                                               \
            { .type = PMD_LEVEL_TYPE_NONE, .ldo = { .opal = PMD_LDO_NONE}, .powerOnState = 0 },                  \
            { .type = PMD_LEVEL_TYPE_OPAL, .ldo = { .pin = { .gpoId = HAL_GPO_NONE}}, .powerOnState = 0 },       \
            { .type = PMD_LEVEL_TYPE_BACKLIGHT, .ldo = { .opal = PMD_LDO_NONE}, .powerOnState = 0 },             \
            { .type = PMD_LEVEL_TYPE_NONE, .ldo = { .opal = PMD_LDO_NONE}, .powerOnState = 0 },                  \
            { .type = PMD_LEVEL_TYPE_NONE, .ldo = { .opal = PMD_LDO_NONE}, .powerOnState = 0 },                  \
            { .type = PMD_LEVEL_TYPE_NONE, .ldo = { .opal = PMD_LDO_NONE}, .powerOnState = 0 },                  \
            { .type = PMD_LEVEL_TYPE_NONE, .ldo = { .opal = PMD_LDO_NONE}, .powerOnState = 0 },                  \
            { .type = PMD_LEVEL_TYPE_OPAL, .ldo = { .pin = { .gpioId = HAL_GPIO_NONE}}, .powerOnState = 0 },     \
            { .type = PMD_LEVEL_TYPE_LDO, .ldo = { .opal = PMD_LDO_VIBR}, .powerOnState = 0 },                   \
            { .type = PMD_LEVEL_TYPE_NONE, .ldo = { .opal = PMD_LDO_NONE}, .powerOnState = 0 }                   \
        },                                                              \
        .lightLevelToBacklight = { 0, 16, 48, 64, 64, 64, 64, 64},      \
        .lightLevelToPwm0 = { 0,  8,  8,  8,  8,  8,  8,  8},           \
        .lightLevelToPwm1 = { 0,   0,   0,   0,   0, 224, 224, 224},    \
        .ldoEnableNormal = 0,                                           \
        .ldoEnableLowPower = 0,                                         \
        .ldoCamIs2_8 = TRUE,                                            \
        .ldoLcdIs2_8 = TRUE,                                            \
        .ldoMMCIs2_8 = TRUE,                                            \
        .ldoIbrIs2_8 = FALSE,                                           \
        .ldoRfIs2_8  = FALSE,                                           \
        .batteryGpadcChannel = HAL_ANA_GPADC_CHAN_7,                    \
        .batteryLevelChargeTermMV    = 4200,                            \
        .batteryLevelRechargeMV      = 4150,                            \
        .batteryLevelFullMV          = 4200,                            \
        .batteryChargeTimeout        = 4 HOURS,                         \
        .batteryOffsetHighActivityMV = 30,                              \
        .batteryOffsetPowerDownChargeMV = -150,                         \
        .powerOnVoltageMV            = 3400,                            \
        .powerDownVoltageMV          = 3400,                            \
        .batteryChargeCurrent        = PMD_CHARGER_400MA,               \
        .chargerGpadcChannel         = HAL_ANA_GPADC_CHAN_6,            \
        .chargerLevelUpperLimit      = 6500,                            \
        .chargerOffsetBackToNormal   = -300,                            \
        .batteryMVScreenAntiFlicker  = 3600,                            \
        .batteryOffsetScreenNormal   = 100,                             \
        .earpieceDetectGpio          = HAL_GPIO_2,                      \
        .earpieceGpadcChannel        = HAL_ANA_GPADC_CHAN_1,            \
    }



// ====================================================================
// 5. GLOBAL ARTIFACT DECLARATIONS & MANAGEMENT STRUCTURES


// ====================================================================
HAL_MAP_TABLE_LOC HAL_MAP_TABLE_T g_halMapTable;
HAL_MAP_TABLE_T* g_halFixedPointer __attribute__((section (".fixptr")));

const PMD_CONFIG_T g_tgtPmdConfig = TGT_PMD_CONFIG;
const PMD_CONFIG_T* g_pmdConfig = NULL;
UINT32 g_pmdInitDone = FALSE;

// State Profiles Variables Tracking Definitions
UINT8  gc_pmdPadLevelLp         = 1;
UINT8  gc_pmdPadLevelAct        = 4;
UINT16 g_pmdMiscControlLp       = 0x44;
UINT16 g_pmdMiscControlActive   = 0x04;
UINT16 gc_pmdLdoBuck1Lp         = 0x07;
UINT16 gc_pmdLdoBuck1Act        = 0x04;
UINT16 g_pmdCalibSetting2       = 0x00;
UINT16 g_pmdThermalCalib        = 0x00;
UINT16 g_pmdLedSetting2         = 0x00;
UINT16 g_pmdLedSetting4         = 0x00;
UINT16 g_pmdLedSetting5         = 0x00;
UINT32 g_pmdBacklightOn         = FALSE;

PMD_MAP_ACCESS_T g_pmdMapAccess;

// Internal Clock System Storage Blocks




const unsigned long gClkRateList[] = {
    32768, 13000000, 26000000, 39000000, 48000000, 52000000, 78000000,
    104000000, 124800000, 156000000, 208000000, 312000000
};

// System Variables
UINT32* g_bootSysCsAddress = (UINT32*)0xA1A00FFF;
UINT32 g_halSysStateShutdownFlag = 0;
BOOL g_halSysBootFromShutdown = FALSE;
UINT32 g_bootResetCause = 0;
PMD_LDO_SETTINGS_T g_pmdLdoSettings = {0, 0, 0};
PMD_LDO_PROFILE_SETTING1_T g_pmdLdoProfileSetting1[2] = {{0}, {0}};
PMD_LDO_PROFILE_SETTING2_T g_pmdLdoProfileSetting2[2] = {{0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}};
UINT32 g_pmdLdoProfileSetting3 = 0;
UINT32 g_pmdLdoUserBitmap[2][9] = {{0}, {0}};
BOOL g_pmdClk4mAvdd3EnRegistryLp[5] = {0};
void (*g_pmdAuxClkStatusHandler)(BOOL) = NULL;

typedef struct { UINT32 pwlGlowPosition; } PWM_CFG_STRUCT_T;
typedef struct { PWM_CFG_STRUCT_T pwmCfg; } CFG_STRUCT_T;
const CFG_STRUCT_T fake_halCfg = {{0}};
const CFG_STRUCT_T* g_halCfg = &fake_halCfg;

void* calib_GetPointers(void) { return NULL; }
void* g_halCalibration = NULL;
UINT32 g_halGpadcAtpConfig[8] = {0};

#define gc_errWrongGpioType 0
#define RDA_PMU_EOMIRQ_MASK 1
#define RDA_PMU_PENIRQ_MASK 2
#define RDA_PMU_KEYIRQ_MASK 4
#define RDA_PMU_HOLD_PRECHARGER_EFUSE 8
#define RDA_PMU_PD_MODE_SEL 16
#define RDA_PMU_HP_IN_MASK 1
#define RDA_PMU_HP_OUT_MASK 2

PUBLIC UINT32 HAL_BOOT_FUNC_INTERNAL boot_SysGetChipId(HAL_SYS_CHIP_ID_T part)
{
    UINT32 chipId = hwp_configRegs->CHIP_ID;

    if (part == HAL_SYS_CHIP_PROD_ID)
    {
        chipId = GET_BITFIELD(chipId, CFG_REGS_PROD_ID);
    }
    else if (part == HAL_SYS_CHIP_BOND_ID)
    {
        chipId = GET_BITFIELD(chipId, CFG_REGS_BOND_ID);
    }
    else if (part == HAL_SYS_CHIP_METAL_ID)
    {
        chipId = GET_BITFIELD(chipId, CFG_REGS_METAL_ID);
    }

    return chipId;
}

PUBLIC UINT32 hal_SysGetChipId(HAL_SYS_CHIP_ID_T part)
{
    return boot_SysGetChipId(part);
}

PUBLIC HAL_SYS_RESET_CAUSE_T hal_SysGetResetCause(VOID)
{
    if (g_halSysStateShutdownFlag == SYS_SHUTDOWN_MAGIC_NUM)
    {
        g_halSysBootFromShutdown = TRUE;
    }
    g_halSysStateShutdownFlag = 0;

    if (g_bootResetCause & SYS_CTRL_WATCHDOG_RESET_HAPPENED)
    {
        return HAL_SYS_RESET_CAUSE_WATCHDOG;
    }

    if (g_bootResetCause & SYS_CTRL_HOSTDEBUG_RESET_HAPPENED)
    {
        return HAL_SYS_RESET_CAUSE_HOST_DEBUG;
    }

    if (g_bootResetCause & SYS_CTRL_ALARMCAUSE_HAPPENED)
    {
        return HAL_SYS_RESET_CAUSE_ALARM;
    }

    if (g_bootResetCause & SYS_CTRL_GLOBALSOFT_RESET_HAPPENED)
    {
        if (g_halSysBootFromShutdown)
        {
            return HAL_SYS_RESET_CAUSE_NORMAL;
        }
        else
        {
            return HAL_SYS_RESET_CAUSE_RESTART;
        }
    }

    return HAL_SYS_RESET_CAUSE_NORMAL;
}

PUBLIC VOID HAL_BOOT_FUNC_INTERNAL hal_IspiRegWrite(HAL_ISPI_CS_T csId, UINT16 address, UINT16 data)
{
    UINT32 sc = hwp_sysIrq->SC;
    UINT32 cmd = SPI_CS(csId) | (0<<25) | ((address & 0x1ff) << 16) | data;

    while (GET_BITFIELD(hwp_ispi->status, SPI_TX_SPACE) == 0)
        ;

    hwp_ispi->rxtx_buffer = cmd;

    for (;;)
    {
        UINT32 status = hwp_ispi->status;
        if ((status & SPI_ACTIVE_STATUS) == 0 &&
                GET_BITFIELD(status, SPI_TX_SPACE) == SPI_TX_FIFO_SIZE)
            break;
    }

    hwp_sysIrq->SC = sc;
}

PUBLIC VOID HAL_BOOT_FUNC pmd_OpalSpiWrite(RDA_REG_MAP_T regIdx, UINT16 value)
{
    hal_IspiRegWrite(HAL_ISPI_CS_PMU, regIdx, value);
}

// Forward declarations
PUBLIC VOID pmd_RDAWrite(RDA_REG_MAP_T regIdx, UINT16 value);
INLINE UINT32 hal_SysEnterCriticalSection(VOID);
INLINE VOID hal_SysExitCriticalSection(UINT32 status);



// ====================================================================
// 7. POWER MANAGEMENT ENGINE MODULE


// ====================================================================
PROTECTED VOID pmd_EnableOpalLdo(PMD_LDO_ID_T ldo, BOOL on,
                                 PMD_PROFILE_MODE_T profile, UINT32 user)
{
    PMD_ASSERT(profile < PMD_PROFILE_MODE_QTY, "pmd_EnableOpalLdo: Invalid profile number (%d)", profile);
    PMD_ASSERT(ldo.type == PMD_LDO_ID_TYPE_OPAL, "pmd_EnableOpalLdo: using ID of non LDO resource (%08x) ", ldo.type);
    PMD_ASSERT(ldo.id < (1<<PMD_LDO_QTY), "pmd_EnableOpalLdo: invalid LDO Id 0x%08x", ldo.id);

    if (ldo.id & PMD_LDO_USB)
    {
        ldo.id &= ~PMD_LDO_USB;
    }
    if (ldo.id & PMD_LDO_MIC)
    {
        ldo.opal |= PMD_LDO_ABB;
    }

    if (ldo.id == 0)
    {
        return;
    }

    UINT32 reg_offset = 0;
    UINT32 idx = 0;
    UINT32 ldoId = ldo.id;

    UINT32 scStatus = hal_SysEnterCriticalSection();

    while (ldoId)
    {
        if (ldoId & 0x1)
        {
            if (on)
            {
                if (g_pmdLdoUserBitmap[profile][idx] != 0)
                {
                    ldo.id &= ~(1<<idx);
                }
                g_pmdLdoUserBitmap[profile][idx] |= user;
            }
            else
            {
                g_pmdLdoUserBitmap[profile][idx] &= ~user;
                if (g_pmdLdoUserBitmap[profile][idx] != 0)
                {
                    ldo.id &= ~(1<<idx);
                }
            }
        }
        ldoId >>= 1;
        idx++;
    }

    if (ldo.id == 0)
    {
        hal_SysExitCriticalSection(scStatus);
        return;
    }

    if (on)
    {
        g_pmdLdoProfileSetting1[profile].reg &= ~(ldo.id & RDA_PMU_LDO_EN_MASK);
        if (ldo.id & PMD_LDO_BLLED)
        {
            g_pmdLdoProfileSetting2[profile].rgbLedOff = FALSE;
        }
    }
    else
    {
        g_pmdLdoProfileSetting1[profile].reg |= ldo.id & RDA_PMU_LDO_EN_MASK;
        if (ldo.id & PMD_LDO_BLLED)
        {
            g_pmdLdoProfileSetting2[profile].rgbLedOff = TRUE;
        }
    }

    if (profile == PMD_PROFILE_MODE_LOWPOWER)
    {
        reg_offset = 5;
    }

    if (g_pmdInitDone)
    {
        pmd_RDAWrite(RDA_ADDR_LDO_ACTIVE_SETTING1+reg_offset,g_pmdLdoProfileSetting1[profile].reg);
        if (ldo.id & PMD_LDO_BLLED)
        {
            pmd_RDAWrite(RDA_ADDR_LDO_ACTIVE_SETTING2+reg_offset,g_pmdLdoProfileSetting2[profile].reg);
        }
    }

    hal_SysExitCriticalSection(scStatus);
}

VOID HAL_FUNC_INTERNAL hal_GpioClr(HAL_APO_ID_T apo)
{
    SWAP_8808S_GPIO_6_AND_13(apo.gpioId);
    SWAP_FP_GPIO_24_AND_25(apo.gpioId);

    switch (apo.type)
    {
        case HAL_GPIO_TYPE_NONE:
            break;
        case HAL_GPIO_TYPE_IO:
            hwp_gpio->gpio_clr = 1 << apo.id;
            break;
        case HAL_GPIO_TYPE_O:
            hwp_gpio->gpo_clr = 1 << apo.id;
            break;
        case HAL_GPIO_TYPE_TCO:
            hal_TcuClrTco( apo.id );
            break;
        default:
            HAL_ASSERT(FALSE, "Trying to use an invalid type of gpio %08x", apo.gpioId);
            break;
    }
}

VOID HAL_FUNC_INTERNAL hal_GpioSet(HAL_APO_ID_T apo)
{
    SWAP_8808S_GPIO_6_AND_13(apo.gpioId);
    SWAP_FP_GPIO_24_AND_25(apo.gpioId);

    switch (apo.type)
    {
        case HAL_GPIO_TYPE_NONE:
            break;
        case HAL_GPIO_TYPE_IO:
            hwp_gpio->gpio_set = 1 << apo.id;
            break;
        case HAL_GPIO_TYPE_O:
            hwp_gpio->gpo_set = 1 << apo.id;
            break;
        case HAL_GPIO_TYPE_TCO:
            hal_TcuSetTco( apo.id );
            break;
        default:
            HAL_ASSERT(FALSE, "Trying to use an invalid type of gpio %08x", apo.gpioId);
            break;
    }
}

PUBLIC VOID pmd_EnablePower(PMD_POWER_ID_T id, BOOL on)
{
    BOOL newState;
    UINT32 callerAddr = 0;
    UINT32 POSSIBLY_UNUSED scStatus;

    SXS_GET_RA(callerAddr);
    newState = on;
    g_pmdMapAccess.powerInfo[id] = newState;

    if (id == PMD_POWER_USB)
    {
        hal_AbbEnableUsbPhy(on);
    }
    else if (id == PMD_POWER_FM)
    {
        pmd_EnableFmInLpMode(on);
        scStatus = hal_SysEnterCriticalSection();
        g_pmdLdoSettings.fmEnable = on;
        pmd_RDAWrite(RDA_ADDR_LDO_SETTINGS, g_pmdLdoSettings.reg);
        hal_SysExitCriticalSection(scStatus);
        hal_ConfigFmIo(on);
    }
    else if (id == PMD_POWER_BT)
    {
        scStatus = hal_SysEnterCriticalSection();
        g_pmdLdoSettings.btEnable = on;
        pmd_RDAWrite(RDA_ADDR_LDO_SETTINGS, g_pmdLdoSettings.reg);
        hal_SysExitCriticalSection(scStatus);
        hal_ConfigBtIo(on);
        if (g_pmdAuxClkStatusHandler)
        {
            (*g_pmdAuxClkStatusHandler)(on);
        }
    }

    if (g_pmdConfig->power[id].ldo.type == PMD_LDO_ID_TYPE_OPAL)
    {
        pmd_EnableOpalLdo(g_pmdConfig->power[id].ldo, newState,
                          PMD_PROFILE_MODE_NORMAL, POWER_LDO_USER(id));

        if ( id == PMD_POWER_EARPIECE && (g_pmdConfig->power[id].ldo.id & PMD_LDO_MIC) != 0 )
        {
            pmd_EnableOpalLdo(g_pmdConfig->power[id].ldo, newState,
                              PMD_PROFILE_MODE_LOWPOWER, POWER_LDO_USER(id));
            if (g_pmdInitDone)
            {
                aud_EnableEarpieceSetting(on);
            }
            else
            {
                if (on)
                {
                    aud_InitEarpieceSetting();
                }
            }
        }
    }
    else
    {
        if ( (newState && !g_pmdConfig->power[id].polarity) ||
                (!newState && g_pmdConfig->power[id].polarity) )
        {
            hal_GpioClr(g_pmdConfig->power[id].ldo.pin);
        }
        else
        {
            hal_GpioSet(g_pmdConfig->power[id].ldo.pin);
        }
    }

    if (id == PMD_POWER_LCD && g_pmdInitDone)
    {
        pmd_SetHighActivity(on);
    }
}

PUBLIC VOID hal_GpioSetOut(HAL_GPIO_GPIO_ID_T gpio)
{
    SWAP_8808S_GPIO_6_AND_13(gpio);
    SWAP_FP_GPIO_24_AND_25(gpio);

    HAL_APO_ID_T apo;
    apo.gpioId = gpio;
    apo.type = HAL_GPIO_TYPE_IO;
    apo.id = gpio;

    hwp_gpio->gpio_oen_set_out = 1 << apo.id;
}

PUBLIC VOID hal_AnaGpadcOpen(HAL_ANA_GPADC_CHAN_T channel, HAL_ANA_GPADC_ATP_T atp)
{
    HAL_ASSERT(g_halCalibration == calib_GetPointers(),
               "GPADC Opening with HAL calib pointer inconsistency");

    if (channel >= HAL_ANA_GPADC_CHAN_QTY)
    {
        return;
    }

    g_halGpadcAtpConfig[channel] = atp;
    hal_AnaGpadcEnable();
}

PUBLIC VOID pmd_SetPadVoltage(UINT8 level);

VOID pmd_InitRdaPmu(VOID)
{
    pmd_InitCoreVoltage();
    pmd_SetMemVoltage(7);
    pmd_SetPadVoltage(gc_pmdPadLevelAct);
    pmd_SetMmcVoltage(1);

    pmd_RegIrqSettingSet(RDA_PMU_EOMIRQ_MASK|RDA_PMU_PENIRQ_MASK|
                         RDA_PMU_KEYIRQ_MASK|RDA_PMU_HOLD_PRECHARGER_EFUSE|
                         RDA_PMU_PD_MODE_SEL);

    pmd_RegHpDetectSettingSet(RDA_PMU_HP_IN_MASK|RDA_PMU_HP_OUT_MASK);

    pmd_InitTouchScreen();
    pmd_InitSpeakerPa();
    pmd_InitCharger();
}

INLINE UINT32 hal_SysEnterCriticalSection(VOID)
{
    UINT32 myLocalVar = *g_bootSysCsAddress;
    asm volatile ("");
    return myLocalVar;
}

INLINE VOID hal_SysExitCriticalSection(UINT32 status)
{
    asm volatile ("");
    *g_bootSysCsAddress = status;
}

PUBLIC VOID pmd_RDAWrite(RDA_REG_MAP_T regIdx, UINT16 value)
{
    hal_IspiRegWrite(HAL_ISPI_CS_PMU, regIdx, value);
}

PUBLIC VOID pmd_SetPadVoltage(UINT8 level)
{
    UINT32 scStatus = hal_SysEnterCriticalSection();
    g_pmdLdoProfileSetting3 =
        PMU_SET_BITFIELD(g_pmdLdoProfileSetting3,
                         RDA_PMU_VPAD_IBIT,
                         level);
    pmd_RDAWrite(RDA_ADDR_LDO_ACTIVE_SETTING3, g_pmdLdoProfileSetting3);
    pmd_RDAWrite(RDA_ADDR_LDO_LP_SETTING3, g_pmdLdoProfileSetting3);
    hal_SysExitCriticalSection(scStatus);
}

PUBLIC VOID hal_PwlSelLevel(HAL_PWL_ID_T id, UINT8 level)
{
    UINT32 Val;
    if (HAL_PWL_NONE == id)
    {
        return;
    }
    HAL_ASSERT(g_halCfg->pwmCfg.pwlGlowPosition != (UINT32)id, "hal_PwlSelLevel: PWL Id (%d) is configured as Glowing", id);
    HAL_ASSERT((HAL_PWL_0 == id) || (HAL_PWL_1 == id), "hal_PwlSelLevel: Invalid PWL Id (%d)",id);

    if ( (HAL_PWL_1 == g_halCfg->pwmCfg.pwlGlowPosition) != (HAL_PWL_1 == id))
    {
        Val = PWM_PWL1_SET_OE;
        if (level == 0)
        {
            Val |= PWM_PWL1_FORCE_L;
        }
        else if (level == 0xff)
        {
            Val |= PWM_PWL1_FORCE_H;
        }
        else
        {
            Val |= (PWM_PWL1_EN_H | PWM_PWL1_THRESHOLD(level));
        }
        hwp_pwm->PWL1_Config = Val;
    }
    else
    {
        Val = PWM_PWL0_SET_OE;
        if (level == 0)
        {
            Val |= PWM_PWL0_FORCE_L;
        }
        else if (level == 0xff)
        {
            Val |= PWM_PWL0_FORCE_H;
        }
        else
        {
            Val |= (PWM_PWL0_EN_H | PWM_PWL_MIN(level));
        }
        hwp_pwm->PWL0_Config = Val;
    }
    hal_PwmResourceMgmt();
}

PROTECTED VOID pmd_EnableClk4mAvdd3InLpMode(PMD_CLK4M_AVDD3_USER_ID_T user, BOOL on)
{
    UINT32 scStatus = hal_SysEnterCriticalSection();

    g_pmdClk4mAvdd3EnRegistryLp[user] = on;

    if (!on)
    {
        for (UINT32 i=0; i<PMD_CLK4M_AVDD3_USER_ID_QTY; i++)
        {
            if (g_pmdClk4mAvdd3EnRegistryLp[i])
            {
                on = TRUE;
                break;
            }
        }
    }

    if (on)
    {
        g_pmdMiscControlLp &= ~(RDA_PMU_PU_CLK_4M_DR|
                                RDA_PMU_PD_LDO_AVDD3_DR);
    }
    else
    {
        g_pmdMiscControlLp |= RDA_PMU_PU_CLK_4M_DR|
                              RDA_PMU_PD_LDO_AVDD3_DR|
                              RDA_PMU_PD_LDO_AVDD3_REG;
        g_pmdMiscControlLp &= ~RDA_PMU_PU_CLK_4M_REG;
    }

    hal_SysExitCriticalSection(scStatus);
}

unsigned char clk_get_usecount(struct clk *clk)
{
    if (clk == NULL)
        return 0;

    return clk->usecount;
}

static void __clk_enable(struct clk *clk)
{
    if (clk->parent)
        __clk_enable(clk->parent);
    if (clk->usecount++ == 0 && (clk->flags & GATE_MANUAL))
        clk_gate_config(clk->gate_data, true);
    if (clk->recalc)
        clk->rate = clk->recalc(clk);
}

void pmd_Open(const PMD_CONFIG_T* pmdConfig)
{
    UINT32 id;
    if (pmdConfig == NULL) return;

    g_pmdConfig = pmdConfig;
    g_pmdInitDone = FALSE;

    for (id = 0; id < 14; id++)
    {
        UINT32 powerOn = g_pmdConfig->power[id].powerOnState;
        pmd_EnablePower(id, powerOn);
    }

    g_pmdInitDone = TRUE;
}

PUBLIC VOID PMD_FUNC_INTERNAL pmd_SetPowerMode(PMD_POWERMODE_T powerMode)
{
    UINT8 padLevel = 0;
    UINT16 miscCtrl = 0;
    UINT16 ldoBuck1 = 0;
    UINT32 scStatus = hal_SysEnterCriticalSection();

    switch (powerMode)
    {
        case PMD_LOWPOWER:
            padLevel = gc_pmdPadLevelLp;
            miscCtrl = g_pmdMiscControlLp;
            ldoBuck1 = gc_pmdLdoBuck1Lp;
            g_pmdCalibSetting2 |= RDA_PMU_PD_CHARGE_LDO;
            break;
        case PMD_IDLEPOWER:
            padLevel = gc_pmdPadLevelAct;
            miscCtrl = g_pmdMiscControlActive;
            ldoBuck1 = gc_pmdLdoBuck1Act;
            g_pmdCalibSetting2 &= ~RDA_PMU_PD_CHARGE_LDO;
            break;
        default:
            break;
    }

    pmd_RDAWrite(RDA_ADDR_MISC_CONTROL, miscCtrl);
    pmd_RDAWrite(RDA_ADDR_CALIBRATION_SETTING2, g_pmdCalibSetting2);
    pmd_SetPadVoltage(padLevel);
    pmd_RDAWrite(RDA_ADDR_LDO_BUCK1, ldoBuck1);

    hal_SysExitCriticalSection(scStatus);
}

static unsigned long getRoundRate(unsigned long rate)
{
    int num = sizeof(gClkRateList)/sizeof(gClkRateList[0]);
    if (rate >= gClkRateList[num - 1])
        return gClkRateList[num - 1];

    if (rate <= gClkRateList[0])
        return gClkRateList[0];

    int i = 0, j = num - 1;
    while (i < j)
    {
        int mid = (i + j) / 2;
        if (gClkRateList[mid] == rate)
            return rate;

        if (gClkRateList[mid] < rate)
            i = mid + 1;
        else
            j = mid - 1;
    }

    if (rate > gClkRateList[i])
        i++;
    return gClkRateList[i];
}


int clk_request(const unsigned long four_cc, unsigned long rate)
{
    int i, ret = 0;
    struct sw_clk_consumer *scc;

    unsigned long flags = hal_SysEnterCriticalSection();

    unsigned long curr_rate = g_clk_sw->req_par_rate;
    unsigned long max_rate = rate;
    bool exist = false;

    list_for_each_entry(scc, &g_sw_consumers, node)
    {
        if (scc->four_cc == four_cc)
        {
            scc->req_rate = rate;
            exist = true;
        }
        if (scc->req_rate > max_rate)
            max_rate = scc->req_rate;
    }

    if (exist) {
        if (max_rate != curr_rate)
            ret = g_clk_sw->set_rate(g_clk_sw, max_rate);
        hal_SysExitCriticalSection(flags);
        return ret;
    }

    for (i = 0; i < MAX_SW_CLK_USER_NUM; i++) {
        if (g_consumers[i].four_cc == FOURCC_UNKNOWN) {
            break;
        }
    }

    HAL_ASSERT((i < MAX_SW_CLK_USER_NUM), "Too many clock users!");
    g_consumers[i].four_cc = four_cc;
    g_consumers[i].req_rate = rate;
    list_add_tail(&g_consumers[i].node, &g_sw_consumers);

    if (clk_get_usecount(g_clk_sw) == 0)
        __clk_enable(g_clk_sw);

    if (max_rate != curr_rate)
        ret = g_clk_sw->set_rate(g_clk_sw, max_rate);

    hal_SysExitCriticalSection(flags);
    return ret;
}

bool hal_SwRequestClk(const UINT32 fourCC, UINT32 rate)
{
    unsigned long roundRate = getRoundRate(rate);
    return 0 == clk_request(fourCC, roundRate);
}

void hal_TimWatchDogClose(void)
{
}

PUBLIC VOID pmd_SetLevel(PMD_LEVEL_ID_T id, UINT32 level)
{
    UINT32 status;
    UINT32 value;

    g_pmdMapAccess.levelInfo[id] = level;

    switch (g_pmdConfig->level[id].type)
    {
        case PMD_LEVEL_TYPE_NONE:
            break;
        case PMD_LEVEL_TYPE_PWL0:
            if (level > 7) level = 7;
            hal_PwlSelLevel(HAL_PWL_0, g_pmdConfig->lightLevelToPwm0[level]);
            break;
        case PMD_LEVEL_TYPE_PWL1:
            if (level > 7) level = 7;
            hal_PwlSelLevel(HAL_PWL_1, g_pmdConfig->lightLevelToPwm1[level]);
            break;
        case PMD_LEVEL_TYPE_LPG:
            PMD_ASSERT(FALSE, "TODO implement LPG", id);
            break;
        case PMD_LEVEL_TYPE_LDO:
            if (g_pmdConfig->level[id].ldo.type == PMD_LDO_ID_TYPE_OPAL)
            {
                pmd_EnableOpalLdo(g_pmdConfig->level[id].ldo, (level != 0),
                                  PMD_PROFILE_MODE_NORMAL, LEVEL_LDO_USER(id));

                if (id == PMD_LEVEL_VIBRATOR)
                {
                    pmd_EnableOpalLdo(g_pmdConfig->level[id].ldo, (level != 0),
                                      PMD_PROFILE_MODE_LOWPOWER, LEVEL_LDO_USER(id));
                }
            }
            else
            {
                switch (id)
                {
                    case PMD_LEVEL_LCD:
                    case PMD_LEVEL_KEYPAD:
                    case PMD_LEVEL_LED0:
                    case PMD_LEVEL_LED1:
                    case PMD_LEVEL_LED2:
                    case PMD_LEVEL_LED3:
                    case PMD_LEVEL_VIBRATOR:
                        if (0 == level)
                            hal_GpioClr(g_pmdConfig->level[id].ldo.pin);
                        else
                            hal_GpioSet(g_pmdConfig->level[id].ldo.pin);
                        break;
                    default:
                        break;
                }
            }
            break;
        case PMD_LEVEL_TYPE_OPAL:
            switch (id)
            {
                case PMD_LEVEL_SIM:
                    pmd_SimSetVoltageClass(level);
                    break;
                case PMD_LEVEL_KEYPAD:
                case PMD_LEVEL_LED0:
                case PMD_LEVEL_LED1:
                case PMD_LEVEL_LED2:
                case PMD_LEVEL_LED3:
                {
                    if (level > 7) level = 7;
                    value = g_pmdConfig->lightLevelToPwm0[level];
                    value = (value & 0xff) >> 5;

                    UINT32 ibitMask, ibit;
                    if (id == PMD_LEVEL_LED0) {
                        ibitMask = RDA_PMU_LED_R_IBIT_MASK;
                        ibit = RDA_PMU_LED_R_IBIT(value);
                    }
                    else if (id == PMD_LEVEL_LED1) {
                        ibitMask = RDA_PMU_LED_G_IBIT_MASK;
                        ibit = RDA_PMU_LED_G_IBIT(value);
                    }
                    else if (id == PMD_LEVEL_LED2) {
                        ibitMask = RDA_PMU_LED_B_IBIT_MASK;
                        ibit = RDA_PMU_LED_B_IBIT(value);
                    }
                    else {
                        ibitMask = RDA_PMU_LED_R_IBIT_MASK|RDA_PMU_LED_G_IBIT_MASK|RDA_PMU_LED_B_IBIT_MASK;
                        ibit = RDA_PMU_LED_R_IBIT(value)|RDA_PMU_LED_G_IBIT(value)|RDA_PMU_LED_B_IBIT(value);
                    }

                    UINT32 mask;
                    PMD_BG_LP_MODE_USER_ID_T bgUser;
                    PMD_CLK4M_AVDD3_USER_ID_T clk4mUser;
                    if (id == PMD_LEVEL_LED0) {
                        mask = RDA_PMU_LED_R_OFF_ACT|RDA_PMU_LED_R_OFF_LP;
                        bgUser = PMD_BG_LP_MODE_USER_LED0;
                        clk4mUser = PMD_CLK4M_AVDD3_USER_LED0;
                    }
                    else if (id == PMD_LEVEL_LED1) {
                        mask = RDA_PMU_LED_G_OFF_ACT|RDA_PMU_LED_G_OFF_LP;
                        bgUser = PMD_BG_LP_MODE_USER_LED1;
                        clk4mUser = PMD_CLK4M_AVDD3_USER_LED1;
                    }
                    else if (id == PMD_LEVEL_LED2) {
                        mask = RDA_PMU_LED_B_OFF_ACT|RDA_PMU_LED_B_OFF_LP;
                        bgUser = PMD_BG_LP_MODE_USER_LED2;
                        clk4mUser = PMD_CLK4M_AVDD3_USER_LED2;
                    }
                    else {
                        if (id == PMD_LEVEL_LED3) {
                            mask = RDA_PMU_LED_R_OFF_ACT|RDA_PMU_LED_R_OFF_LP;
                            bgUser = PMD_BG_LP_MODE_USER_LED3;
                            clk4mUser = PMD_CLK4M_AVDD3_USER_LED3;
                        } else {
                            mask = RDA_PMU_LED_G_OFF_ACT|RDA_PMU_LED_G_OFF_LP|RDA_PMU_LED_B_OFF_ACT|RDA_PMU_LED_B_OFF_LP;
                            bgUser = PMD_BG_LP_MODE_USER_KEYPAD_LED;
                            clk4mUser = PMD_CLK4M_AVDD3_USER_KEYPAD_LED;
                        }
                    }

                    PMD_LDO_ID_T ldo;
                    ldo.opal = PMD_LDO_BLLED;
                    pmd_EnableOpalLdo(ldo, (level != 0), PMD_PROFILE_MODE_LOWPOWER, LEVEL_LDO_USER(id));
                    pmd_EnableOpalLdo(ldo, (level != 0), PMD_PROFILE_MODE_NORMAL, LEVEL_LDO_USER(id));
                    pmd_ChargerSetBgLpMode(bgUser, (level == 0));
                    pmd_EnableClk4mAvdd3InLpMode(clk4mUser, (level != 0));

                    status = hal_SysEnterCriticalSection();
                    if (0 == level) {
                        g_pmdLedSetting5 |= mask;
                    } else {
                        g_pmdLedSetting5 &= ~mask;
                        g_pmdLedSetting5 = (g_pmdLedSetting5 & (~ibitMask)) | ibit;
                    }
                    pmd_RDAWrite(RDA_ADDR_LED_SETTING5, g_pmdLedSetting5);
                    hal_SysExitCriticalSection(status);
                }
                break;
                default:
                    break;
            }
            break;
        case PMD_LEVEL_TYPE_BACKLIGHT:
        {
            if (level > 7) level = 7;
            if (level <= 3)
                g_pmdThermalCalib |= (1 << 1);
            else
                g_pmdThermalCalib &= ~(1 << 1);

            value = g_pmdConfig->lightLevelToBacklight[level];
            value = (value & 0xff) >> 4;

            status = hal_SysEnterCriticalSection();
            if (0 == level) {
                g_pmdBacklightOn = FALSE;
                g_pmdLedSetting2 |= RDA_PMU_BL_OFF_ACT;
            } else {
                g_pmdBacklightOn = TRUE;
                g_pmdLedSetting2 &= ~RDA_PMU_BL_OFF_ACT;
                g_pmdLedSetting2 = PMU_SET_BITFIELD(g_pmdLedSetting2, RDA_PMU_BL_IBIT_ACT, value);
            }
            pmd_RDAWrite(RDA_ADDR_LED_SETTING2, g_pmdLedSetting2);
            pmd_RDAWrite(RDA_ADDR_THERMAL_CALIBRATION, g_pmdThermalCalib);

            PMD_LDO_ID_T blLdo = { .opal = PMD_LDO_BLLED };
            pmd_EnableOpalLdo(blLdo, (level != 0), PMD_PROFILE_MODE_NORMAL, LEVEL_LDO_USER(id));
            hal_SysExitCriticalSection(status);
        }
        break;
        default:
            break;
    }
}



// ====================================================================
// 8. CRITICAL ENTRY INITIALIZATION INTERFACE VECTOR


// ====================================================================
int main(void) {
    UINT16 i;
    UINT32 staticCsAddress = 0;
    g_bootSysCsAddress = &staticCsAddress;

    for (i = 0; i < HAL_MAP_ID_COUNT; i++)
    {
        g_halMapTable.modules[i].version = NULL;
        g_halMapTable.modules[i].access = NULL;
    }

    *UNCACHED(&g_halFixedPointer) = (UINT32)&g_halMapTable;

    pmd_Open(&g_tgtPmdConfig);
    pmd_SetPowerMode(PMD_IDLEPOWER);
    hal_SwRequestClk(FOURCC_TOOL, HAL_CLK_RATE_104M);
    hal_TimWatchDogClose();
    pmd_SetLevel(PMD_LEVEL_LCD, 7);
    pmd_SetLevel(PMD_LEVEL_KEYPAD, 7);

    while (1) {
       // Custom bare-metal code continues here loop...
    }

    return 0;
}







// ====================================================================
// GLOBAL INSTANTIATIONS
// ====================================================================
struct sw_clk_consumer g_consumers[MAX_SW_CLK_USER_NUM];
struct list_head g_sw_consumers = { (struct list_head*)&g_sw_consumers, (struct list_head*)&g_sw_consumers };
struct clk fake_clk_sw = { 26000000, NULL, 0, NULL, 0, 0, 0, NULL };
struct clk *g_clk_sw = &fake_clk_sw;
