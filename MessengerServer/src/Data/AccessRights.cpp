#include "Data/AccessRights.h"

#include <initializer_list>

using namespace Data;
using namespace MyRequestHandlers;


void AccessRights::appendResourceRights(const Resource& resource, std::initializer_list<Method>&& methods)
{
	const ResourcesMethods::iterator itResource = _resourceAccessRights.find(resource);
	if (itResource == _resourceAccessRights.end())
	{
		_resourceAccessRights.emplace_hint(itResource, resource, std::move(methods));
	}
	else
	{
		for (Method method : methods)
		{
			itResource->second.addMethod(method);
		}
	}
}

AccessRights::AccessRights(const AccessRights& accessRights, const Resource& resource, std::initializer_list<Method>&& methods) :
	AccessRights{accessRights}
{
	this->appendResourceRights(resource, std::move(methods));
}

AccessRights::AccessRights(const AccessRights& accessRights, std::initializer_list<std::pair<Resource, std::initializer_list<Method>&&>>&& rights) :
	AccessRights(accessRights)
{
	for(auto& pair : rights)
	{
		this->appendResourceRights(pair.first, std::move(pair.second));
	}
}

AccessRights::Resource AccessRights::getResourceWithoutFile(const Resource& resource) const
{
	const char segmentDivider = '/';
	const size_t extensionIndex = resource.find_last_of('.');
	const size_t lastSegmentStartIndex = resource.find_last_of(segmentDivider);
	
	if (extensionIndex == resource.npos)
	{
		return lastSegmentStartIndex == resource.npos ? resource : resource.substr(lastSegmentStartIndex);
	}
	else
	{
		const size_t prevLastSegmentStartIndex = resource.find_last_of(segmentDivider, lastSegmentStartIndex - 1);
		return resource.substr(prevLastSegmentStartIndex, lastSegmentStartIndex - prevLastSegmentStartIndex);
	}
}
