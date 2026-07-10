import re

with open("chunk_1_disasm.txt", "r") as f:
    lines = f.readlines()

current_base_reg = {}

for line in lines:
    lui_match = re.search(r"lui\s+([a-z0-9]+),0x([a-f0-9]+)", line)
    if lui_match:
        reg = lui_match.group(1)
        val = int(lui_match.group(2), 16) << 16
        current_base_reg[reg] = val
        continue

    ori_match = re.search(r"ori\s+([a-z0-9]+),([a-z0-9]+),0x([a-f0-9]+)", line)
    if ori_match:
        dst = ori_match.group(1)
        src = ori_match.group(2)
        imm = int(ori_match.group(3), 16)
        if src in current_base_reg:
            current_base_reg[dst] = current_base_reg[src] | imm
        continue

    # e.g. 81c00bd0:   8ca300a0    lw  v1,160(a1)
    mem_match = re.search(r"(sw|lw|sb|lhu|lbu|lb)\s+([a-z0-9]+),(-?\d+)\(([a-z0-9]+)\)", line)
    if mem_match:
        op = mem_match.group(1)
        offset = int(mem_match.group(3))
        base_reg = mem_match.group(4)

        if base_reg in current_base_reg:
            addr = current_base_reg[base_reg] + offset
            if (addr & 0xFFFF0000) == 0xA1A00000:
                print(f"{line.strip()} -> accesses 0x{addr:08X} (SYS_CTRL block)")
