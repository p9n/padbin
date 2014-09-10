#!/usr/bin/env python

# a simple script for decompress .ab file

import zlib, os

tar = open('pad.tar', 'w')

d = zlib.decompressobj()

ab = open('pad.ab', 'r')
s = ab.read(24)
while True :
    s = ab.read(1024 * 1024)
    if s == "" : break
    t = d.decompress(s)
    tar.write(t)

ab.close()
tar.close()

os.system("tar -xvf pad.tar --wildcards --no-anchored 'data*.bin'")
