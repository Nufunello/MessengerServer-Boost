#pragma once

#include "Factories/AHandlerFactory.hpp"

#include "Handlers/ActiveUsersHandler.hpp"

namespace Factories
{
    class ActiveUsersFactory
        : public AHandlerFactory
    {
    public:
        ActiveUsersFactory(Users::Data::UsersData& usersData)
            : _activeUsersHandler{usersData}
        {
            AHandlerFactory::setHandler(&_activeUsersHandler);
        }

        ~ActiveUsersFactory() = default;

    public:
        Handlers::AHandler* getHandler(URI::URIIterator& iterator) const override
        {
            return iterator.isLastSegment() ? AHandlerFactory::getHandler(iterator) : nullptr;
        }

    private:
        Handlers::ActiveUsersHandlers _activeUsersHandler;
    
    };
};
