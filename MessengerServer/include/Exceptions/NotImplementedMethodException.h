#pragma once

#include "Exceptions/RequestException.h"

namespace Exceptions
{
    class NotImplementedMethodException :
        public RequestException
    {
    public:
        NotImplementedMethodException() :
            RequestException{"Method not implemented", Status::not_implemented}
        {}

        virtual ~NotImplementedMethodException() = default;
    };
}
