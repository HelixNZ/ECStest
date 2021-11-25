#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <iostream>
#include <string>
#include <vector>

#include "vld.h"

#include <ctime>

#include "entity_manager.hpp"
#include "entity.hpp"
#include "component.h"
#include "component_transform.h"
#include "component_tags.h"
#include "properties.h"
#include "typefactory.hpp"

using namespace Components;

class ITest
{
public:
	virtual ~ITest() {};
};

class CTest : public ITest
{
public:
	virtual ~CTest() {};
};

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);

	srand((unsigned int)time(0));

	auto& rFactory = CTypeFactory<IComponent>::GetInstance();

	rFactory.RegisterType<CTranslation>();
	rFactory.RegisterType<CRotation>();
	rFactory.RegisterType<CScale>();

	rFactory.RegisterType<Tags::StaticTag>();
	rFactory.RegisterType<Tags::EnemyTag>();
	rFactory.RegisterType<Tags::PlayerTag>();


	//TODO: make a test case for where
	//		forEach player
	//			forEach enemy
	//				check distance for collision
	// 
	// this should give a rough idea of the flexibility of the current code
	// including calling back up to the "system" for variables such as a distance var
	// and any other things that may be useful
	// 
	// consider adding Add() Get() Remove() to IProperty, but this breaks the concept of read/write
	//

	//Manager
	CEntityManager& EntityManager = CEntityManager::GetInstance();

	//Unmanaged
	auto prototype = new CEntity;
	prototype->AddComponent<Tags::StaticTag>();
	((CTranslation*)prototype->components[0])->m_vec3Pos = float3(0.0f, 5.0f, 0.0f);
	

	auto scale = new Components::CScale();
	scale->m_vec3Scale = float3(10.0f, 10.0f, 6.0f);

	//Create 100 entities using the unmanaged prototype
	for(auto i = 0; i < 100; ++i)
	{
		CEntity* pEntity = EntityManager.CreateEntity(prototype);

		pEntity->AddComponent(scale);
	}
	
	delete prototype;
	delete scale;

	//Iterate through each entity
	EntityManager.entities.ForEach([](CEntity& _entity)
	{
		if(rand() % 100 > 75)
		{
			_entity.AddComponent<Tags::EnemyTag>();
			//_entity.RemoveComponent<Tags::StaticTag>();
		}

		_entity.AddChild();
		_entity.AddChild();
		_entity.AddChild();
		_entity.AddChild();

		_entity.children[1]->AddComponent<Tags::PlayerTag>();

		std::cout << "echild::" << _entity.children.Count() << "\n";

		int i = 0;
		_entity.children.ForEach([&](CEntity* _entity)
		{
			_entity->components.ForEach([&](IComponent* _component)
			{
				++i; //Valid and calls back up
				std::cout << "echild::" << typeid(*_component).name() << "\n";
			});
		});

		_entity.children.ForEach([](CEntity* _entity)
		{
			auto n = _entity->components.Count();
			std::cout << "entity2::" << n << "\n";
		});

		const IComponent* pComp = _entity.components[0];
		CTranslation* pComp2 = (CTranslation*)_entity.components[0];
		
		int b = 0;


		_entity.components.ForEach([&](IComponent* _component)
		{
			++b;
			std::cout << "entity4::" << typeid(*_component).name() << "\n";
		});

		_entity.components.ForEach([&](CTranslation* _trans)
		{
			++b;
			_trans->m_vec3Pos = float3(0.0f, 5.0f, 0.0f);
			std::cout << "entity5::(" << _trans->m_vec3Pos.x << "," << _trans->m_vec3Pos.y << "," << _trans->m_vec3Pos.z << ")\n";
		});

		_entity.GetComponent<CTranslation>()->m_vec3Pos += float3(1.0f, 0.0f, 0.0f);

		_entity.components.ForEach([](CTranslation& _trans)
		{
			std::cout << "entity6::(" << _trans.m_vec3Pos.x << "," << _trans.m_vec3Pos.y << "," << _trans.m_vec3Pos.z << ")\n";
		});

		_entity.GetComponent<CTranslation>()->m_vec3Pos += float3(1.0f, 0.0f, 0.0f);

		_entity.components.ForEach([](CTranslation& _trans)
		{
			std::cout << "entity7::(" << _trans.m_vec3Pos.x << "," << _trans.m_vec3Pos.y << "," << _trans.m_vec3Pos.z << ")\n";
		});

		_entity.components.ForEach([&](CTranslation& _trans)
		{
			++b;
			_trans.m_vec3Pos = float3(0.0f, 9.0f, 0.0f);
			std::cout << "entity8::(" << _trans.m_vec3Pos.x << "," << _trans.m_vec3Pos.y << "," << _trans.m_vec3Pos.z << ")\n";
		});

		_entity.components.ForEach([](CTranslation& _trans)
		{
			std::cout << "entity9::(" << _trans.m_vec3Pos.x << "," << _trans.m_vec3Pos.y << "," << _trans.m_vec3Pos.z << ")\n";
		});

		bool end = true;
	});

	/*EntityManager.ForEachWithTag<EnemyTag>([](CEntity& _entity)
	{
		_entity.GetComponent<CTranslation>()->m_vec3Pos += float3(5.0f, 0.0f, 0.0f);
	});*/

	//Destroy
	CTypeFactory<IComponent>::DestroyInstance();
	CEntityManager::DestroyInstance();
	return(0);
}