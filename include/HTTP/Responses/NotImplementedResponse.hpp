#pragma once

#include "HTTP/Responses/HTTPResponse.hpp"

namespace HTTP
{
    namespace Responses
    {
        class NotImplementedResponse
            : public HTTPResponse
        {
        public:
            NotImplementedResponse(boost::asio::ip::tcp::socket&& socket)
                : HTTPResponse{std::move(socket), Status::not_implemented, "Not implemented"}
            {}

            ~NotImplementedResponse() = default;
            
        };
    }
}
