#pragma once
#ifndef __ENTITY_MANAGER_H__
#define __ENTITY_MANAGER_H__

//Local Includes
#include "properties.h"
#include "entity.hpp"
//#include "entity_chunk.h"

//Prototype
class CEntityManager
{
	//Member Functions
public:
	static CEntityManager& GetInstance();
	static void DestroyInstance();

	CEntity* CreateEntity(CEntity* _pPrototype = nullptr);

	//template<typename _funcType>
	//void ForEach(_funcType _func);

	//template<typename T, typename _funcType>
	//void ForEachWithType(_funcType _func);

private:
	CEntityManager();
	CEntityManager(const CEntityManager&) = default;
	~CEntityManager();

	//Member Variables
public:
	IProperty<CEntity, CEntityManager> entities;

protected:
	static CEntityManager* sm_pThis;
	//IProperty<CEntityChunk, CEntityManager> m_chunks; //protected as we don't want to expose this

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
    //unsigned int uiECCount = pEntity->components.Count();

    entities.data.push_back(_pPrototype ? new CEntity(*_pPrototype) : new CEntity);
    pEntity = entities.data.back();

    /*if(_pPrototype)
    {
        _pPrototype->components.ForEach([&](IComponent* _pComponent)
        {
        });
    }*/

    //For each chunk
    //m_chunks.ForEach([&](CEntityChunk& _rChunk)
    //{
    //    unsigned int uiCTCount = _rChunk.types.Count();

    //    //Match for component count
    //    if(uiECCount == uiCTCount)
    //    {
    //        bool bSameTypes = true;

    //        //For each type
    //        for(auto i = 0; i < uiECCount; ++i)
    //        {
    //            for(auto j = 0; j < uiCTCount; ++j)
    //            {

    //            }
    //        }
    //        //If it contains the type we are looking for
    //        if(_rChunk.ContainsType<T>())
    //        {
    //            //For each entity in chunk (_func is a lambda so pass it on)
    //            _rChunk.entities.ForEach(_func);
    //        }
    //    }
    //});

    //if(!pEntity)
    //{
    //    //suitable chunk not found
    //    //make new chunk with matching types
    //}

    return(pEntity);
}


//Templated Function Implementation
//template<typename _funcType>
//void CEntityManager::ForEach(_funcType _func)
//{
//	//& all lambda
//	//For each chunk
//	m_chunks.ForEach([&](CEntityChunk& _rChunk)
//	{
//		//For each entity in chunk (_func is a lambda so pass it on)
//		_rChunk.entities.ForEach(_func);
//	});
//}
//
//template<typename T, typename _funcType>
//void CEntityManager::ForEachWithType(_funcType _func)
//{
//	//& all lambda
//	//For each chunk
//	m_chunks.ForEach([&](CEntityChunk& _rChunk)
//	{
//		//If it contains the type we are looking for
//		if(_rChunk.ContainsType<T>())
//		{
//			//For each entity in chunk (_func is a lambda so pass it on)
//			_rChunk.entities.ForEach(_func);
//		}
//	});
//}

#endif //__ENTITY_MANAGER_H__