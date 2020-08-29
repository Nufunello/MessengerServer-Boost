#pragma once

#include "Handlers/RequestTypes.h"
#include "Exceptions/NotImplementedMethodException.h"

namespace MyRequestHandlers
{
	class ARequestHandler
	{
	public:
		using Ptr = std::unique_ptr<ARequestHandler>;

	protected:
		ARequestHandler() = default;

	public:
		virtual ~ARequestHandler() = default;

	protected:
		virtual Response doGet(boost::string_view currentTarget, const Message& message)
		{
			throw Exceptions::NotImplementedMethodException();
		}

		virtual Response doPost(boost::string_view currentTarget, const Message& message)
		{
			throw Exceptions::NotImplementedMethodException();
		}

		virtual Response doDelete(boost::string_view currentTarget, const Message& message)
		{
			throw Exceptions::NotImplementedMethodException();
		}

	public:
		virtual Response processRequest(boost::string_view currentTarget, const Message& message);

	private:

	};
}
