#pragma once

#include "HTTP/Sockets/MessageContainers/AWebSocketMessageContainer.hpp"

namespace HTTP
{
    namespace Sockets
    {
        namespace MessageContainers
        {
            class UserOnlineWebSocketMessageContainer
                : public AWebSocketMessageContainer
            {
            public:
                UserOnlineWebSocketMessageContainer()
                    : AWebSocketMessageContainer{"o"}
                {}
                
                ~UserOnlineWebSocketMessageContainer() = default;
            
            };
        }
    }
}
