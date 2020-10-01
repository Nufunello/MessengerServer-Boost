#pragma once

#include "HTTP/Sockets/AWebSocketMessage.hpp"

namespace HTTP
{
    namespace Sockets
    {
        class IdentifiyingWebSocketMessage
            : public AWebSocketMessage
        {
        public:
            IdentifiyingWebSocketMessage(const std::string& websocketToken, const std::string& username)
                : AWebSocketMessage{'i' + websocketToken + username}
            {
                _isEmpty = false;
            }
            
            ~IdentifiyingWebSocketMessage() = default;
        
        };
    }
}
