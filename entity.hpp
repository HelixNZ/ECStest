#pragma once
#ifndef __ENTITY_H__
#define __ENTITY_H__

//Library Includes
#include <vector>

//Local Includes
#include "typefactory.hpp"
#include "properties.h"
#include "component.h"
#include "component_transform.h" //CEntity must have a transform

//Prototypes
class CEntity
{
	//Member Function
public:
	CEntity();
    CEntity(const CEntity& _rhs);
	~CEntity();

    template<typename T> T* AddComponent(const T* _pPrototype = nullptr);

	template<typename T> T* GetComponent() const;
	template<typename T> void GetComponents(std::vector<T*>& _rOutput);

    CEntity* AddChild(CEntity* _pPrototype = nullptr);
    CEntity* GetParent() const;

	//Member Variables
public:
    IProperty<Components::IComponent, CEntity> components;
    IProperty<CEntity, CEntity> children;

protected:
    CEntity* m_pParent;

};

//Implementation
CEntity::CEntity()
    : m_pParent(nullptr)
{
    //Constructor
    AddComponent<Components::CTranslation>();
    AddComponent<Components::CRotation>();
}

CEntity::CEntity(const CEntity& _rhs)
    : m_pParent(nullptr)
{
    //Copy Constructor
    auto& rTypeFactory = CTypeFactory<Components::IComponent>::GetInstance();

    //Copy components across using factory to ensure correct copy
    _rhs.components.ForEach([&](Components::IComponent* _pComponent)
    {
        components.data.push_back(rTypeFactory.Create(_pComponent));
    });
}

CEntity::~CEntity()
{
    //Destructor
    m_pParent = nullptr; //Unlink
}

template<typename T> T*
CEntity::AddComponent(const T* _pPrototype)
{
    components.data.push_back(_pPrototype ? new T(*_pPrototype) : new T);
    return((T*)components.data.back());
}

template<typename T> T*
CEntity::GetComponent() const
{
    T* pComponent = nullptr;
    for(auto i = 0; i < components.data.size(); ++i)
    {
        if(typeid(T) != typeid(*components.data[i])) continue;
        pComponent = (T*)components.data[i];
        break;
    }
    return(pComponent);
}

template<typename T> void
CEntity::GetComponents(std::vector<T*>& _rOutput)
{
    for(auto i = 0; i < components.data.size(); ++i)
    {
        if(typeid(T) == typeid(*components.data[i])) _rOutput.push_back((T*)components.data[i]);
    }
}

CEntity*
CEntity::AddChild(CEntity* _pPrototype)
{
    auto child = _pPrototype ? new CEntity(*_pPrototype) : new CEntity;
    child->m_pParent = this;
    children.data.push_back(child);
    return(child);
}

CEntity*
CEntity::GetParent() const
{
    return(m_pParent);
}

#endif //__ENTITY_H__
