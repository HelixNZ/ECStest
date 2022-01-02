#pragma once
#ifndef __ENTITY_MANAGER_H__
#define __ENTITY_MANAGER_H__

//Local Includes
#include "properties.h"
#include "entity.hpp"

//Prototype
class CEntityManager
{
	//Member Functions
public:
	static CEntityManager& GetInstance();
	static void DestroyInstance();

	CEntity* CreateEntity(CEntity* _pPrototype = nullptr);

private:
	CEntityManager();
	CEntityManager(const CEntityManager&) = default;
	~CEntityManager();

	//Member Variables
public:
	IProperty<CEntity, CEntityManager> entities;

protected:
	static CEntityManager* sm_pThis;

};


//Singleton
CEntityManager* CEntityManager::sm_pThis = nullptr;

//Implementation
CEntityManager::CEntityManager()
{
	//Constructor
}

CEntityManager::~CEntityManager()
{
	//Destructor
}

CEntityManager&
CEntityManager::GetInstance()
{
	//Singleton Create
	if(!sm_pThis) sm_pThis = new CEntityManager();
	return(*sm_pThis);
}

void
CEntityManager::DestroyInstance()
{
	//Singleton Destroy
	if(sm_pThis)
	{
		delete sm_pThis;
		sm_pThis = nullptr;
	}
}

CEntity*
CEntityManager::CreateEntity(CEntity* _pPrototype)
{
	CEntity* pEntity = nullptr;

	entities.data.push_back(_pPrototype ? new CEntity(*_pPrototype) : new CEntity);
	pEntity = entities.data.back();

	return(pEntity);
}

#endif //__ENTITY_MANAGER_H__