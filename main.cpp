#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <iostream>
#include <string>
#include <vector>

#include <ctime>

#include "entity_manager.hpp"
#include "entity.hpp"
#include "component.h"
#include "component_transform.h"
#include "component_tags.h"
#include "properties.h"
#include "typefactory.hpp"

#include "profiler.h"

using namespace Components;

class ITest
{
public:
	virtual ~ITest() {};
};

class CTest: public ITest
{
public:
	virtual ~CTest() {};
};

int main()
{
	//Memory leak detection
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);

	//Seed random
	srand((unsigned int)time(0));

	//Type factory
	auto& rFactory = CTypeFactory<IComponent>::GetInstance();
	rFactory.RegisterType<CTranslation>();
	rFactory.RegisterType<CRotation>();
	rFactory.RegisterType<CScale>();
	rFactory.RegisterType<Tags::StaticTag>();
	rFactory.RegisterType<Tags::EnemyTag>();
	rFactory.RegisterType<Tags::PlayerTag>();

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

	//Clean up before perf test
	CEntityManager::DestroyInstance();

	//Performance testing of lambda vs. for loop in this instance
	for(int zip = 0; zip < 10; ++zip)
	{
		{
			ProfileCurrentScope("ForLoop");

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
			for(auto foo = 0U; foo < EntityManager.entities.Count(); ++foo)
			{
				auto _entity = *EntityManager.entities[foo];

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

				int i = 0;
				for(auto bar = 0U; bar < _entity.children.Count(); ++bar)
				{
					for(auto baz = 0U; baz < _entity.children[bar]->components.Count(); ++baz)
					{
						++i; //Valid and calls back up
					}
				}

				for(auto bar = 0U; bar < _entity.children.Count(); ++bar)
				{
					auto n = _entity.components.Count();
				}

				const IComponent* pComp = _entity.components[0];
				CTranslation* pComp2 = (CTranslation*)_entity.components[0];

				int b = 0;

				for(auto bar = 0U; bar < _entity.components.Count(); ++bar)
				{
					++b;
				}

				for(auto bar = 0U; bar < _entity.components.Count(); ++bar)
				{
					if(typeid(*_entity.components[bar]) == typeid(CTranslation))
					{
						++b;
						auto _trans = (CTranslation*)_entity.components[bar];
						_trans->m_vec3Pos = float3(0.0f, 5.0f, 0.0f);
					}
				}

				_entity.GetComponent<CTranslation>()->m_vec3Pos += float3(1.0f, 0.0f, 0.0f);

				for(auto bar = 0U; bar < _entity.components.Count(); ++bar)
				{
					if(typeid(*_entity.components[bar]) == typeid(CTranslation))
					{
						auto _trans = (CTranslation&)*_entity.components[bar];
					}
				}

				_entity.GetComponent<CTranslation>()->m_vec3Pos += float3(1.0f, 0.0f, 0.0f);

				for(auto bar = 0U; bar < _entity.components.Count(); ++bar)
				{
					if(typeid(*_entity.components[bar]) == typeid(CTranslation))
					{
						auto _trans = (CTranslation&)*_entity.components[bar];
					}
				}

				for(auto bar = 0U; bar < _entity.components.Count(); ++bar)
				{
					if(typeid(*_entity.components[bar]) == typeid(CTranslation))
					{
						++b;
						auto _trans = (CTranslation&)*_entity.components[bar];
						_trans.m_vec3Pos = float3(0.0f, 9.0f, 0.0f);
					}
				}

				for(auto bar = 0U; bar < _entity.components.Count(); ++bar)
				{
					if(typeid(*_entity.components[bar]) == typeid(CTranslation))
					{
						auto _trans = (CTranslation&)*_entity.components[bar];
					}
				}

				bool end = true;
			}

			CEntityManager::DestroyInstance();
		}

		{
			ProfileCurrentScope("LambdaF");

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

				int i = 0;
				_entity.children.ForEach([&](CEntity* _entity)
				{
					_entity->components.ForEach([&](IComponent* _component)
					{
						++i; //Valid and calls back up
					});
				});

				_entity.children.ForEach([](CEntity* _entity)
				{
					auto n = _entity->components.Count();
				});

				const IComponent* pComp = _entity.components[0];
				CTranslation* pComp2 = (CTranslation*)_entity.components[0];

				int b = 0;

				_entity.components.ForEach([&](IComponent* _component)
				{
					++b;
				});

				_entity.components.ForEach([&](CTranslation* _trans)
				{
					++b;
					_trans->m_vec3Pos = float3(0.0f, 5.0f, 0.0f);
				});

				_entity.GetComponent<CTranslation>()->m_vec3Pos += float3(1.0f, 0.0f, 0.0f);

				_entity.components.ForEach([](CTranslation& _trans)
				{
				});

				_entity.GetComponent<CTranslation>()->m_vec3Pos += float3(1.0f, 0.0f, 0.0f);

				_entity.components.ForEach([](CTranslation& _trans)
				{
				});

				_entity.components.ForEach([&](CTranslation& _trans)
				{
					++b;
					_trans.m_vec3Pos = float3(0.0f, 9.0f, 0.0f);
				});

				_entity.components.ForEach([](CTranslation& _trans)
				{
				});
			});

			CEntityManager::DestroyInstance();
		}
	}

	//Clean up
	CEntityManager::DestroyInstance();
	CTypeFactory<IComponent>::DestroyInstance();
	CProfiler::DestroyInstance();
	return(0);
}