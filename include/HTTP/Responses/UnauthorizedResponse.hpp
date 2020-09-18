#pragma once

#include "HTTP/Responses/HTTPResponse.hpp"

namespace HTTP
{
    namespace Responses
    {
        class UnauthorizedResponse
            : public HTTPResponse
        {
        public:
            UnauthorizedResponse(boost::asio::ip::tcp::socket&& socket)
                : HTTPResponse{std::move(socket), Status::bad_request, "User is not authorized"}
            {}

            ~UnauthorizedResponse() = default;
            
        };
    };
};
