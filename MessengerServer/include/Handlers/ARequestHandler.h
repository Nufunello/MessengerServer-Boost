#pragma once

#include "Data/AuthorizedUsersMap.h"
#include "Handlers/RequestTypes.h"
#include "Exceptions/NotImplementedMethodException.h"

namespace MyRequestHandlers
{
	class ARequestHandler
	{
	public:
		using Ptr = std::unique_ptr<ARequestHandler>;

	protected:
		ARequestHandler(const Data::AuthorizedUsersMap& authorizedUsers) :
			_authorizedUsersConst{authorizedUsers}
		{}

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
		const Data::AccessRights& getAccessRights(const Message& message) const
		{
			const Message::const_iterator itToken = message.find(Field::cookie);
			return itToken == message.end() ? _authorizedUsersConst.getUnathorizedAccessRights() : _authorizedUsersConst.getAccessRights(itToken->value());
		}

	protected:
		bool hasRights(const Message& message) const
		{
			const Data::AccessRights& accessRights = this->getAccessRights(message);
			return accessRights.hasAccess(message.target().to_string(), message.method());
		}

	private:
        const Data::AuthorizedUsersMap& _authorizedUsersConst;

	};
}
