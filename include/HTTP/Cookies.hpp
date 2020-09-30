#pragma once

#include <variant>

#include <boost/beast.hpp>

namespace HTTP
{
    class Cookie
    {
    private:
        template <typename Name, typename Value, typename ...Attributes>
        inline void append(Name&& name, Value&& value, Attributes&&... attributes)
        {
            _cookies.append(" " + std::string(name) + "=" + std::string(value) + ";");
            if constexpr(sizeof...(attributes) > 0)
            {
                this->append(std::forward<Attributes>(attributes)...);
            }
        }

    public:
        Cookie() = default;

        inline Cookie(const boost::string_view cookies)
            : _cookies{cookies}
        {
        }

        template <typename Name, typename Value, typename ...Attributes>
        inline Cookie(Name&& name, Value&& value, Attributes&&... attributes)
            : _cookies{}
        {
            append(name, value, std::forward<Attributes>(attributes)...);
        }

        inline std::optional<boost::string_view> getAttribute(const boost::string_view name) const
        {
            const boost::string_view cookies = _cookies;
            const auto itAttr = cookies.find(name);
            if (itAttr != cookies.npos)
            {
                const auto itAttrValueStart = cookies.find('=', itAttr + 1) + 1;
                const auto itAttrValueEnd = cookies.find(';', itAttrValueStart + 1);
                if (itAttrValueStart != cookies.npos)
                {
                    const auto length = itAttrValueEnd == cookies.npos ? cookies.npos : itAttrValueEnd - itAttrValueStart;
                    return cookies.substr(itAttrValueStart, length);
                }
            }
            return {};
        }

        inline const std::string& asString() const
        {
            return _cookies;
        }

    private:
        std::string _cookies;
    
    };
}