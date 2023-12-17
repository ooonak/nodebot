#pragma once

#include <string>
#include <cstdint>
#include <vector>
#include "IngressMessage.hpp"

namespace ok {

class IngressQueue {
public:

    virtual ~IngressQueue() = default;

    virtual bool push(IngressMessage msg) = 0;
};

} // namespace ok
