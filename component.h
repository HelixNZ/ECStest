#pragma once
#ifndef __COMPONENT_H__
#define __COMPONENT_H__

namespace Components
{
	class IComponent
	{
	public:
		virtual ~IComponent() = default;

	protected:
		IComponent() = default;
		IComponent(IComponent&&) = default;
		IComponent(const IComponent&) = default;

	};
}

#endif //__COMPONENT_H__
