// more bindings: http://lua-users.org/wiki/BindingCodeToLua
%module mylua

%include <std_string.i>

%{
#include "mylua.h"
%}
// Now list ISO C/C++ declarations
%include "mylua.h"
