#include "ConfigLoader.h"
#include <format>
#include <utility>
#include <iostream>
#pragma comment(lib, "tinyxml2.lib")
#include <tinyxml2.h>
using namespace tinyxml2;

void ConfigLoader::LoadNetworkConfig(const char* configPath)
{
    auto xmlDoc = std::make_shared<tinyxml2::XMLDocument>();
    xmlDoc->LoadFile(configPath);
    if (xmlDoc->ErrorID() != tinyxml2::XMLError::XML_SUCCESS)
    {
        throw std::format("{} Load fail - {}", configPath, xmlDoc->ErrorIDToName(xmlDoc->ErrorID()));
    }
    auto root = xmlDoc->FirstChildElement("Config");

    tinyxml2::XMLElement* Server = root->FirstChildElement("ChatServer");
    if (!Server)
    {
        throw std::format("NotFound <chatServer> config");
    }

    for (auto child = Server->FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
    {
        if (IsElementName(child, "IP"))
        {
            mChatServerInfo.IP = GetText(child, true);
        }
        else if (IsElementName(child, "Port"))
        {
            mChatServerInfo.Port = (unsigned short)child->IntText();
        }
    }

    Server = root->FirstChildElement("RedisServer");
    if (!Server)
    {
        throw std::format("NotFound <RedisServer> config");
    }

    for (auto child = Server->FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
    {
        if (IsElementName(child, "IP"))
        {
            mRedisServerInfo.IP = GetText(child, true);
        }
        else if (IsElementName(child, "Port"))
        {
            mRedisServerInfo.Port = (unsigned short)child->IntText();
        }
    }

    Server = root->FirstChildElement("AccountDB");
    if (!Server)
    {
        throw std::format("NotFound <AccountDB> config");
    }

    for (auto child = Server->FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
    {
        if (IsElementName(child, "IP"))
        {
            mAccountDBInfo.IP = GetText(child, true);
        }
        else if (IsElementName(child, "Port"))
        {
            mAccountDBInfo.Port = (unsigned short)child->IntText();
        }
        else if (IsElementName(child, "ID"))
        {
            mAccountDBInfo.ID = GetText(child, true);
        }
        else if (IsElementName(child, "Password"))
        {
            mAccountDBInfo.Password = GetText(child, true);
        }
    }
}

bool ConfigLoader::IsElementName(tinyxml2::XMLElement* ele, const char* compareName)
{
    return (ele && (strcmp(ele->Name(), compareName) == 0));
}

const char* ConfigLoader::GetText(tinyxml2::XMLElement* ele, bool required)
{
    if (ele && ele->GetText())
    {
        return ele->GetText();
    }
    if (required)
    {
        throw std::format("GetTextFail");
    }
    return "";
}