#pragma once
#ifndef __COMPONENT_TRANSFORM_H__
#define __COMPONENT_TRANSFORM_H__

//Local Includes
#include "component.h"
#include "vectormath.h"

namespace Components
{
	struct CTranslation: IComponent
	{
		float3 m_vec3Pos;
	};

	struct CScale: IComponent
	{
		float3 m_vec3Scale;
	};

	struct CRotation: IComponent
	{
		quaternion m_quatRot;
	};
}

#endif //__COMPONENT_TRANSFORM_H__
