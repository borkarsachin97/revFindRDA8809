import os
import re

header_dir = "helpfull-headers"
bases = {}

# 1. First pass: find bases like REG_SYS_CTRL_BASE 0x01A00000
for root, _, files in os.walk(header_dir):
    for f in files:
        if f.endswith("_asm.h"):
            with open(os.path.join(root, f), "r") as fh:
                for line in fh:
                    m = re.match(r"#define\s+(REG_[A-Z0-9_]+_BASE)\s+(0x[0-9A-Fa-f]+)", line.strip())
                    if m:
                        base_name = m.group(1)
                        base_val = int(m.group(2), 16)
                        if 0x01A00000 <= base_val <= 0x01A0FFFF:
                            bases[base_name] = base_val

# Dictionary to store mapping: abs_addr -> [macro_name]
macro_map = {}

# 2. Second pass: find macros like REG_SYS_CTRL_SYS_RST_SET REG_SYS_CTRL_BASE_LO + 0x00000004
for root, _, files in os.walk(header_dir):
    for f in files:
        if f.endswith("_asm.h"):
            with open(os.path.join(root, f), "r") as fh:
                for line in fh:
                    m = re.match(r"#define\s+(REG_[A-Z0-9_]+)\s+(REG_[A-Z0-9_]+_BASE_LO)\s*\+\s*(0x[0-9A-Fa-f]+)", line.strip())
                    if m:
                        macro = m.group(1)
                        base_lo_macro = m.group(2)
                        offset = int(m.group(3), 16)

                        base_name = base_lo_macro.replace("_LO", "")
                        if base_name in bases:
                            abs_addr = bases[base_name] + offset
                            macro_map[abs_addr] = macro

for addr, macro in sorted(macro_map.items()):
    print(f"0x{addr:08X}: {macro}")
