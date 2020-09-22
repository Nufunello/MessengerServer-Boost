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
        AHandlerFactory()
            : _handler{nullptr}
        {}

        AHandlerFactory(const AHandlerFactory&) = default;

    public:
        virtual ~AHandlerFactory() = default;

    protected:
        inline void setHandler(Handlers::AHandler* handler)
        {
            _handler = handler;
        }

    public:
        virtual Handlers::AHandler* getHandler(URI::URIIterator& iterator) const
        {
            boost::ignore_unused(iterator);
            return _handler;
        }

    private:
        Handlers::AHandler* _handler;

    };
}
