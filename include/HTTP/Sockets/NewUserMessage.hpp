#pragma once

#include "HTTP/Sockets/AWebSocketMessage.hpp"

namespace HTTP
{
    namespace Sockets
    {
        class NewUserMessage
            : public AWebSocketMessage
        {
        public:
            NewUserMessage(const std::string& token, const std::string& name)
                : AWebSocketMessage{'a' + token + name}
            {
                _isEmpty = false;
            }
            
            ~NewUserMessage() = default;
        
        };
    }
}
