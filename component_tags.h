#pragma once
#ifndef __COMPONENT_TAGS_H__
#define __COMPONENT_TAGS_H__

//Local Includes
#include "component.h"

//Macro for tags
#define MakeTag(_tagName) struct _tagName: IComponent {};

//Tags
namespace Components
{
	namespace Tags
	{
		/// <summary>
		/// Used for static objects that don't require updating in world
		/// </summary>
		MakeTag(StaticTag);

		/// <summary>
		/// Quick tag lookup for the players
		/// </summary>
		MakeTag(PlayerTag);

		/// <summary>
		/// Tag for quickly iterating on Enemies
		/// </summary>
		MakeTag(EnemyTag);

		/// <summary>
		/// Entities that are bullets
		/// </summary>
		MakeTag(BulletTag);
	}
}

#endif //__COMPONENT_TAGS_H__