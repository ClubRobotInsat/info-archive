#ifndef ROOT_IGUICLIENT_H
#define ROOT_IGUICLIENT_H

#include <string>
#include <vector>

struct IAProcessData {
    std::string executablePath;
    std::vector<std::string> parameters;
};

struct ConnectionData {
    std::string method;
    std::vector<std::string> parameters;
};

class IGuiClient {
public:
    virtual ~IGuiClient() = default;

    virtual void connect(const ConnectionData &connectionData) = 0;

    virtual void createIAProcess(const IAProcessData &iaProcessData, const ConnectionData &connectionData) = 0;
};

#endif //ROOT_IGUICLIENT_H
