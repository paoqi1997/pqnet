#!/usr/bin/python3

import os

if __name__ == '__main__':
    sCmd = 'python3 setup.py build_ext --inplace'
    os.system(sCmd)
    import mypy
    print(mypy.add(1, 2))
