#pragma once

#include "Factories/AHandlerFactory.hpp"

#include <boost/container/flat_map.hpp>

namespace Factories
{
    class AMappedHandlerFactory
        : public AHandlerFactory
    {
    public:
        using Map = boost::container::flat_map<boost::string_view, Factories::AHandlerFactory::Ptr>;

    private:
        static void emplaceInMap(Map& map)
        {}

        template<typename Segment, typename Pointer, typename ...Segments>
        static void emplaceInMap(Map& map, Segment segment, Pointer pointer, Segments... segments)
        {
            map.emplace(segment, std::move(pointer));
            emplaceInMap(map, std::move(segments)...);
        }

        template<typename Segment, typename Pointer, typename ...Segments>
        static Map makeMap(Segment segment, Pointer pointer, Segments... segments)
        {
            static_assert((sizeof...(segments) & 1) == 0, "Pack size should be even");
            Map map;
            map.reserve(sizeof...(segments));
            emplaceInMap(map, segment, std::move(pointer), std::move(segments)...);
            return map;
        }

    public:
        AMappedHandlerFactory() = delete;

        template<typename ...Segments>
        inline AMappedHandlerFactory(Segments... segments)
            : _factories{makeMap(std::move(segments)...)}
        {}

        virtual ~AMappedHandlerFactory() = default;
    
    public:
        virtual Handlers::AHandler::Ptr getHandler(URI::URIIterator& iterator) const override
        {
            if (iterator.hasNext())
            {
                URI::Segment segment = iterator.next();
                if (!segment.isFile())
                {
                    const auto handlerFactory = findFactory(segment);
                    return handlerFactory ? handlerFactory->getHandler(iterator) : nullptr;
                }
                else
                {
                    iterator = URI::URIIterator(segment);
                }
            }
            return AHandlerFactory::getHandler(iterator);
        }

    private:
        inline AHandlerFactory* findFactory(const URI::Segment segment) const
        {
            const auto itHandlerFactory = _factories.find(segment);
            return itHandlerFactory == std::end(_factories) ? nullptr : itHandlerFactory->second.get();
        }

    private:
        const Map _factories;
        
    };
};
