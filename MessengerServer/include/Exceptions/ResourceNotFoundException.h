#pragma once

#include "Exceptions/RequestException.h"

namespace Exceptions
{
    class ResourceNotFoundException :
        public RequestException
    {
    public:
        ResourceNotFoundException() :
            RequestException{"Resource not found", Status::not_found}
        {}

        virtual ~ResourceNotFoundException() = default;
    private:
        /* data */
    };
    
} // namespace Exceptions


