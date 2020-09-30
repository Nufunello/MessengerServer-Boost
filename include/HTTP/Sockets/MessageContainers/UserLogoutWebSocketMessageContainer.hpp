#pragma once

#include "HTTP/Sockets/MessageContainers/AWebSocketMessageContainer.hpp"

namespace HTTP
{
    namespace Sockets
    {
        namespace MessageContainers
        {
            class UserLogoutWebSocketMessageContainer
                : public AWebSocketMessageContainer
            {
            public:
                UserLogoutWebSocketMessageContainer()
                    : AWebSocketMessageContainer{"l"}
                {}
                
                ~UserLogoutWebSocketMessageContainer() = default;
            
            };
        }
    }
}
