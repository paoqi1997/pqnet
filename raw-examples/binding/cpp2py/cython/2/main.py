#!/usr/bin/python3

import math
import os

def build():
    sCmd = 'python3 setup.py build_ext --inplace'
    os.system(sCmd)

build()

if __name__ == '__main__':
    import c_object
    print(c_object.foo_add(1, 2))
    obj = c_object.Object()
    obj.setName('paoqi'.encode('utf-8'))
    obj.setMoney(648)
    print(obj.getName())
    print(obj.getMoney())
