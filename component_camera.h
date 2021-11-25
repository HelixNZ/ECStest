#pragma once
#ifndef __COMPONENT_CAMERA_H__
#define __COMPONENT_CAMERA_H__

//Local Includes
#include "component.h"
#include "vectormath.h"

namespace Components
{
	struct CCamera: IComponent
	{
		float4x4 m_matView;
		float4x4 m_matProj;

		float m_fFOV;
		float m_fNear;
		float m_fFar;
	};
}

#endif //__COMPONENT_CAMERA_H__
