#include <cerrno>
#include <cstring>
#include <iostream>

#include <ucontext.h>

using std::cout;
using std::endl;

using fn_ctx_t = void(*)();

static ucontext_t ctx1, ctx2, mainCtx;
static const std::size_t StackSize = 4096;

void fn1(int *iptr) {
    cout << *iptr << endl;
    cout << "fn1: started\n";
    if (swapcontext(&ctx1, &ctx2) == -1) {
        cout << std::strerror(errno) << endl;
    }
    cout << "fn1: returning\n";
}

void fn2() {
    cout << "fn2: started\n";
    if (swapcontext(&ctx2, &ctx1) == -1) {
        cout << std::strerror(errno) << endl;
    }
    cout << "fn2: returning\n";
}

int main()
{
    char stk1[StackSize];
    char stk2[StackSize];

    if (getcontext(&ctx1) == -1) {
        cout << std::strerror(errno) << endl;
    }
    ctx1.uc_stack.ss_sp = stk1;
    ctx1.uc_stack.ss_size = sizeof(stk1);
    ctx1.uc_link = &mainCtx;
    int n = 2;
    makecontext(&ctx1, reinterpret_cast<fn_ctx_t>(fn1), 1, &n);

    if (getcontext(&ctx2) == -1) {
        cout << std::strerror(errno) << endl;
    }
    ctx2.uc_stack.ss_sp = stk2;
    ctx2.uc_stack.ss_size = sizeof(stk2);
    ctx2.uc_link = &ctx1;
    makecontext(&ctx2, fn2, 0);

    cout << "main: swapcontext(&mainCtx, &ctx2)\n";
    if (swapcontext(&mainCtx, &ctx2) == -1) {
        cout << std::strerror(errno) << endl;
    }
    cout << "main: exiting\n";

    return 0;
}
