#pragma once

#include "URI/Segment.hpp"
#include "HTTP/Responses/NotImplementedResponse.hpp"
#include "HTTP/Requests/Request.hpp"

namespace Users
{
    namespace AccessRights
    {
        class MethodWithPointer;
        class Methods;
    };
};

namespace Handlers
{
    class AHandler
    {
    public:
        using Ptr = AHandler*;

    protected:
        using MethodPointer = HTTP::Responses::IResponse::Ptr (AHandler::*)(HTTP::Requests::Request&&, URI::Segment);

    protected:
        AHandler()
        {
            static_assert(std::is_same<MethodPointer, decltype(&AHandler::doPost)>(), "AHandler POST method has invalid declaration");
            static_assert(std::is_same<MethodPointer, decltype(&AHandler::doGet)>(), "AHandler GET method has invalid declaration");
            static_assert(std::is_same<MethodPointer, decltype(&AHandler::doDelete)>(), "AHandler DELETE method has invalid declaration");
        }

    public:
        virtual ~AHandler() = default;

    protected:
        virtual HTTP::Responses::IResponse::Ptr doPost(HTTP::Requests::Request&& request, URI::Segment target)
        {
            return std::make_unique<HTTP::Responses::NotImplementedResponse>(std::move(request));
        }

        virtual HTTP::Responses::IResponse::Ptr doGet(HTTP::Requests::Request&& request, URI::Segment target)
        {
            return std::make_unique<HTTP::Responses::NotImplementedResponse>(std::move(request));
        }

        virtual HTTP::Responses::IResponse::Ptr doDelete(HTTP::Requests::Request&& request, URI::Segment target)
        {
            return std::make_unique<HTTP::Responses::NotImplementedResponse>(std::move(request));
        }

    public:
        virtual HTTP::Responses::IResponse::Ptr doRequest(HTTP::Requests::Request&& request, URI::Segment target, const Users::AccessRights::Methods& methodsAllowed)
        {
            switch (request.getMethod())
            {
            case HTTP::Requests::Method::get:
                return this->doGet(std::move(request), target);
                
            case HTTP::Requests::Method::post:
                return this->doPost(std::move(request), target);

            case HTTP::Requests::Method::delete_:
                return this->doDelete(std::move(request), target);
            
            default:
                return std::make_unique<HTTP::Responses::NotImplementedResponse>(std::move(request));
            }
        }

    private:
        friend class Users::AccessRights::Methods;
        friend class Users::AccessRights::MethodWithPointer;

    };

};

