import sys
import struct

def parse(filename):
    with open(filename, 'rb') as f:
        magic = f.read(3)
        if magic != b'LOD':
            print("Invalid magic")
            return

        unk = f.read(3)

        chunk_idx = 0
        while True:
            addr_bytes = f.read(4)
            if not addr_bytes or len(addr_bytes) < 4:
                break
            addr = struct.unpack('<I', addr_bytes)[0]

            size_bytes = f.read(2)
            if not size_bytes or len(size_bytes) < 2:
                break
            size = struct.unpack('<H', size_bytes)[0]

            data = f.read(size)
            with open(f"chunk_{chunk_idx}_0x{addr:08x}.bin", "wb") as out:
                out.write(data)

            unk_byte = f.read(1)
            if not unk_byte:
                break
            chunk_idx += 1

parse('8809_00400000_usb.fp')
