#pragma once

#include "Exceptions/RedirectException.h"

namespace Exceptions
{
    class NotAuthorizedException :
        public RedirectException
    {
    public:
        NotAuthorizedException() :
            RedirectException{"Not authorized", "/login", Status::unauthorized}
        {}
        virtual ~NotAuthorizedException() = default;
    };
} // namespace Exceptions


