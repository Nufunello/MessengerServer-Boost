#include "Handlers/RootRequestHandler.h"
#include "Exceptions/RedirectException.h"
#include "Data/AuthorizedUsersMap.h"

#include <iostream>

#include <boost/asio/streambuf.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ip/address.hpp>

#include <boost/beast/http/read.hpp>
#include <boost/beast/http/write.hpp>
#include <boost/beast/http/string_body.hpp>


void acceptConnection(boost::asio::io_service& service, Data::AuthorizedUsersMap& authorizedUsersMap, MyRequestHandlers::RootRequestHandler& rootHandler, boost::asio::ip::tcp::acceptor& acceptor)
{
	boost::asio::streambuf stream;

	boost::asio::ip::tcp::socket socket(service);
	acceptor.accept(socket);

	boost::beast::http::request_parser<boost::beast::http::string_body> parser;
	boost::beast::error_code ec;
	boost::beast::http::read_header(socket, stream, parser, ec);

	const MyRequestHandlers::Message& message = parser.get();
	const boost::beast::string_view target = message.target();

	MyRequestHandlers::Response response;

	const size_t index = target.find('/') + 1;
	try
	{
		response = rootHandler.processRequest(target.substr(index), message); 
	}
	catch(const Exceptions::RedirectException& exception)
	{
		response.result(exception.Code);
		response.reason(exception.what());
		response.insert(MyRequestHandlers::Field::host, exception.Where);
	}
	catch(const Exceptions::RequestException& exception) 
	{
		response.result(exception.Code);
		response.reason(exception.what());
	}
	catch(const std::exception& exception)
	{
		response.result(MyRequestHandlers::Status::internal_server_error);
	}

	boost::beast::http::write(socket, response);

	socket.close();
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

		Data::AuthorizedUsersMap authorizedUsersMap;
		MyRequestHandlers::RootRequestHandler rootHandler{authorizedUsersMap};

		boost::asio::ip::tcp::acceptor acceptor(service, endpoint);
		while (true)
		{
			acceptConnection(service, authorizedUsersMap, rootHandler, acceptor);
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << "Server failed due to " << e.what() << '\n';
	}
	
}
