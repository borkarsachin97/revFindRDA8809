# Source Code to Disassembly Mapping: Flash Programmer

To bridge our understanding from the C source implementation in `src/flash_programmer/` to the bootloader disassembly in `chunk_1_disasm.txt`, we trace the execution flow of the Flash Command Interface and Command Polling loops.

## 1. Flash Command Interface & Mailbox Registers

The Host PC communicates with the flash programmer using a Mailbox/Command Structure stored in memory. In `src/flash_programmer/flash_prog_map.h`, this is defined as:

```c
typedef struct {
    FPC_COMMAND_TYPE_T cmd;       // 0x00000000 (Command Enum, e.g., FPC_PROGRAM, FPC_ERASE)
    UINT8*             flashAddr; // 0x00000004 (Destination Address in Flash)
    UINT8*             ramAddr;   // 0x00000008 (Source Address in SRAM Buffer)
    UINT32             size;      // 0x0000000C (Payload Size)
    UINT32             fcs;       // 0x00000010 (Frame Check Sequence/CRC)
} FPC_COMMAND_DATA_T;
```
A globally registered struct `g_flashProgDescr` (of type `FLASH_PROG_MAP_ACCESS_T`) acts as the mailbox, containing two such command descriptors. It is registered via `hal_MapEngineRegisterModule(HAL_MAP_ID_FLASH_PROG, ...)`, so the Host tool (like an RDA flasher over UART/USB) knows the SRAM address to write commands to.

The tool parses these variables via uncached memory to avoid cache-coherency issues.


## 2. The Main Command Polling Loop

Once initialization completes (clocks are stable, CRC validation passed), the bootloader triggers a `SEND_EVENT(EVENT_FLASH_PROG_READY)`. The macro for `EVENT_FLASH_PROG_READY` is `0xF0`.
In the disassembly, we see this specific value being generated and pushed to a debug trace register or the UART via the `0xA1A3F000` block (if mapped to an event/trace system):
```assembly
81c00470:	3c1fa1a3 	lui	ra,0xa1a3
81c00474:	37e9f000 	ori	t1,ra,0xf000
...
81c00484:	ad200014 	sw	zero,20(t1)
```

The C code then enters a `while (1)` loop, spinning on `c = cmds[i].cmd`. Since `cmds` is located in uncached SRAM (configured via `g_flashProgDescr`), the compiled assembly accesses this memory using raw `lw` (load word) instructions.

A prominent execution structure reflecting this loop spans across instructions loading from the `cmds` array mapped in SRAM and branching heavily using `beq` / `bne` corresponding to the C `switch (c)` statement over `FPC_COMMAND_TYPE_T` enums (e.g. `FPC_SHUTDOWN`=0x10, `FPC_DONE`=0xFFFFFF9C).

## 3. Flash Memory Translation Layer

In the `flash_programmer.c` source, target payload execution happens through mapping the `FPC_COMMAND_DATA_T` addresses to HAL layer translation macros:

```c
if (!hal_SpiFlashWrite((uint32_t)cmds[i].flashAddr, cmds[i].ramAddr, cmds[i].size))
```
- `cmds[i].flashAddr`: The destination inside the SPI Flash layout (frequently translated to physical address space around `0x08000000` via CS0 base macros, as indicated in `mem_bridge_asm.h`).
- `cmds[i].ramAddr`: The source buffer inside SRAM (`0x81C0XXXX` or `0x81D0XXXX` depending on staging memory).
- `cmds[i].size`: Derived straight from the mailbox structure.

Once standard commands succeed, it writes "Magic Numbers" to `flashSpecialSector[cpt].address`. To verify the write was successful, the application issues `hal_SpiFlashMapUncacheAddress`:
```c
void *globalAddress = hal_SpiFlashMapUncacheAddress((uint32_t)flashSpecialSector[cpt].address);
if (*((UINT32 *)globalAddress) != flashSpecialSector[cpt].magicNumber)
```
This forces an uncached read over the memory bridge (`MEM_BRIDGE`) for the freshly written data, bypassing the XCPU Data Cache. This ensures that the flash programmer confirms the exact physical signals were latched into the SPI ROM, eliminating caching false-positives before emitting an `FPC_DONE` event back up to the PC.
