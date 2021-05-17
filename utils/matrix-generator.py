#!/usr/bin/env python3
from random import random
import numpy as np
import sys
import struct

dataname = sys.argv[1]
n = int(sys.argv[2])

a = (b := np.full((n,n), 5))
c = np.full((n,n), 5*5*n)

with open(dataname, "bw") as f:
    f.write(struct.pack("@N", n))
    f.write(struct.pack("@N", n))
    f.write(struct.pack("@N", n))

    print("Matrix a")

    for row in a:
        for e in row:
            f.write(struct.pack("@d", e))

    print("Matrix b")

    for row in b:
        for e in row:
            f.write(struct.pack("@d", e))

    print("Matrix c")

    for row in c:
        for e in row:
            f.write(struct.pack("@d", e))

    print("Finished")
