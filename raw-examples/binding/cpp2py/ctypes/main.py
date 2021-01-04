#!/usr/bin/python3

import os
import platform
import sys

# help: https://docs.python.org/zh-cn/3.6/library/ctypes.html
from ctypes import *

class Coord(Structure):
    _fields_ = [
        ('x', c_int),
        ('y', c_int)
    ]

if __name__ == '__main__':
    dName = {
        'Windows': 'util.dll',
        'Linux': 'libutil.so'
    }
    sKey = platform.system()
    sPath = os.path.join(sys.path[0], dName[sKey])

    oMod = cdll.LoadLibrary(sPath)
    print(oMod.add(1, 2))

    arrType = c_int * 10
    arr = arrType()
    arr[2] = c_int(6)
    for idx in range(10):
        print(oMod.getElement(arr, idx), end=' ')
    print()

    oCoord = Coord(1, 2)
    print(oCoord.x, oCoord.y)
    oMod.setX(byref(oCoord), 2)
    oMod.setY(byref(oCoord), 4)
    print(oCoord.x, oCoord.y)
