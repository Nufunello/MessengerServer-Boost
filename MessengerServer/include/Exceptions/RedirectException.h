#pragma once

#include "Exceptions/RequestException.h"

namespace Exceptions
{
    class RedirectException :
        public RequestException
    {
    public:
        RedirectException(const std::string& message, const std::string& where, const Status code) :
            RequestException{message, code},
            Where{where}
        {}

        ~RedirectException() = default;
        
    public:
        const std::string Where;

    };
}
