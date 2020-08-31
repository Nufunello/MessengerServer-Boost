#pragma once

#include "Handlers/AMappedRequestHandler.h"
#include "Handlers/ResourceFileMap.h"

namespace MyRequestHandlers
{
    class ARecursiveLoadableHandler :
        public AMappedRequestHandler,
        public ResourceFileMap
    {
    protected:
        ARecursiveLoadableHandler(const Data::AuthorizedUsersMap& authorizedUsers, AMappedRequestHandler::HandlersMap&& map, const std::string& filesDir) :
            AMappedRequestHandler{authorizedUsers, std::move(map)},
            ResourceFileMap{filesDir}
        {}

    public:
        virtual ~ARecursiveLoadableHandler() = default;

    protected:
        Response doGet(boost::string_view currentTarget, const Message& message)
        {
            return ResourceFileMap::getResponseWithPage(currentTarget);
        }

    };
}
