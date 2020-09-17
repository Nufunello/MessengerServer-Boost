#pragma once

#include "Users/AccessRights/Methods.hpp"
#include "URI/URIIterator.hpp"

#include <string>
#include <set>

#include <boost/utility/string_view.hpp>

namespace Users
{
    namespace AccessRights
    {
        class AccessRightsNode
        {
        private:
            AccessRightsNode() 
                : _allowedMethods{{}}
            {}

            AccessRightsNode& operator=(AccessRightsNode&& accessRightsNode)
            {
                this->_name = accessRightsNode._name;
                this->_children = std::move(accessRightsNode._children);
                this->_allowedMethods = std::move(accessRightsNode._allowedMethods);

                return *this;
            } 

        public:
            AccessRightsNode(std::string name, Users::AccessRights::Methods allowedMethods)
                : _name{std::move(name)}
                , _allowedMethods{std::move(allowedMethods)}
            {}

            AccessRightsNode(AccessRightsNode&&) = default;

            ~AccessRightsNode() = default;

        private:
            void emplace(const boost::string_view name, Users::AccessRights::Methods allowedMethods = {})
            {
                auto it = _children.emplace(name.to_string(), AccessRightsNode()).first;
                it->second = AccessRightsNode(it->first, std::move(allowedMethods));
            }

        public:
            void appendAccessRightNode(const boost::string_view fullPath, Users::AccessRights::Methods allowedMethods)
            {
                URI::URIIterator uriIterator{fullPath, false};
                URI::Segment name{uriIterator.next()};

                AccessRightsNode* prevAccessRightsNode = this;
                AccessRightsNode* currentAccessRightsNode = this->next(name);

                while(uriIterator.hasNext() && currentAccessRightsNode != nullptr)
                {
                    name = uriIterator.next();
                    prevAccessRightsNode = currentAccessRightsNode;
                    currentAccessRightsNode = this->next(name);
                }

                if (currentAccessRightsNode != nullptr)
                {
                    currentAccessRightsNode->emplace(name, std::move(allowedMethods));
                }
                else
                {
                    while (uriIterator.hasNext())
                    {
                        prevAccessRightsNode->emplace(name);
                        name = uriIterator.next();
                    }
                    prevAccessRightsNode->emplace(name, std::move(allowedMethods));
                }
            }

            AccessRightsNode* next(const boost::string_view nextNodeName)
            {
                const auto itChild = _children.find(nextNodeName);
                return itChild == std::end(_children) ? nullptr : &itChild->second;
            }

        private:
            boost::string_view     _name;
            Users::AccessRights::Methods _allowedMethods;

            std::map<std::string, AccessRightsNode, std::less<>> _children;

        };
        
    };
};
