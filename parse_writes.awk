{
    if (match($0, /lui[ \t]+([a-z0-9]+),(0x[a-f0-9]+)/, m)) {
        reg[m[1]] = strtonum(m[2]) * 65536
    }
    if (match($0, /ori[ \t]+([a-z0-9]+),([a-z0-9]+),(0x[a-f0-9]+)/, m)) {
        if (m[2] in reg) {
            reg[m[1]] = or(reg[m[2]], strtonum(m[3]))
        }
    }
    if (match($0, /addiu[ \t]+([a-z0-9]+),([a-z0-9]+),(-?[0-9]+)/, m)) {
        if (m[2] in reg) {
            reg[m[1]] = reg[m[2]] + m[3]
        }
    }
    if (match($0, /(sw|sb|lw|lb)[ \t]+([a-z0-9]+),(-?[0-9]+)\(([a-z0-9]+)\)/, m)) {
        base = m[4]
        offset = m[3]
        if (base in reg) {
            addr = reg[base] + offset
            # Filter for specific peripheral bases mapped through KSEG1
            if (addr >= 0xA1A00000 && addr <= 0xA1A2FFFF) {
                printf "0x%08X: %s\n", addr, $0
            }
        }
    }
}
