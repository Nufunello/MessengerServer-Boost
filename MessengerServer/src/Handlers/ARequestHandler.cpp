#include "Handlers/ARequestHandler.h"

using namespace MyRequestHandlers;

Response ARequestHandler::processRequest(boost::string_view currentTarget, const Message& message)
{
    switch (message.method())
    {
        case Method::get:
            return this->doGet(std::move(currentTarget), message);

        case Method::post:
            return this->doPost(std::move(currentTarget), message);

        case Method::delete_:
            return this->doDelete(std::move(currentTarget), message);

        default:
            throw Exceptions::NotImplementedMethodException();
    }
}