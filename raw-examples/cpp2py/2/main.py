#!/usr/bin/python3

import mypy
obj = mypy.Object()
obj.setName('paoqi')
obj.setSum(648)
content = 'Name: %s, Sum: %d'%(obj.getName(), obj.getSum())
print(content)
