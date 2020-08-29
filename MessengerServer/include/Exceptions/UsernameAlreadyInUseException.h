#pragma once

#include "Exceptions/RequestException.h"

namespace Exceptions
{
    class UsernameAlreadyInUseException :
        public RequestException
    {
    public:
        UsernameAlreadyInUseException() :
            RequestException{"Username is already in use", Status::bad_request}
        {}
        
        ~UsernameAlreadyInUseException() = default;
    };
}
