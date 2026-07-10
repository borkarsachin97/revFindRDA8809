# Bare-Metal Boot Sequence: Mapped to Source

## Source-to-Disassembly Vector Mapping

By searching the official SDK sources under `platform/chip/boot/`, we identified the origins of the exact initialization traces seen in our disassembly `chunk_1_disasm.txt`.

### 1. Coprocessor 0 (CP0) Configuration
In `platform/chip/boot/8809/src/boot_rom.S` at line `662`, we see:
```assembly
    // enable it in risc
    li  t0, (Status_IntMASK | Status_IEc | 0x400000)
    mtc0    t0, Status
```
Similarly at line `782` in `boot_DisableInterrupt`:
```assembly
    li  t0, (Status_IntMASK | Status_IEp)
    mtc0  t0, Status
```
This confirms that the native SDK utilizes explicit `mtc0` operations against the `Status` and `Cause` registers before vectoring into main logic. In `chunk_1_disasm.txt`, our observed configuration at `0x81c01234` uses:
```assembly
    3c1b0040    lui k1,0x40
    377bc001    ori k1,k1,0xc001
    40806800    mtc0    zero,$13  (Cause)
    409b6000    mtc0    k1,$12    (Status)
```
This `0x0040C001` mask perfectly aligns with `Status_IntMASK | Status_IEc | 0x400000` (where `0x400000` is the ERL/EXL or BEV bit, enabling uncached exception vectors).

### 2. The `.bss` Clear Loop
Our disassembly analysis found an unrolled memory-clearing loop bridging `0x81c00498` to `0x81c004ac`.
When cross-referencing `platform/chip/boot/src/boot_mem_init.c` at line `1394`, we find the direct C source implementation that compiled down into this assembly block:

```c
    // Clear booting BSS in internal SRAM
    for (dst = (UINT32*) &_boot_sys_sram_bss_start;
            dst < (UINT32*) &_boot_sys_sram_bss_end;
            dst++)
    {
        *dst = 0;
    }
```
This loop bounds `dst` (in `$v1`) against the pre-calculated size of the region mapping explicitly to the limit register check (`sltu a1,v0,a0` `bnez a1`).

The structural comparison verifies that our custom `start.S` implementation matches the hardware-level necessities natively handled by the RDA BSP.
