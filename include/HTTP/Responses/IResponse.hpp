#pragma once

#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/http/string_body.hpp>

namespace HTTP
{
    namespace Responses
    {
        using Response = boost::beast::http::response<boost::beast::http::string_body>;
        using Status   = boost::beast::http::status;

        class IResponse
        {
        public:
            using Ptr = std::unique_ptr<IResponse>;

        protected:
            IResponse() = default;

        public:
            virtual ~IResponse() = default;

        public:
            virtual void send() = 0;

        };
    }
}
