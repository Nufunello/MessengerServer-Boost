#pragma once

#include "Exceptions/RequestException.h"

namespace Exceptions
{
    class NotAuthorizedException :
        public RequestException
    {
    public:
        NotAuthorizedException() :
            RequestException{"Not authorized", Status::unauthorized}
        {}
        virtual ~NotAuthorizedException() = default;
    };
} // namespace Exceptions


