#pragma once

#include "Handlers/RequestTypes.h"

#include <stdexcept>

namespace Exceptions
{
    using Status = MyRequestHandlers::Status;

    class RequestException :
        public std::logic_error
    {
    public:
        RequestException(std::string message, const Status code) :
            std::logic_error{std::move(message)},
            Code{code}
        {}

        virtual ~RequestException() = default;

    public:
        const Status Code;

    };
    
}

