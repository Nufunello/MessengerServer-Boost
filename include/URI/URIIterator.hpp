#pragma once

#include "Segment.hpp"

namespace URI
{
    class URIIterator
    {
    public:
        inline URIIterator(const std::string_view uri) :
        _uri{uri.empty() ? uri : uri.substr(uri.find(SEGMENT_DIVIDER) + 1)}
        {}

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
            Segment segment(_uri.substr(0, nextSegmentDividerIndex));
            _uri = _uri.substr(nextSegmentDividerIndex == _uri.npos ? _uri.size() : nextSegmentDividerIndex + 1);
            return segment;
        }

    private:
        std::string_view _uri;

    };
}
