#pragma once

#include <boost/asio/ip/tcp.hpp>

namespace HTTP
{
    namespace Responses
    {
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
    };
};
