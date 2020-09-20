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
            WebSocketResponse(boost::beast::websocket::stream<boost::beast::tcp_stream>& websocket)
                : _websocket{websocket}
            {}
            
            ~WebSocketResponse() = default;

        public:
            virtual void send() override
            {
                _websocket.accept();
            }

        private:
            boost::beast::websocket::stream<boost::beast::tcp_stream>& _websocket;

        };
    };
};
