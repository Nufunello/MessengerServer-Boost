#pragma once

#include "Data/AuthorizedUsersMap.h"
#include "Handlers/ARecursiveLoadableHandler.h"

namespace MyRequestHandlers
{
    class RootRequestHandler :
        public ARecursiveLoadableHandler
    {
    public:
        RootRequestHandler(Data::AuthorizedUsersMap& usersMap);
        virtual ~RootRequestHandler() = default;
    };    
}
