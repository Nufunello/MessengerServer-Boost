#pragma once

#include "Handlers/ARequestHandler.h"
#include "Handlers/ResourceFileMap.h"

namespace MyRequestHandlers
{
    class ChatRequestHandler :
        public ARequestHandler,
        public ResourceFileMap
    {
    public:
        ChatRequestHandler(const Data::AuthorizedUsersMap& authorizedUsers) : 
            ARequestHandler{authorizedUsers},
            ResourceFileMap{"/webpages/chat"}
        {}

        Response doGet(boost::string_view currentTarget, const Message& message) override
        {
            if (hasRights(message))
                return ResourceFileMap::getResponseWithPage(currentTarget);
            else
                throw Exceptions::NotAuthorizedException();
        }

        virtual ~ChatRequestHandler() = default;

    private:
    };
} // namespace MyRequestHandlers
