#pragma once

#include "HTTP/Responses/HTTPResponse.hpp"

#include <boost/beast/http/empty_body.hpp>

namespace HTTP
{
    namespace Responses
    {
        class MethodNotAllowedResponse
            : public HTTPResponse
        {
        public:
            MethodNotAllowedResponse(boost::asio::ip::tcp::socket&& socket)
                : HTTPResponse{std::move(socket), boost::beast::http::status::method_not_allowed, "Method not allowed"}
            {}

            ~MethodNotAllowedResponse() = default;
            
        };
    };
};
