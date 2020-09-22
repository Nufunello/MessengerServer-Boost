#include <iostream>

#include "Users/Data/UsersData.hpp"

#include "HTTP/Responses/BadRequestResponse.hpp"
#include "HTTP/Responses/InternalServerErrorResponse.hpp"

#include "Factories/RootFactory.hpp"
#include "Factories/LoginFactory.hpp"

#include <boost/asio/io_service.hpp>

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

        Handlers::AHandler* handler = rootFactory.getHandler(iterator);
        const URI::Segment fileSegment  = iterator.next();

        if (handler && !iterator.hasNext())
        {
            auto token = request.getToken();
            const Users::AccessRights::RootAccessRightsNode& accessRights = token.has_value()
                                                                    ? usersData.getUsersAllowedMethods(token.value())
                                                                    : usersData.getUnauthorizedAllowedMethods();

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

void acceptRequest(boost::asio::io_service& service, boost::asio::ip::tcp::acceptor& acceptor, boost::asio::ip::tcp::socket&& socket, Factories::RootFactory& rootFactory, Users::Data::UsersData& usersData)
{
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

    acceptor.async_accept(socket, [&](const boost::system::error_code& error){
        if (!error)
        {
            acceptRequest(service, acceptor, std::move(socket), rootFactory, usersData);
        }
    });
}

int main(int argc, char** argv)
{
    try
	{
        if (argc < 3)
        {
            throw std::runtime_error("Invalid arguments count");
        }
		const std::string ip = argv[1];
		const short unsigned int port = static_cast<short unsigned int>(atoi(argv[2]));
		boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(ip), port);

		std::cout << "Server is running on " << ip << ":" << port << std::endl;

		boost::asio::io_service service;

        Users::Data::UsersData usersData;
        Factories::RootFactory rootFactory(usersData);

		boost::asio::ip::tcp::acceptor acceptor(service, endpoint);
        boost::asio::ip::tcp::socket socket{service};
        acceptor.async_accept(socket, [&](const boost::system::error_code& error){
            if (!error)
            {
                acceptRequest(service, acceptor, std::move(socket), rootFactory, usersData);
            }
        });
        service.run();
	}
	catch(const std::exception& e)
	{
		std::cerr << "Server start failed due to " << e.what() << '\n';
	}
}