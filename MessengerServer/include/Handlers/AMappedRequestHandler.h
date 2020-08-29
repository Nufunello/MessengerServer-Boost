#pragma once

#include "Handlers/ARequestHandler.h"

#include <map>

namespace MyRequestHandlers
{
    class AMappedRequestHandler :
        public ARequestHandler
    {
    public:
        using HandlerKey = boost::string_view;
        using HandlersMap = std::map<HandlerKey, ARequestHandler::Ptr>;

    protected:
        AMappedRequestHandler(HandlersMap&& map) :
            _handlers{std::move(map)}
        {}

    public:
        virtual ~AMappedRequestHandler() = default;
        
    public:
		virtual Response processRequest(boost::string_view currentTarget, const Message& message) override;

    private:
        const HandlersMap _handlers;
    };
}
