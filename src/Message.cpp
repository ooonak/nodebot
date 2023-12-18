#include "Message.hpp"

#include <sstream>

ok::Message::ActionT ok::fromString(const std::string &str)
{
  if (str == "connect") return Message::ActionT::Connect;
  if (str == "disconnect") return Message::ActionT::Disconnect;
  if (str == "message") return Message::ActionT::Message;
  if (str == "registercommand") return Message::ActionT::RegisterCommand;
  if (str == "unregistercommand") return Message::ActionT::UnregisterCommand;
  if (str == "command") return Message::ActionT::Command;

  return Message::ActionT::None;
}

std::string ok::toString(ok::Message::ActionT action)
{
  switch (action)
  {
    case Message::ActionT::Connect:
      return "connect";
    case Message::ActionT::Disconnect:
      return "disconnect";
    case Message::ActionT::Message:
      return "message";
    case Message::ActionT::RegisterCommand:
      return "registercommand";
    case Message::ActionT::UnregisterCommand:
      return "unregistercommand";
    case Message::ActionT::Command:
      return "command";
    default:
      return "none";
  }

  return "none";
}

std::string ok::toString(const ok::Message &msg)
{
  std::ostringstream oss;
  oss << "id: " << msg.id << ", action: " << toString(msg.action) << ", group: " << msg.group
      << ", payload: " << msg.jsonPayload;
  return oss.str();
}
