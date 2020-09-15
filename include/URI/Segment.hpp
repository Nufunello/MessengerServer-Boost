#pragma once

#include <string_view>
#include <algorithm>

namespace URI
{
    constexpr char SEGMENT_DIVIDER = '/';
    constexpr char FILE_EXTENSION  = '.';

    class Segment
    {
    public:
        inline Segment(std::string_view segment) :
        _segment{segment}
        {}

        inline ~Segment() = default;

    public:
        inline bool isEmpty() const
        {
            return _segment.empty();
        }

        inline bool isFile() const
        {
            const std::string_view::const_reverse_iterator end = _segment.rend();
            const std::string_view::const_reverse_iterator it = std::find_if(_segment.rbegin(), end, [](const char symbol){ return symbol == SEGMENT_DIVIDER || symbol == FILE_EXTENSION; });
            return it != end && *it == FILE_EXTENSION;
        }

        inline operator std::string_view() const
        {
            return _segment;
        }

    private:
        const std::string_view _segment;

    };
};
