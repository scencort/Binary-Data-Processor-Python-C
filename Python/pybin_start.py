import struct

fmt = "<Id16s8s"

stud_info = [
    (1, 4.8, b"Polyakov\x00", b"-403IS\x00",),
    (3, 3.8, b"Puchkov\x00", b"-403IS\x00",),
    (2, 4.8, b"Petrov\x00", b"-403IS\x00",),
    (4, 3.5, b"Safonov\x00", b"-403IS\x00",),
]

with open("data.bin", "wb") as file:
    for si in stud_info:
        p = struct.pack(fmt, *si)
        file.write(p)