#include "Network/SteamBackend.hpp"
#include "Logger.hpp"
#include <iostream>

bool SteamBackend::init()
{
    if(m_steamBackend)
        return false;

    if (!SteamAPI_Init())
    {
        LOG_ERROR("SteamAPI_Init failed. Make sure Steam is running and your appID is valid");
        return false;
    }

    SteamNetworkingUtils()->InitRelayNetworkAccess();

    m_steamBackend = new SteamBackend();

    SteamNetworkingUtils()->SetDebugOutputFunction(k_ESteamNetworkingSocketsDebugOutputType_Msg, &SteamBackend::steamDebugOutput);

    m_steamBackend->m_isInitialized = true;

    return true;
}

bool SteamBackend::isInitialized()
{
    if(!m_steamBackend)
        return false;
    
    return m_steamBackend->m_isInitialized;
}

void SteamBackend::steamDebugOutput(ESteamNetworkingSocketsDebugOutputType type, const char* msg)
{
    std::string steamMsg = "[SteamNet] " +  std::string(msg);
    LOG_DEBUG(steamMsg);
}

bool SteamBackend::destroy()
{
    if(!m_steamBackend)
        return false;
    
    delete m_steamBackend;
    m_steamBackend = nullptr;

    SteamAPI_Shutdown();

    return true;
}