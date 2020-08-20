#!/usr/bin/python3

import mypy
print('doc: %s'%mypy.__doc__)
print(mypy.add(1, 2))

obj = mypy.Object()
obj.setName('paoqi')
obj.setSum(648)
content = 'Name: %s, Sum: %d'%(obj.getName(), obj.getSum())
print(content)
print(obj.FLAG)
print(mypy.DEEP_DARK_FANTASY)
