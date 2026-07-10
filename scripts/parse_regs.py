import re

with open("chunk_1_disasm.txt", "r") as f:
    lines = f.readlines()

bases = {}
current_base_reg = {}

for line in lines:
    lui_match = re.search(r"lui\s+([a-z0-9]+),0x([a-f0-9]+)", line)
    if lui_match:
        reg = lui_match.group(1)
        val = int(lui_match.group(2), 16) << 16
        current_base_reg[reg] = val
        continue

    mem_match = re.search(r"(sw|lw|sb|lhu|lbu|lb)\s+([a-z0-9]+),(-?\d+)\(([a-z0-9]+)\)", line)
    if mem_match:
        op = mem_match.group(1)
        src_dst_reg = mem_match.group(2)
        offset = int(mem_match.group(3))
        base_reg = mem_match.group(4)

        if base_reg in current_base_reg:
            base_addr = current_base_reg[base_reg]
            if base_addr not in bases:
                bases[base_addr] = {}
            if offset not in bases[base_addr]:
                bases[base_addr][offset] = []
            bases[base_addr][offset].append((op, src_dst_reg, line.strip()))

for base, offsets in sorted(bases.items()):
    print(f"\nBase: 0x{base:08X}")
    for offset, accesses in sorted(offsets.items()):
        print(f"  Offset: 0x{offset:04X} ({offset})")
        for acc in accesses:
            print(f"    - {acc[2]}")
