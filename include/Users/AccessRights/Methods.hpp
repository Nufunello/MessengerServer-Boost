#pragma once

#include "Users/AccessRights/MethodWithPointer.hpp"

#include <algorithm>
#include <unordered_set>

namespace Users
{
    namespace AccessRights
    {      
        class Methods
        {
        public:
            inline Methods(std::initializer_list<MethodWithPointer> list)
                : _methods{std::move(list)}
            {}
            
        public:
            void addMethods(std::initializer_list<MethodWithPointer> list)
            {
                for (auto& method : list)
                {
                    _methods.emplace(std::move(method));
                }
            }
        
        public:
            Handlers::AHandler::MethodPointer const getMethodPointer(const HTTP::Requests::Method method) const
            {
                const auto itMethod = _methods.find(method);
                return itMethod == std::end(_methods) 
                        ? nullptr : itMethod->MethodPointer ;
            } 
            
        private:
            std::unordered_set<MethodWithPointer> _methods;

        };
    }
};
