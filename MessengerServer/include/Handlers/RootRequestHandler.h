#pragma once

#include "Data/AuthorizedUsersMap.h"
#include "Handlers/AMappedRequestHandler.h"
#include "Handlers/ALoadablePageRequestHandler.h"

namespace MyRequestHandlers
{
    class RootRequestHandler :
        virtual public AMappedRequestHandler,
        virtual public ALoadablePageRequestHandler
    {
    public:
        RootRequestHandler(Data::AuthorizedUsersMap& usersMap);
        virtual ~RootRequestHandler() = default;

    protected:
        virtual Response doGet(boost::string_view currentTarget, const Message& message)
        {
            return ALoadablePageRequestHandler::doGet(std::move(currentTarget), message);
        }

    public:
        Response processRequest(boost::string_view currentTarget, const Message& message) override
        {
            return AMappedRequestHandler::processRequest(std::move(currentTarget), message);
        }

    };    
}
