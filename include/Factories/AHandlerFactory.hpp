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
        inline void setHandler(Handlers::AHandler::Ptr handler)
        {
            _handler = handler;
        }

    public:
        virtual Handlers::AHandler::Ptr getHandler(URI::URIIterator& iterator) const
        {
            return _handler;
        }

    private:
        Handlers::AHandler::Ptr _handler;

    };
};
