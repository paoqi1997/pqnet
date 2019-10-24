-- Lua调用C++

local mod = require('mylua')
local obj = mod.Object()
obj:setName('paoqi')
obj:setSum(648)
content = string.format('Name: %s, Sum: %d', obj:getName(), obj:getSum())
print(content)
