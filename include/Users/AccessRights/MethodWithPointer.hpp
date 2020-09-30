#pragma once

#include "Handlers/AHandler.hpp"

#include <stdexcept>

namespace Users
{
    namespace AccessRights
    {     
        class MethodWithPointer
        {
        private:
            static constexpr Handlers::AHandler::MethodPointer getMethodPointer(const HTTP::Requests::Method method)
            {
                switch (method)
                {
                case HTTP::Requests::Method::get:
                    return &Handlers::AHandler::doGet;
                    
                case HTTP::Requests::Method::post:
                    return &Handlers::AHandler::doPost;

                case HTTP::Requests::Method::delete_:
                    return &Handlers::AHandler::doDelete;
                
                default:
                    throw std::runtime_error("Invalid method");
                }
            }

        public:
            constexpr MethodWithPointer(const HTTP::Requests::Method method)
                : Method{method}
                , MethodPointer{getMethodPointer(method)}
            {
            }

            bool operator ==(const MethodWithPointer& other) const
            {
                return this->Method == other.Method;
            }

        public:
            const HTTP::Requests::Method Method;
            const Handlers::AHandler::MethodPointer MethodPointer;
        
        };
    }
}

namespace std {
    template <>
    struct hash<Users::AccessRights::MethodWithPointer>
    {
        size_t operator()(const Users::AccessRights::MethodWithPointer& k) const
        {
            return size_t(k.Method);
        }
    };
}
