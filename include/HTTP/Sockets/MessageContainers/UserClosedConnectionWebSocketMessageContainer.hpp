#pragma once

#include "HTTP/Sockets/MessageContainers/AWebSocketMessageContainer.hpp"

namespace HTTP
{
    namespace Sockets
    {
        namespace MessageContainers
        {
            class UserClosedConnectionWebSocketMessageContainer
                : public AWebSocketMessageContainer
            {
            public:
                UserClosedConnectionWebSocketMessageContainer()
                    : AWebSocketMessageContainer{"c"}
                {}
                
                ~UserClosedConnectionWebSocketMessageContainer() = default;
            
            };
        }
    }
}
