#pragma once

#include "Handlers/AHandler.hpp"
#include "HTTP/Responses/ResourceNotFoundResponse.hpp"
#include "HTTP/Responses/OKResponse.hpp"

#include <map>

#include <boost/filesystem.hpp>

namespace Handlers
{
    class WebPageLoader
    {
    public:
        using Map = std::map<std::string, std::string, std::less<>>;

    private:
        static Map readDir(const std::string_view relativePath)
        {
            const boost::filesystem::path dirPath = boost::filesystem::current_path().append(relativePath);

            Map map;
            for(auto & file : boost::filesystem::directory_iterator(dirPath))
            {
                const auto& path = file.path();
                if (boost::filesystem::is_regular_file(path))
                {
                    std::string filename = path.filename().generic_string();
                    const auto it = map.emplace(std::move(filename), std::string{}).first;
                    boost::filesystem::load_string_file(path, it->second);
                }
            }
            return map;
        }

    public:
        WebPageLoader(const std::string_view webPagesDir)
            : _webPages{readDir(webPagesDir)}
        {}

        virtual ~WebPageLoader() = default;

    public:
        HTTP::Responses::HTTPResponse::Ptr getResponseWithPage(HTTP::Requests::Request&& request, const boost::string_view page) const
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
    
}
