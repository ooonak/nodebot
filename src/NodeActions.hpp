#pragma once

#include <string>

namespace ok {

class NodeActions {
public:
    
    virtual ~NodeActions() = default;

    virtual bool connect(std::string uuid) = 0;

    virtual bool disconnect(std::string uuid) = 0;
    
    virtual bool sendMsg(std::string msg) = 0;
    
    virtual bool registerCommand(std::string uuid) = 0;
};

} // namespace ok