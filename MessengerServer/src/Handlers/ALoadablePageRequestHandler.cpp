#include "Handlers/ALoadablePageRequestHandler.h"

using namespace MyRequestHandlers;

namespace {
    using Map = std::map<ALoadablePageRequestHandler::FileName, ALoadablePageRequestHandler::Content>;
    
    const ALoadablePageRequestHandler::FileName DEFAULT_LOAD_FILE {"index.html"};

    Map getMap(const std::string& path)
    {
        const boost::filesystem::path dirPath = boost::filesystem::current_path().append(path);
        const boost::filesystem::directory_iterator eod;

        Map map;
        for (boost::filesystem::directory_iterator iter(dirPath); iter != eod; ++iter)
        {
            const Path& path = iter->path();
            if (boost::filesystem::is_regular_file(path))
            {
                const std::string filename = path.filename().generic_string();
                map.emplace(filename, ALoadablePageRequestHandler::Content()).first;
                boost::filesystem::load_string_file(path, map.at(filename));
            }
        }
        return map;
    }
}

ALoadablePageRequestHandler::ALoadablePageRequestHandler(const std::string& relPath) :
    _files{getMap(relPath)}
{
}

Response ALoadablePageRequestHandler::doGet(boost::string_view currentTarget, const Message& message)
{
    const FileName& filename = currentTarget.empty() ? DEFAULT_LOAD_FILE : currentTarget.to_string();
    const Map::const_iterator it = _files.find(filename);
    if (it != _files.end())
    {
        Response response;
        response.body() = std::move(it->second);
        response.result(Status::ok);
        response.reason("OK");
        return response;
    }
    else
    {
        throw Exceptions::RequestException("File not found", Status::not_found);
    }
}
