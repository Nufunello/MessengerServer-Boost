#pragma once

#include "Handlers/AStrictedHandler.hpp"

#include "HTTP/Responses/UnauthorizedResponse.hpp"

namespace Handlers
{
    class AAuthorizedHandler
        : public AStrictedHandler
    {
    public:
        AAuthorizedHandler() = default;
        virtual ~AAuthorizedHandler() = default;

    protected:
        virtual HTTP::Responses::IResponse::Ptr findAppropiateResponse(HTTP::Requests::Request&& request, const HTTP::Requests::Method method) override
        {
            boost::ignore_unused(method);
            return std::make_unique<HTTP::Responses::UnauthorizedResponse>(std::move(request));
        }

    };
}
