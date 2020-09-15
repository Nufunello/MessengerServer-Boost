#pragma once

#include "Handlers/AHandler.hpp"
#include "HTTP/Responses/ResourceNotFoundResponse.hpp"
#include "HTTP/Responses/OKResponse.hpp"

#include <map>

namespace std
{
    template<>
    struct less<std::string_view>
    {
        using is_transparent = void; // Enable comparison with the view
    
        template<class T>
        bool operator ()(std::string const& lhs, T&& rhs) const {
            return lhs < rhs;
        }
        
        template<class T>
        bool operator ()(T&& lhs, std::string const& rhs,
            typename std::enable_if<!std::is_same_v<std::string,
                typename std::decay<T>::type>>::type* = nullptr) const {
            return lhs < rhs;
        }
    }
};

namespace Handlers
{
    class WebPageLoader
    {
    public:
        using Map = std::map<std::string_view, std::string>;

    public:
        WebPageLoader(const std::string_view webPagesDir)
            : _webPages{{"index.html", "IndexPage"}}
        {}

        virtual ~WebPageLoader() = default;

    public:
        HTTP::Responses::HTTPResponse::Ptr getResponseWithPage(HTTP::Requests::Request&& request, const std::string_view page) const
        {
            const auto itWebPage = _webPages.find(page.empty() ? "index.html" : page);
            if (itWebPage == std::end(_webPages))
                return std::make_unique<HTTP::Responses::ResourceNotFoundResponse>(std::move(request));
            else
                return std::make_unique<HTTP::Responses::OKResponse>(std::move(request), std::move(itWebPage->second));
        }

    private:
        const Map _webPages;
    };
    
};
