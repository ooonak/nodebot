#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace ok
{

enum class ActionT
{
  None,
  Connect,
  Disconnect,
  Message,
  RegisterCommand,
  UnregisterCommand
};

struct IngressMessage
{
  ActionT action;
  std::uint64_t id;
  std::string group;
  std::string jsonPayload;
};

ActionT fromString(const std::string &str);

std::string toString(ActionT action);

std::string toString(IngressMessage msg);

}  // namespace ok
