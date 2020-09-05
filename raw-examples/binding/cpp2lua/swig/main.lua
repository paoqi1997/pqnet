-- Lua调用C++

local mod = require('mylua')
print(mod.add(1, 2))

local obj = mod.Object()
obj:setName('paoqi')
obj:setSum(648)
content = string.format('Name: %s, Sum: %d', obj:getName(), obj:getSum())
print(content)
