#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace ok
{

struct Message
{
  enum class ActionT
  {
    None,
    Connect,
    Disconnect,
    Message,
    RegisterCommand,
    UnregisterCommand,
    Command
  };

  ActionT action;
  std::uint64_t id;
  std::string group;
  std::string jsonPayload;
};

Message::ActionT fromString(const std::string &str);

std::string toString(Message::ActionT action);

std::string toString(const Message &msg);

}  // namespace ok
