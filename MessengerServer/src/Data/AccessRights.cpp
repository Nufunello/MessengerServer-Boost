#include "Data/AccessRights.h"

#include <initializer_list>

using namespace Data;
using namespace MyRequestHandlers;

AccessRights::AccessRights(const AccessRights& accessRights, const Resource& resource, std::initializer_list<Method>&& methods) :
	AccessRights{accessRights}
{
	const Resources::iterator it = _resourceAccessRights.find(resource);
	if (it == _resourceAccessRights.end())
	{
		_resourceAccessRights.emplace(resource, std::move(methods));
	}
	else
	{
		Methods& addedMethods = it->second;
		std::for_each(methods.begin(), methods.end(), [&addedMethods](const Method method){
			addedMethods.addMethod(method);
		});
	}
}
