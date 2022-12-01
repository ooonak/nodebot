#include "nb/NodeHandle.hpp"

nb::NodeHandle::NodeHandle(uint64_t id, const std::string &jsonStr)
    : mId{id}, mJsonStr(jsonStr)
{
}

void nb::NodeHandle::msg(std::string) {}

std::string nb::NodeHandle::toString() const
{
  return std::string("NodeHandle ") + std::to_string(mId) + " " + mJsonStr;
}
