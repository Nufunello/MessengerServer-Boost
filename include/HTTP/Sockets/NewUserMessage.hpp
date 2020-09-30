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

            NewUserMessage(const boost::string_view token, const boost::string_view name)
                : AWebSocketMessage{'a' + token.to_string() + name.to_string()}
            {
                _isEmpty = false;
            }
            
            ~NewUserMessage() = default;
        
        };
    }
}
