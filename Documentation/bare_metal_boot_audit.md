# Bare Metal Boot Sequence Audit

## 1. Coprocessor 0 (CP0) Handling
MIPS systems utilize CP0 for hardware and exception states. In our disassembly, around `0x81c00de0` and `0x81c01234`, we see explicit CP0 manipulations:

```assembly
81c0122c:   3c1b0040    lui k1,0x40
81c01230:   377bc001    ori k1,k1,0xc001
81c01234:   40806800    mtc0    zero,$13  ; CP0 Cause register
81c01238:   00000000    nop
81c0123c:   409b6000    mtc0    k1,$12    ; CP0 Status register
```
- `$13` is the **Cause** register. Writing `zero` clears all pending software interrupts and sets the exception code back to a clean slate.
- `$12` is the **Status** register. `0x0040C001` corresponds to specific interrupt masks, the Exception Level (ERL/EXL) bits, and global interrupt enable (`IE`). This establishes a controlled exception state before dropping down to the main processing layer.

## 2. Block Zeroing (.bss Clearing)
At `0x81c00498` to `0x81c004ac`, we observe a classic C-runtime (CRT) startup memory zeroing/moving loop:
```assembly
81c00494:   24e40000    addiu   a0,a3,0    ; Target limit
81c00498:   8c4a0000    lw      t2,0(v0)   ; Load word
81c0049c:   24420004    addiu   v0,v0,4    ; Increment src
81c004a0:   ac6a0000    sw      t2,0(v1)   ; Store word
81c004a4:   24630004    addiu   v1,v1,4    ; Increment dst
81c004a8:   0044282b    sltu    a1,v0,a0   ; Compare to end bound
81c004ac:   14a0fffa    bnez    a1,0x81c00498
```
This loop ensures uninitialized variables inside `.bss` are cleared or initialized data `.data` is copied from Flash into SRAM before invoking `main()`.

## Native Start Sequence vs Custom Start.S
To build a clean-room `start.S`, we must replicate the sequence derived from `chunk_1_disasm.txt`:
1. Vector at `0x81c00280`.
2. Clear CP0 Cause (`mtc0 zero, $13`).
3. Set CP0 Status (`mtc0 k1, $12` where `k1` configures system constraints).
4. Set up Stack Pointer (`sp = 0x81C0XXXX` top of SRAM).
5. Zero out `.bss` region dynamically mapped from link.ld variables.
6. Jump into C execution `main()`.
