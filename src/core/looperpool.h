#ifndef PQNET_CORE_LOOPER_POOL_H
#define PQNET_CORE_LOOPER_POOL_H

#include <memory>
#include <vector>

#include "looper.h"

namespace pqnet
{

class LooperPool
{
public:
    LooperPool();
    ~LooperPool();
private:
    bool running;
    std::size_t tn;
    std::vector<std::unique_ptr<Looper>> pool;
};

} // namespace pqnet

#endif // PQNET_CORE_LOOPER_POOL_H
