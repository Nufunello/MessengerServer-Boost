#pragma once

#include "HTTP/Sockets/AWebSocketMessage.hpp"

namespace HTTP
{
    namespace Sockets
    {
        namespace MessageContainers
        {
            class AWebSocketMessageContainer
                : public AWebSocketMessage
            {
            protected:
                AWebSocketMessageContainer(std::string messageType)
                    : AWebSocketMessage{std::move(messageType)}
                {}

            public:
                virtual ~AWebSocketMessageContainer() = default;

                void append(const std::string& param)
                {
                    _message += param;
                    _isEmpty = false;
                }

            };
        }
    }
}
