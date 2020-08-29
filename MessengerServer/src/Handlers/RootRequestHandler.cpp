#include "Handlers/RootRequestHandler.h"

#include "Handlers/LoginRequestHandler.h"

using namespace MyRequestHandlers;

namespace {
    constexpr char LOGIN_RESOURCE[] = "login";

    AMappedRequestHandler::HandlersMap getMap(Data::AuthorizedUsersMap& usersMap)
    {
        AMappedRequestHandler::HandlersMap map;
        map.emplace(LOGIN_RESOURCE, std::make_unique<LoginRequestHandler>(usersMap));
        return map;
    }
}

RootRequestHandler::RootRequestHandler(Data::AuthorizedUsersMap& usersMap) :
    AMappedRequestHandler{ getMap(usersMap) },
    ALoadablePageRequestHandler{"/webpages/"}
{}