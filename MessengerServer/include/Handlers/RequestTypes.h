#pragma once

#include <boost/beast/http/message.hpp>
#include <boost/beast/http/string_body.hpp>

namespace MyRequestHandlers
{
	using Field = boost::beast::http::field;
	using Response = boost::beast::http::response<boost::beast::http::string_body>;
	using Message = boost::beast::http::message<true, boost::beast::http::string_body>;
	using Header = Message::header_type;
	using Method = boost::beast::http::verb;
	using Status = boost::beast::http::status;
}
