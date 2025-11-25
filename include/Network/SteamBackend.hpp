#ifndef STEAM_BACKEND_HPP
#define STEAM_BACKEND_HPP

#include <steam/steam_api.h>
#include <steam/isteamnetworkingsockets.h>
#include <steam/steamnetworkingtypes.h>

class SteamBackend
{
public:
    static bool init();

    static bool destroy();

    static bool isInitialized();
private:
    static inline SteamBackend* m_steamBackend;

    static void steamDebugOutput(ESteamNetworkingSocketsDebugOutputType type, const char* msg);

    bool m_isInitialized{false};
};

#endif //STEAM_BACKEND_HPP