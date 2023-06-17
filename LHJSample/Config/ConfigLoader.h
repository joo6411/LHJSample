#pragma once

#include <string>

namespace tinyxml2
{
    class XMLElement;
}

struct ServerConnectionInfo
{
public:
    std::string IP;
    unsigned short Port;
};

class ConfigLoader
{
public:
    void LoadNetworkConfig(const char* configPath);
    static ConfigLoader& GetInstance()
    {
        static ConfigLoader config;
        return config;
    }

    ServerConnectionInfo mChatServerInfo;
    ServerConnectionInfo mRedisServerInfo;

private:
    bool IsElementName(tinyxml2::XMLElement* ele, const char* compareName);
    const char* GetText(tinyxml2::XMLElement* ele, bool required);

#define gConfig ConfigLoader::GetInstance()
};
