#pragma once

#include "Handlers/RequestTypes.h"

#include <boost/utility/string_view.hpp>
#include <boost/filesystem.hpp>

namespace MyRequestHandlers
{
    using Path = boost::filesystem::path;

    class ResourceFileMap
    {
    public:
        using FileName = std::string;
        using Content  = std::string;

    public:        
        ResourceFileMap(const std::string& dirPath);
        virtual ~ResourceFileMap() = default;

    protected:
        Response getResponseWithPage(const boost::string_view& filenameView) const;

    private:
        const std::map<FileName, Content> _files;

    };
}
