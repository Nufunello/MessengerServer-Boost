#pragma once

#include <boost/utility/string_view.hpp>
#include <algorithm>

namespace URI
{
    constexpr char SEGMENT_DIVIDER = '/';
    constexpr char FILE_EXTENSION  = '.';

    class Segment
    {
    public:
        inline Segment(boost::string_view segment) :
        _segment{segment}
        {}

        inline ~Segment() = default;

    public:
        inline Segment& operator=(const Segment& segment)
        {
            this->_segment = segment._segment;
            return *this;
        }

    public:
        inline bool isEmpty() const
        {
            return _segment.empty();
        }

        inline bool isFile() const
        {
            if (isEmpty()) return false;
            const boost::string_view::const_reverse_iterator end = _segment.rend();
            const boost::string_view::const_reverse_iterator it = std::find_if(_segment.rbegin(), end, [](const char symbol){ return symbol == SEGMENT_DIVIDER || symbol == FILE_EXTENSION; });
            return it != end && *it == FILE_EXTENSION;
        }

        inline operator boost::string_view() const
        {
            return _segment;
        }

    private:
        boost::string_view _segment;

    };
};
