#include "Handlers/AMappedRequestHandler.h"

#include "Exceptions/ResourceNotFoundException.h"

using namespace MyRequestHandlers;

Response AMappedRequestHandler::processRequest(boost::string_view currentTarget, const Message& message)
{
	size_t index = currentTarget.find('/');

	if (index == currentTarget.npos)
	{
		if (currentTarget.find('.') != currentTarget.npos)
		{
			return ARequestHandler::processRequest(std::move(currentTarget), message);
		}
		else
		{
			index = currentTarget.length();
		}
	}

	const boost::string_view currentSegment = currentTarget.substr(0, index);
    auto itPHandler = _handlers.find(currentSegment);

	if (itPHandler != _handlers.end())
	{
		ARequestHandler* pHandler = itPHandler->second.get();

		const size_t length = currentTarget.length() - index;
        const boost::string_view nextTarget = index == currentTarget.length() 
							? boost::string_view() 
							: currentTarget.substr(index + 1, length);
        
		return pHandler->processRequest(std::move(nextTarget), message);
	}
	else
	{
		throw Exceptions::ResourceNotFoundException();
	}
}
