#pragma once

#include <string>

namespace HTTP
{
    namespace Sockets
    {
        class AWebSocketMessage
        {
        protected:
            AWebSocketMessage(std::string message)
                : _message{std::move(message)}
                , _isEmpty{true}
            {
            }

        public:
            virtual ~AWebSocketMessage() = default;

            bool isEmpty() const
            {
                return _isEmpty;
            }

            const std::string& asString() const
            {
                return _message;
            }

        protected:
            std::string _message;
            bool _isEmpty;

        };
    }
}
