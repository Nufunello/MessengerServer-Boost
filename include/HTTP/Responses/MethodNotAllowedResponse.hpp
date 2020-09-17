#pragma once

#include "HTTP/Responses/HTTPResponse.hpp"

namespace HTTP
{
    namespace Responses
    {
        class MethodNotAllowedResponse
            : public HTTPResponse
        {
        public:
            MethodNotAllowedResponse(boost::asio::ip::tcp::socket&& socket)
                : HTTPResponse{std::move(socket), Status::method_not_allowed, "Method not allowed"}
            {}

            ~MethodNotAllowedResponse() = default;
            
        };
    };
};
