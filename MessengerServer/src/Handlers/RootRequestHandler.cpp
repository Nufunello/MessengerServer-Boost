#include "Handlers/RootRequestHandler.h"

#include "Handlers/LoginRequestHandler.h"
#include "Handlers/ChatRequestHandler.h"

using namespace MyRequestHandlers;

namespace {
    AMappedRequestHandler::HandlersMap getMap(Data::AuthorizedUsersMap& usersMap)
    {
        AMappedRequestHandler::HandlersMap map;
        map.emplace("login", std::make_unique<LoginRequestHandler>(usersMap));
        map.emplace("chat", std::make_unique<ChatRequestHandler>(usersMap));
        return map;
    }
}

RootRequestHandler::RootRequestHandler(Data::AuthorizedUsersMap& usersMap) :
    ARecursiveLoadableHandler{usersMap, getMap(usersMap), "/webpages/"}
{}
