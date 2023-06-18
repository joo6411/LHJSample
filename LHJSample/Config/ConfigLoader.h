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

struct DBConnectionInfo
{
public:
    std::string IP;
    unsigned short Port;
    std::string ID;
    std::string Password;
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

    const ServerConnectionInfo& GetChatServerInfo() { return mChatServerInfo; }
    const ServerConnectionInfo& GetRedisServerInfo() { return mRedisServerInfo; }
    const DBConnectionInfo& GetAccountDBInfo() { return mAccountDBInfo; }

private:
    bool IsElementName(tinyxml2::XMLElement* ele, const char* compareName);
    const char* GetText(tinyxml2::XMLElement* ele, bool required);

    ServerConnectionInfo mChatServerInfo;
    ServerConnectionInfo mRedisServerInfo;
    DBConnectionInfo mAccountDBInfo;

#define gConfig ConfigLoader::GetInstance()
};
