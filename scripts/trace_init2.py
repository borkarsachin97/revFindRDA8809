import re

current_base_reg = {}

with open("chunk_1_disasm.txt", "r") as f:
    for line in f:
        # Stop tracing after we go past the early boot section
        # (Let's say after 1000 lines or we hit a recognizable main loop)

        lui_match = re.search(r"lui\s+([a-z0-9]+),0x([a-f0-9]+)", line)
        if lui_match:
            reg = lui_match.group(1)
            val = (int(lui_match.group(2), 16) << 16) & 0xFFFFFFFF
            current_base_reg[reg] = val
            continue

        add_match = re.search(r"addiu?\s+([a-z0-9]+),([a-z0-9]+),-?(\d+)", line)
        if add_match:
            dst = add_match.group(1)
            src = add_match.group(2)
            imm = int(add_match.group(3))
            if src in current_base_reg and "-" not in line:
                current_base_reg[dst] = current_base_reg[src] + imm
            continue

        ori_match = re.search(r"ori\s+([a-z0-9]+),([a-z0-9]+),0x([a-f0-9]+)", line)
        if ori_match:
            dst = ori_match.group(1)
            src = ori_match.group(2)
            imm = int(ori_match.group(3), 16)
            if src in current_base_reg:
                current_base_reg[dst] = current_base_reg[src] | imm
            continue

        mem_match = re.search(r"(sw|sb|sh)\s+([a-z0-9]+),(-?\d+)\(([a-z0-9]+)\)", line)
        if mem_match:
            op = mem_match.group(1)
            src_reg = mem_match.group(2)
            offset = int(mem_match.group(3))
            base_reg = mem_match.group(4)

            if base_reg in current_base_reg:
                addr = (current_base_reg[base_reg] + offset) & 0xFFFFFFFF

                # Check hardware addresses
                if 0xA1A00000 <= addr <= 0xA1A0FFFF or 0xA1A20000 <= addr <= 0xA1A2FFFF:
                    print(f"{line.strip()} -> WRITE to 0x{addr:08X}")
