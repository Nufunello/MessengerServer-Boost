#pragma once

#include "HTTP/Responses/HTTPResponse.hpp"

namespace HTTP
{
    namespace Responses
    {
        class InternalServerErrorResponse
            : public HTTPResponse
        {
        public:
            InternalServerErrorResponse(boost::asio::ip::tcp::socket&& socket)
                : HTTPResponse{std::move(socket), Status::internal_server_error, "Internal server error"}
            {}

            ~InternalServerErrorResponse() = default;
            
        };
    }
}
