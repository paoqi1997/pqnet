#include <cerrno>
#include <cstring>
#include <iostream>

#include <ucontext.h>

#define STACKSIZE 4096

using std::cout;
using std::endl;

/**
 * typedef struct ucontext_t {
 *     struct ucontext_t *uc_link;
 *     sigset_t          uc_sigmask;
 *     stack_t           uc_stack;
 *     mcontext_t        uc_mcontext;
 *     ...
 * } ucontext_t;
 */
ucontext_t ctx1, ctx2, mainCtx;

void fn1() {
    cout << "fn1: swapcontext(&ctx1, &ctx2)\n";
    if (swapcontext(&ctx1, &ctx2) == -1) {
        cout << std::strerror(errno) << endl;
    }
    cout << "fn1: returning\n";
}

void fn2() {
    cout << "fn2: swapcontext(&ctx2, &ctx1)\n";
    if (swapcontext(&ctx2, &ctx1) == -1) {
        cout << std::strerror(errno) << endl;
    }
    cout << "fn2: returning\n";
}

int main()
{
    char stack1[STACKSIZE];
    char stack2[STACKSIZE];

    // Ctx1
    if (getcontext(&ctx1) == -1) {
        cout << std::strerror(errno) << endl;
    }
    ctx1.uc_stack.ss_sp = stack1;
    ctx1.uc_stack.ss_size = sizeof(stack1);
    ctx1.uc_link = &mainCtx;
    makecontext(&ctx1, fn1, 0);

    // Ctx2
    if (getcontext(&ctx2) == -1) {
        cout << std::strerror(errno) << endl;
    }
    ctx2.uc_stack.ss_sp = stack2;
    ctx2.uc_stack.ss_size = sizeof(stack2);
    ctx2.uc_link = &ctx1;
    makecontext(&ctx2, fn2, 0);

    // mainCtx -> ctx2 -> ctx1 -> ctx2 -> ctx1 -> mainCtx
    cout << "main: swapcontext(&mainCtx, &ctx2)\n";
    if (swapcontext(&mainCtx, &ctx2) == -1) {
        cout << std::strerror(errno) << endl;
    }
    cout << "main: exiting\n";

    return 0;
}
