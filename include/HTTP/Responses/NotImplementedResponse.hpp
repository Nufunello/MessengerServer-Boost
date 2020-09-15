#pragma once

#include "HTTP/Responses/HTTPResponse.hpp"

#include <boost/beast/http/empty_body.hpp>

namespace HTTP
{
    namespace Responses
    {
        class NotImplementedResponse
            : public HTTPResponse
        {
        public:
            NotImplementedResponse(boost::asio::ip::tcp::socket&& socket)
                : HTTPResponse{std::move(socket), boost::beast::http::status::not_implemented, "Not implemented"}
            {}

            ~NotImplementedResponse() = default;
            
        };
    };
};
