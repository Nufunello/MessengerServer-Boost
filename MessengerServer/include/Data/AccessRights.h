#pragma once

#include "Handlers/RequestTypes.h"

#include <map>
#include <string>

namespace Data
{
	using Method = MyRequestHandlers::Method;

	class Methods
	{
	private:
		using Iterator = std::vector<Method>::const_iterator;

	public:
		Methods(std::initializer_list<Method>&& list) :
			_methods(std::move(list))
		{}

	private:
		Iterator findMethod(const Method method, const Iterator& itEnd) const
		{
			const Iterator itBegin = _methods.begin();
			return std::find(itBegin, itEnd, method);
		}

	public:
		bool containsMethod(Method method) const
		{
			const Iterator itEnd = _methods.end();
			return findMethod(method, itEnd) != itEnd;
		}

		void addMethod(Method method)
		{
			if (!this->containsMethod(method))
				_methods.push_back(method);
		}

		void removeMethod(Method method)
		{
			const Iterator itEnd = _methods.end();
			const Iterator itMethod = findMethod(method, itEnd);
			if (itMethod != itEnd)
				_methods.erase(itMethod);
		}

	private:
		std::vector<Method> _methods;
		
	};

	class AccessRights
	{
	public:
		using Resource = std::string;

	private:
		using Resources = std::map<Resource, Methods>;

	public:
		AccessRights(Resource resource, std::initializer_list<Method>&& methods) :
			_resourceAccessRights{{std::move(resource), Methods{std::move(methods)}}}
		{}

		AccessRights(const AccessRights& accessRights) = default;
		AccessRights(const AccessRights& accessRights, const Resource& resource, std::initializer_list<Method>&& methods);

		virtual ~AccessRights() = default;

	private:
		Resource getResourceWithoutFile(const Resource& resource) const
		{
			const char segmentDivider = '/';

			const size_t extensionIndex = resource.find_last_of('.');
			const size_t lastSegmentStartIndex = resource.find_last_of(segmentDivider);
			
			if (extensionIndex == resource.npos)
			{
				return resource.substr(lastSegmentStartIndex);
			}
			else
			{
				const size_t prevLastSegmentStartIndex = resource.find_last_of(segmentDivider, lastSegmentStartIndex - 1);
				return resource.substr(prevLastSegmentStartIndex, lastSegmentStartIndex - prevLastSegmentStartIndex);
			}
		}

	public:
		bool hasAccess(const Resource& resource, const Method method) const
		{
			const Resource& withoutFile { this->getResourceWithoutFile(resource) };
			const Resources::const_iterator itResource = _resourceAccessRights.find(withoutFile);
			return itResource != _resourceAccessRights.end() && itResource->second.containsMethod(method);
		}

	private:
		Resources _resourceAccessRights;

	};
}