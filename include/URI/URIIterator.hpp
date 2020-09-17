#pragma once

#include "Segment.hpp"

namespace URI
{
    class URIIterator
    {
    public:
        inline URIIterator(const boost::string_view uri, bool includeFile = true) :
        _uri{uri.empty() ? uri : uri.substr(uri.find(SEGMENT_DIVIDER) + 1)}
        {
            if (!includeFile)
            {
                auto itEnd = _uri.rend();
                const auto pos = _uri.find_last_of(FILE_EXTENSION);
                if (pos != _uri.npos)
                {
                    _uri = _uri.substr(0, _uri.find_last_of(SEGMENT_DIVIDER, pos));
                }
            }
        }

        inline ~URIIterator() = default;

    public:
        inline bool isLast() const
        {
            return _uri.find(SEGMENT_DIVIDER) == _uri.npos;
        }

        inline bool hasNext() const
        {
            return !_uri.empty();
        }

        inline Segment next()
        {
            const size_t nextSegmentDividerIndex = _uri.find(SEGMENT_DIVIDER);
            Segment segment{_uri.substr(0, nextSegmentDividerIndex)};
            _uri = _uri.substr(nextSegmentDividerIndex == _uri.npos ? _uri.size() : nextSegmentDividerIndex + 1);
            return segment;
        }

    private:
        boost::string_view _uri;

    };
}
