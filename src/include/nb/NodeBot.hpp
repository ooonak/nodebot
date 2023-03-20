#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "DataTypes.hpp"
#include "spdlog/spdlog.h"

namespace nb
{

class NodeBotImpl;

class NodeBot
{
 public:
  /**
   * @brief NodeBot
   * @param filename: Configuration file to use.
   * @param logger: Logger to use.
   */
  explicit NodeBot(const std::string &filename, const std::shared_ptr<spdlog::logger> &logger);

  ~NodeBot();

  /**
   * @brief start: Start the event loop, blocking. Spawn a seperate thread for this.
   */
  void start();

  /**
   * @brief stop: Stop event loop.
   */
  void stop();

  /**
   * @brief getHandle
   * @param info: Information for node we are about to add.
   * @return Unique reference for node.
   */
  uint64_t getHandle(const NodeInfo &info);

  /**
   * @brief updateNodeHandle
   * @param id: Unique reference for node.
   * @param info: Update information.
   * @return false on any error.
   */
  bool updateNodeHandle(uint64_t id, const NodeInfo &info);

  /**
   * @brief registerCommand
   * @param id: Unique reference for node.
   * @param name: Name of command.
   * @param cb: Function to call on command.
   * @return false on any error.
   */
  bool registerCommand(uint64_t id, std::string name, CmdCbT cb);

  /**
   * @brief sendMessage
   * @param id: Unique reference for node.
   * @param message: Message to send.
   * @return false on any error.
   */
  bool sendMessage(uint64_t id, std::string message);

 private:
  std::unique_ptr<NodeBotImpl> mImpl;
};

}  // namespace nb
