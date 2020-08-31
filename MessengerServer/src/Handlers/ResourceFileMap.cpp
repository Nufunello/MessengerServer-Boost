#include "Handlers/ResourceFileMap.h"

#include "Exceptions/RequestException.h"

using namespace MyRequestHandlers;

namespace {
    using Map = std::map<ResourceFileMap::FileName, ResourceFileMap::Content>;

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
                map.emplace(filename, ResourceFileMap::Content()).first;
                boost::filesystem::load_string_file(path, map.at(filename));
            }
        }
        return map;
    }
}

ResourceFileMap::ResourceFileMap(const std::string& relPath) :
    _files{getMap(relPath)}
{}

Response ResourceFileMap::getResponseWithPage(const boost::string_view& filenameView) const
{
    const FileName& filename = filenameView.empty() ? "index.html" : filenameView.to_string();
    const std::map<FileName, Content>::const_iterator itFile = _files.find(filename);
    if (itFile == _files.end())
    {
        throw Exceptions::RequestException("Not found", Status::not_found);
    }
    else
    {        
        Response response;
        response.result(Status::ok);
        response.body() = itFile->second;
        return response;
    }
}
