#pragma once
#ifndef __ENTITY_CHUNK_H__
#define __ENTITY_CHUNK_H__

//Local Includes
#include "entity.hpp"

//Prototypes
class CEntityChunk
{
	//Member Functions
public:
	CEntityChunk();
	~CEntityChunk();

	bool Init(const CEntity* _pPrototype);

	bool AddEntity();
	bool RemoveEntity();

	template<typename T>
	void AddType();

	template<typename T>
	bool ContainsType() const;

	//Member Variables
public:
	IProperty<CEntity, CEntityChunk> entities;

	//Components found in every entity stored in this chunk
	IProperty<Components::IComponent, CEntityChunk> types;

protected:

};

#endif //__ENTITY_CHUNK_H__