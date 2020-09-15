#pragma once

#include "HTTP/Responses/IResponse.hpp"

#include <boost/beast.hpp>

namespace HTTP
{
    namespace Responses
    {
        class WebSocketResponse
            : public IResponse
        {
        public:
            WebSocketResponse() = default;
            ~WebSocketResponse() = default;

        public:
            virtual void send() override
            {
            }

        };
    };
};
