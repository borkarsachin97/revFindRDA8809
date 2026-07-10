# RDA 8809 Boot Sequence Analysis

## Entry Point

The `8809_00400000.fp` binary contains a small header chunk (Chunk 0) and the main firmware body (Chunk 1).

### Chunk 0 (0x81c000a0)
Chunk 0 starts with a `nop` followed by an absolute address pointer `0x81c00280`. This pointer specifies the execution entry point in Chunk 1.

```assembly
81c000a0:	00000000 	nop
81c000a4:	81c00280 	lb	zero,640(t6)  ; This is data, an address pointer to 0x81c00280
```

### Chunk 1 Entry (0x81c00280)

Execution begins at `0x81c00280`. The first steps involve checking hardware states and initializing basic environment:
1. Load `0x81c000cc` into `v1`. This address could contain system status or configuration.
2. Setup the stack pointer (`sp`): `addiu sp,sp,-40`.
3. Save registers `s0`, `ra`, `s1` to the stack.
4. It reads memory at `0x81c00278` and processes bitfields to determine subsequent control flow.

Control flow handles setup and initialization before moving towards a major loop or jumping to other functional blocks (like `0x81e0...` routines).
