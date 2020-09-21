#pragma once

#include "Handlers/AStrictedHandler.hpp"
#include "Handlers/WebPageLoader.hpp"

#include "HTTP/Responses/OKResponse.hpp"
#include "HTTP/Responses/UnauthorizedResponse.hpp"
#include "HTTP/Responses/UserAlreadyAuthorizedResponse.hpp"

#include "Users/Data/UsersData.hpp"

namespace Handlers
{
    class LoginHandler
        : public AStrictedHandler
        , private WebPageLoader
    {
    public:
        LoginHandler(Users::Data::UsersData& usersData)
            : WebPageLoader{"/webpages/login"}
            , _usersData{usersData}
        {}

        ~LoginHandler() = default;

    protected:
        HTTP::Responses::IResponse::Ptr doGet(HTTP::Requests::Request&& request, URI::Segment target) override
        {
            return WebPageLoader::getResponseWithPage(std::move(request), target);
        }

        HTTP::Responses::IResponse::Ptr doPost(HTTP::Requests::Request&& request, URI::Segment target) override
        {
            const HTTP::Requests::Message& message = request.message();
            const HTTP::Requests::Message::const_iterator itName = message.find(HTTP::Requests::Field::authorization);

            if (itName == std::end(message))
            {
                return std::make_unique<HTTP::Responses::HTTPResponse>(std::move(request), HTTP::Responses::Status::bad_request, "Authorization required");
            }

            using namespace Users::Data;
            
            UsersData::LoginStatus status;
            std::string token = _usersData.addUser(itName->value(), status);

            switch(status)
            {
                case UsersData::LoginStatus::OK:
                {
                    HTTP::Responses::Response response;
                    response.result(HTTP::Responses::Status::ok);
                    response.reason("OK");
                    response.insert(HTTP::Requests::Field::set_cookie, token);
                    return std::make_unique<HTTP::Responses::HTTPResponse>(std::move(request), std::move(response));
                }

                case UsersData::LoginStatus::UsernameAlredyInUse:
                {
                    return std::make_unique<HTTP::Responses::HTTPResponse>(std::move(request), HTTP::Responses::Status::bad_request, "Username is already in use");
                }

                default:
                {
                    return std::make_unique<HTTP::Responses::BadRequestResponse>(std::move(request));
                }
            }

            return WebPageLoader::getResponseWithPage(std::move(request), target);
        }

        HTTP::Responses::IResponse::Ptr doDelete(HTTP::Requests::Request&& request, URI::Segment target) override
        {
            auto token = request.getToken();
            if (!token.has_value())
            {
                return std::make_unique<HTTP::Responses::UnauthorizedResponse>(std::move(request));
            }

            using namespace Users::Data;
            
            UsersData::LogoutStatus status = _usersData.LogoutUser(token.value());

            switch(status)
            {
                case UsersData::LogoutStatus::OK:
                {
                    HTTP::Responses::Response response;
                    response.result(HTTP::Responses::Status::ok);
                    response.reason("OK");
                    response.insert(HTTP::Requests::Field::set_cookie, "");
                    return std::make_unique<HTTP::Responses::HTTPResponse>(std::move(request), std::move(response));
                }

                default:
                {
                    return std::make_unique<HTTP::Responses::BadRequestResponse>(std::move(request));
                }
            }
        }

    protected:
        virtual HTTP::Responses::IResponse::Ptr findAppropiateResponse(HTTP::Requests::Request&& request, const HTTP::Requests::Method method) override
        {
            switch (method)
            {
            case HTTP::Requests::Method::post:
            case HTTP::Requests::Method::get:
                return std::make_unique<HTTP::Responses::UserAlreadyAuthorizedResponse>(std::move(request));

            case HTTP::Requests::Method::delete_:
                return std::make_unique<HTTP::Responses::UnauthorizedResponse>(std::move(request));
            
            default:
                return AStrictedHandler::findAppropiateResponse(std::move(request), method);
            }
        }

    private:
        Users::Data::UsersData& _usersData;
        
    };
    
};
