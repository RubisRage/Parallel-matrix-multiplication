#!/usr/bin/env python3
from random import random
import numpy as np
import sys
import struct

dataname = "data/" + sys.argv[1] + ".data"
resname = dataname + ".res"
n = int(sys.argv[2])
p = int(sys.argv[3])
m = int(sys.argv[4])
max_value = int(sys.argv[5])

rng = np.random.default_rng()

a = max_value*rng.random((n,p))
b = max_value*rng.random((p,m))

with open(dataname, "bw") as f:
    f.write(struct.pack("@N", n))
    f.write(struct.pack("@N", p))
    f.write(struct.pack("@N", m))

    for row in a:
        for e in row:
            print(e)
            f.write(struct.pack("@d", e))

    for row in b:
        for e in row:
            print(e)
            f.write(struct.pack("@d", e))

    c = np.dot(a,b)

    for row in c:
        for e in row:
            print(e)
            f.write(struct.pack("@d", e))
