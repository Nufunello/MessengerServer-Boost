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
            IdentifiyingWebSocketMessage(const std::string& websocketToken, const boost::string_view name)
                : AWebSocketMessage{'i' + websocketToken + name.to_string()}
            {
                _isEmpty = false;
            }
            
            ~IdentifiyingWebSocketMessage() = default;
        
        };
    }
}
