#include <string>
#include <functional>

namespace nb
{

class NodeHandle
{
 public:
  explicit NodeHandle(uint64_t id, const std::string &mJsonStr);

  void msg(std::string);

  using CommandT = std::function<void(std::string)>;

  // TODO void registerCommand(std::string, CommandT);

  std::string toString() const;

 private:
  uint64_t mId{0};
  std::string mJsonStr;
};

}  // namespace nb
