#include <iostream>

#include "Users/AccessRights/AccessRightsNode.hpp"

#include "HTTP/Responses/BadRequestResponse.hpp"

#include "Factories/RootFactory.hpp"
#include "Factories/LoginFactory.hpp"

#include <boost/asio/io_service.hpp>

const Users::AccessRights::Methods Users::AccessRights::AccessRightsNode::sEmptyMethods = Users::AccessRights::Methods{};

void proccessRequest(HTTP::Requests::Request&& request, Factories::RootFactory& rootFactory)
{
    if (!request.isValid())
    {
        HTTP::Responses::BadRequestResponse response{std::move(request)};
        response.send();
    }

    URI::URIIterator iterator{request.fields().target()};

    Handlers::AHandler::Ptr handler = rootFactory.getHandler(iterator);
    const URI::Segment fileSegment  = iterator.next();


    if (handler && !iterator.hasNext())
    {
        Users::AccessRights::Methods unauth{HTTP::Requests::Method::get, HTTP::Requests::Method::post};
        HTTP::Responses::IResponse::Ptr pResponse = handler->doRequest(std::move(request), fileSegment, unauth);
        pResponse->send();
    }
    else
    {
        HTTP::Responses::ResourceNotFoundResponse response{std::move(request)};
        response.send();
    }
}

void acceptRequest(boost::asio::io_service& service, boost::asio::ip::tcp::acceptor& acceptor, Factories::RootFactory& rootFactory)
{
	boost::asio::ip::tcp::socket socket(service);
    acceptor.accept(socket);
    proccessRequest(HTTP::Requests::Request{std::move(socket)}, rootFactory);
}

int main(int argc, char** argv)
{
    const Users::AccessRights::Methods methd = rootNode.getAllowedMethods("/");
    try
	{
		const std::string ip = argv[1];
		const size_t port = atoi(argv[2]);
		boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(ip), port);

		std::cout << "Server is running on " << ip << ":" << port << std::endl;

		boost::asio::io_service service;

        AuthorizhedUsers d = 4;
        Factories::RootFactory rootFactory(d);

		boost::asio::ip::tcp::acceptor acceptor(service, endpoint);
        
	}
	catch(const std::exception& e)
	{
		std::cerr << "Server failed due to " << e.what() << '\n';
	}
}