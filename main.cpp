#include <iostream>

#include "Factories/RootFactory.hpp"
#include "Factories/LoginFactory.hpp"

#include <boost/asio/io_service.hpp>


int main()
{
    AuthorizhedUsers d = 4;

    Factories::RootFactory rootFactory(d);

    URI::URIIterator iterator("/login/index.html");

    boost::asio::io_service service;
    boost::asio::ip::tcp::socket socket{service};
    HTTP::Requests::Request request(std::move(socket));

    Handlers::AHandler::Ptr handler = rootFactory.getHandler(iterator);
    const URI::Segment fileSegment = iterator.next();

    if (handler && !iterator.hasNext())
    {
        Users::AccessRights::Methods unauth{HTTP::Requests::Method::post, HTTP::Requests::Method::post};
        HTTP::Responses::IResponse::Ptr pResponse = handler->doRequest(std::move(request), fileSegment, unauth);
        pResponse->send();
    }
    else
    {
        HTTP::Responses::ResourceNotFoundResponse response{std::move(request)};
        response.send();
    }
    
}