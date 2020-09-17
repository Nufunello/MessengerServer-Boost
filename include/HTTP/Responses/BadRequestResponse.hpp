#pragma once

#include "HTTP/Responses/HTTPResponse.hpp"

namespace HTTP
{
    namespace Responses
    {
        class BadRequestResponse
            : public HTTPResponse
        {
        public:
            BadRequestResponse(boost::asio::ip::tcp::socket&& socket)
                : HTTPResponse{std::move(socket), Status::bad_request, "Bad request"}
            {}

            ~BadRequestResponse() = default;
            
        };
    };
};
