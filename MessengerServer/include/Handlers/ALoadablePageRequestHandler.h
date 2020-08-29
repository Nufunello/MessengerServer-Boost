#pragma once

#include "Handlers/ARequestHandler.h"

#include <boost/filesystem.hpp>

namespace MyRequestHandlers
{
    using Path = boost::filesystem::path;

    class ALoadablePageRequestHandler :
        public ARequestHandler
    {
    public:
        using FileName = std::string;
        using Content  = std::string;

    public:
        ALoadablePageRequestHandler(const std::string& dirPath);
        virtual ~ALoadablePageRequestHandler() = default;

    protected:
        virtual Response doGet(boost::string_view currentTarget, const Message& message) override;

    private:
        const std::map<FileName, Content> _files;

    };
}
