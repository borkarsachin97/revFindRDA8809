import re

# 1. Load macro map
macro_map = {}
with open("scripts/macro_map.txt", "r") as f:
    for line in f:
        if ":" in line:
            addr_str, macro = line.split(":", 1)
            addr = int(addr_str.strip(), 16)
            macro_map[addr] = macro.strip()

# 2. Parse disassembly for accesses
accesses = {}
current_base_reg = {}

with open("chunk_1_disasm.txt", "r") as f:
    for line in f:
        lui_match = re.search(r"lui\s+([a-z0-9]+),0x([a-f0-9]+)", line)
        if lui_match:
            reg = lui_match.group(1)
            # Mask to 0x01A0xxxx instead of 0xA1A0xxxx to match macros
            val = (int(lui_match.group(2), 16) << 16) & 0x1FFFFFFF
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

        mem_match = re.search(r"(sw|lw|sb|lhu|lbu|lb)\s+([a-z0-9]+),(-?\d+)\(([a-z0-9]+)\)", line)
        if mem_match:
            op = mem_match.group(1)
            offset = int(mem_match.group(3))
            base_reg = mem_match.group(4)

            if base_reg in current_base_reg:
                addr = (current_base_reg[base_reg] + offset) & 0x1FFFFFFF

                # Check if it's in our range 0x01A00000 - 0x01A0FFFF
                if 0x01A00000 <= addr <= 0x01A0FFFF:
                    if addr not in accesses:
                        accesses[addr] = []
                    accesses[addr].append(f"({op}) {line.strip()}")

# 3. Output Master Register Map
print("# Master Peripheral Register Map (0x01A00000 - 0x01A0FFFF)\n")

blocks = {
    0x01A00000: "SYS_CTRL",
    0x01A01000: "SYS_IRQ",
    0x01A02000: "TIMER",
    0x01A03000: "GPIO",
    0x01A04000: "MEM_BRIDGE",
    0x01A05000: "KEYPAD",
    0x01A06000: "PWM",
    0x01A07000: "I2C_MASTER",
    0x01A08000: "DMA",
    0x01A09000: "SYS_IFC",
    0x01A0A000: "CALENDAR",
    0x01A0B000: "SYS_COMREGS",
    0x01A0C000: "PAGE_SPY",
    0x01A0D000: "DEBUG_PORT",
    0x01A0E000: "RF_SPI",
    0x01A0F000: "TCU",
}

for base_addr, block_name in sorted(blocks.items()):
    print(f"## {len([k for k in blocks.keys() if k <= base_addr])}. {block_name} Base (0x{base_addr:08X} / 0x{(base_addr | 0xA0000000):08X})")

    # Get all macros for this block
    block_macros = {addr: macro for addr, macro in macro_map.items() if base_addr <= addr < base_addr + 0x1000}

    # Add any addresses that were accessed but not in headers
    for addr in accesses:
        if base_addr <= addr < base_addr + 0x1000 and addr not in block_macros:
            block_macros[addr] = "[UNKNOWN MACRO]"

    for addr, macro in sorted(block_macros.items()):
        print(f"- **0x{addr:08X}**: `{macro}`")
        if addr in accesses:
            print(f"  - **Disassembly Context**:")
            for acc in accesses[addr][:3]: # Limit to first 3 instances for brevity
                print(f"    - `{acc}`")
            if len(accesses[addr]) > 3:
                print(f"    - ... and {len(accesses[addr]) - 3} more.")
    print("\n")
