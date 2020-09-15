#pragma once

#include "Users/AccessRights/Methods.hpp"
#include "HTTP/Responses/MethodNotAllowedResponse.hpp"
#include "HTTP/Requests/Request.hpp"

namespace Handlers
{
    class AStrictedHandler
        : public AHandler
    {
    protected:
        AStrictedHandler() = default;
    
    public:
        virtual ~AStrictedHandler() = default;

    protected:
        virtual HTTP::Responses::IResponse::Ptr findAppropiateResponse(HTTP::Requests::Request&& request, const HTTP::Requests::Method method)
        {
            return std::make_unique<HTTP::Responses::MethodNotAllowedResponse>(std::move(request));
        }

    public:
        virtual HTTP::Responses::IResponse::Ptr doRequest(HTTP::Requests::Request&& request, URI::Segment target, const Users::AccessRights::Methods& methodsAllowed) override
        {
            const HTTP::Requests::Method method = request.getMethod();
            auto methodPointer = methodsAllowed.getMethodPointer(method);
            return (methodPointer == nullptr) ? this->findAppropiateResponse(std::move(request), method) : (this->*methodPointer)(std::move(request), target);
        }

    };
};
