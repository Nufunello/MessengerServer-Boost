#pragma once

#include "Exceptions/RedirectException.h"

namespace Exceptions
{
    class AlreadyAuthorizedException :
        public RedirectException
    {
    public:
        AlreadyAuthorizedException() :
            RedirectException{"Already logined", "/chat", Status::found}
        {}
        
        virtual ~AlreadyAuthorizedException() = default;
    };

} // namespace Exceptions

