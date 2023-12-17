#include "IngressMessage.hpp"

#include <sstream>

ok::ActionT ok::fromString(const std::string &str)
{
  if (str == "connect")
    return ActionT::Connect;
  else if (str == "disconnect")
    return ActionT::Disconnect;
  else if (str == "message")
    return ActionT::Message;
  else if (str == "registercommand")
    return ActionT::RegisterCommand;
  else if (str == "unregistercommand")
    return ActionT::UnregisterCommand;
  else
    return ActionT::None;
}

std::string ok::toString(ActionT action)
{
  switch (action)
  {
    case ActionT::Connect:
      return "connect";
    case ActionT::Disconnect:
      return "disconnect";
    case ActionT::Message:
      return "message";
    case ActionT::RegisterCommand:
      return "register_command";
    case ActionT::UnregisterCommand:
      return "unregister_command";
    default:
      return "none";
  }

  return "none";
}

std::string ok::toString(ok::IngressMessage msg)
{
  std::ostringstream oss;
  oss << "id: " << msg.id << ", action: " << toString(msg.action) << ", group: " << msg.group
      << ", payload: " << msg.jsonPayload;
  return oss.str();
}
