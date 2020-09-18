#pragma once

#include "Users/AccessRights/AccessRightsNode.hpp"

namespace Users
{
    namespace AccessRights
    {
        class RootAccessRightsNode
            : public AccessRightsNode
        {
        private:
            void emplace()
            {
            }

            template <typename Path, typename Methods, typename ...Args>
            void emplace(Path path, Methods methods, Args... args)
            {
                this->appendAccessRightNode(std::move(path), std::move(methods));
                RootAccessRightsNode::emplace(std::move(args)...);
            }

        public:
            template <typename ...Args>
            RootAccessRightsNode(Args... args)
                : AccessRightsNode{"/", {}}
            {
                static_assert(sizeof...(args) % 2 == 0, "Arguments size should be even(path, methods list)");
                RootAccessRightsNode::emplace(std::move(args)...);
            }

            RootAccessRightsNode(const RootAccessRightsNode&) = default;
            ~RootAccessRightsNode()                           = default;
        };
        
    };
};
