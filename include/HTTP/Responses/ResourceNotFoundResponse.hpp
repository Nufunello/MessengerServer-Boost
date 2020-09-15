#pragma once

#include "HTTP/Responses/HTTPResponse.hpp"

#include <boost/beast/http/empty_body.hpp>

namespace HTTP
{
    namespace Responses
    {
        class ResourceNotFoundResponse
            : public HTTPResponse
        {
        public:
            ResourceNotFoundResponse(boost::asio::ip::tcp::socket&& socket)
                : HTTPResponse{std::move(socket), boost::beast::http::status::not_found, "Resource not found"}
            {}

            ~ResourceNotFoundResponse() = default;
            
        };
    };
};
