#include <iostream>

#include "Users/Data/UsersData.hpp"

#include "HTTP/Responses/BadRequestResponse.hpp"
#include "HTTP/Responses/InternalServerErrorResponse.hpp"

#include "Factories/RootFactory.hpp"
#include "Factories/LoginFactory.hpp"

#include <boost/asio/io_service.hpp>

namespace {
    constexpr HTTP::Requests::Field TOKEN_FIELD = HTTP::Requests::Field::cookie;
}

const Users::AccessRights::Methods Users::AccessRights::AccessRightsNode::sEmptyMethods = Users::AccessRights::Methods{};

template <typename Response>
void sendResponse(HTTP::Requests::Request&& request)
{
    Response response{std::move(request)};
    response.send();
}

void proccessRequest(HTTP::Requests::Request&& request, Factories::RootFactory& rootFactory, Users::Data::UsersData& usersData)
{
    try
    {
        const HTTP::Requests::Message& message = request.message();
        const boost::string_view target = message.target();
         
        URI::URIIterator iterator{target};

        Handlers::AHandler::Ptr handler = rootFactory.getHandler(iterator);
        const URI::Segment fileSegment  = iterator.next();

        if (handler && !iterator.hasNext())
        {
            const auto itToken = message.find(TOKEN_FIELD);
            const Users::AccessRights::RootAccessRightsNode& accessRights = itToken == std::end(message) 
                                                                    ? usersData.getUnauthorizedAllowedMethods() 
                                                                    : usersData.getUsersAllowedMethods(itToken->value());
                                  
            const Users::AccessRights::Methods& allowedMethods = accessRights.getAllowedMethods(target); 

            HTTP::Responses::IResponse::Ptr pResponse = handler->doRequest(std::move(request), fileSegment, allowedMethods);
            pResponse->send();
        }
        else
        {
            sendResponse<HTTP::Responses::ResourceNotFoundResponse>(std::move(request));
        }
    }
    catch(const std::exception& e)
    {   
        sendResponse<HTTP::Responses::InternalServerErrorResponse>(std::move(request));
    }
}

void acceptRequest(boost::asio::io_service& service, boost::asio::ip::tcp::acceptor& acceptor, Factories::RootFactory& rootFactory, Users::Data::UsersData& usersData)
{
    boost::asio::ip::tcp::socket socket{service};
    acceptor.accept(socket);
    HTTP::Requests::Request request{std::move(socket)};

    if (!request.isValid())
    {
        std::string error = request.error();
        sendResponse<HTTP::Responses::BadRequestResponse>(std::move(request));
    }
    else
    {
        proccessRequest(std::move(request), rootFactory, usersData);
    }
}

int main(int argc, char** argv)
{
    try
	{
		const std::string ip = argv[1];
		const size_t port = atoi(argv[2]);
		boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(ip), port);

		std::cout << "Server is running on " << ip << ":" << port << std::endl;

		boost::asio::io_service service;

        Users::Data::UsersData usersData;
        Factories::RootFactory rootFactory(usersData);

		boost::asio::ip::tcp::acceptor acceptor(service, endpoint);
        
        while (true)
        {
            acceptRequest(service, acceptor, rootFactory, usersData);
        }
        
	}
	catch(const std::exception& e)
	{
		std::cerr << "Server start failed due to " << e.what() << '\n';
	}
}