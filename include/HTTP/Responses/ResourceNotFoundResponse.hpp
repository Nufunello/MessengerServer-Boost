#pragma once

#include "HTTP/Responses/HTTPResponse.hpp"

namespace HTTP
{
    namespace Responses
    {
        class ResourceNotFoundResponse
            : public HTTPResponse
        {
        public:
            ResourceNotFoundResponse(boost::asio::ip::tcp::socket&& socket)
                : HTTPResponse{std::move(socket), Status::not_found, "Resource not found"}
            {}

            ~ResourceNotFoundResponse() = default;
            
        };
    };
};
