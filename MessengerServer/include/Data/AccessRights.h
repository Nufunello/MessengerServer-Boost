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
		Methods() = default;

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
		using ResourcesMethods = std::map<Resource, Methods>;

	public:
		AccessRights() = default;

		AccessRights(Resource resource, std::initializer_list<Method>&& methods) :
			_resourceAccessRights{{std::move(resource), Methods{std::move(methods)}}}
		{}

		AccessRights(const AccessRights& accessRights) = default;
		AccessRights(const AccessRights& accessRights, const Resource& resource, std::initializer_list<Method>&& methods);

		AccessRights(const AccessRights& accessRights, std::initializer_list<std::pair<Resource, std::initializer_list<Method>&&>>&& rights);


		virtual ~AccessRights() = default;

	private:
		Resource getResourceWithoutFile(const Resource& resource) const;
		void appendResourceRights(const Resource& resource, std::initializer_list<Method>&& methods);

	public:
		bool hasAccess(const Resource& resource, const Method method) const
		{
			const Resource& withoutFile { this->getResourceWithoutFile(resource) };
			const ResourcesMethods::const_iterator itResource = _resourceAccessRights.find(withoutFile);
			return itResource != _resourceAccessRights.end() && itResource->second.containsMethod(method);
		}

	private:
		ResourcesMethods _resourceAccessRights;

	};
}