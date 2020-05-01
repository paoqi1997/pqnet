#!/usr/bin/python3

import math
import os

def build():
    sCmd = 'python3 setup.py build_ext --inplace'
    os.system(sCmd)

build()

if __name__ == '__main__':
    import c_foo
    import c_math
    print(c_foo.add(1, 2))
    # c_foo
    print(c_foo.foo_sin(math.pi / 6.0))
    # c_math
    print(c_math.sin(math.pi / 6.0))
    # math
    print(math.sin(math.pi / 6.0))
