#pragma once

#include "Users/AccessRights/Methods.hpp"

#include <string>
#include <set>

namespace Users
{
    namespace AccessRights
    {
        class AccessRightsNode
        {
        private:
            struct Comparator
            {
                using is_transparent = void;
                
                bool operator()(const std::string& lhs, const std::string& rhs) const
                {
                    return lhs < rhs;
                }
                bool operator()(std::string_view lhs, const std::string& rhs) const
                {
                    return lhs < rhs;
                }
                bool operator()(const std::string& lhs, std::string_view rhs) const
                {
                    return lhs < rhs;
                }
            };

        public:
            AccessRightsNode(std::string name, Users::AccessRights::Methods allowedMethods)
                : _name{std::move(name)}
                , _allowedMethods{std::move(allowedMethods)}
            {}

            ~AccessRightsNode()
            {}

        public:
            const AccessRightsNode* next(const std::string_view nextNodeName) const
            {
                const auto itChild = _children.find(nextNodeName);
                return itChild == std::end(_children)
                    ? nullptr : &*itChild;
            }

        private:
            const std::string _name;
            Users::AccessRights::Methods _allowedMethods;

            std::set<AccessRightsNode, Comparator> _children;

        };
        
    };
};
