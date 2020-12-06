#include <cstdio>
#include <functional>

#include <dlfcn.h>

using funcType = double(*)(double);

void error(const char *callname, const char *msg)
{
    std::printf("Failed to call %s(), error: %s\n", callname, msg);
}

int main()
{
    const char *msg = nullptr;

    auto handle = dlopen("libm.so.6", RTLD_LAZY);
    if (handle == nullptr) {
        msg = dlerror();
        if (msg != nullptr) {
            error("dlopen", msg);
        }
    }

    auto address = reinterpret_cast<funcType>(dlsym(handle, "cos"));
    if (address == nullptr) {
        msg = dlerror();
        if (msg != nullptr) {
            error("dlsym", msg);
        }
    }

    std::function<double(double)> fn(address);
    std::printf("cos(2.0)=%f\n", fn(2.0));

    if (dlclose(handle) != 0) {
        msg = dlerror();
        if (msg != nullptr) {
            error("dlclose", msg);
        }
    }

    return 0;
}
