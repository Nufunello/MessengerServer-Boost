#pragma once

#include "Handlers/AHandler.hpp"
#include "URI/URIIterator.hpp"

namespace Factories
{
    class AHandlerFactory
    {
    public:
        using Ptr = std::unique_ptr<AHandlerFactory>;
        
    protected:
        AHandlerFactory() = default;

    protected:
        inline void setHandler(Handlers::AHandler* handler)
        {
            _handler = handler;
        }

    public:
        virtual Handlers::AHandler* getHandler(URI::URIIterator& iterator) const
        {
            return _handler;
        }

    private:
        Handlers::AHandler* _handler;

    };
};
