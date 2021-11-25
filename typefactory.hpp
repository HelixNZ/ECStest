#pragma once
#ifndef __TYPE_FACTORY_H__
#define __TYPE_FACTORY_H__

//Library Includes
#include <assert.h>
#include <unordered_map>

//Prototypes
template<class T>
class CTypeFactory
{
	//Private Types
private:
	template<typename T>
	class IFactoryInterface
	{
		//Member Functions
	public:
		virtual ~IFactoryInterface() {};
		virtual T* Create(void* _pData = nullptr) = 0;
	};

	template<typename T, typename _baseClass>
	class IFactory: public IFactoryInterface<_baseClass>
	{
	public:
		T* Create(void* _pData = nullptr)
		{
			return(new T(*reinterpret_cast<T*>(_pData)));
		}
	};

	//Member Functions
public:
	static CTypeFactory<T>& GetInstance()
	{
		if(!sm_pThis) sm_pThis = new CTypeFactory<T>();
		return(*sm_pThis);
	}

	static void DestroyInstance()
	{
		if(sm_pThis)
		{
			delete sm_pThis;
			sm_pThis = nullptr;
		}
	}

	template<typename _type>
	void RegisterType()
	{
		static_assert(std::is_base_of<T, _type>::value, "Unable to register type to factory as it is not of base type");

		if(m_umapFactories.find(typeid(_type).name()) == m_umapFactories.end())
		{
			m_umapFactories.insert(std::pair<std::string, IFactoryInterface<T>*>(typeid(_type).name(), new IFactory<_type, T>));
		}
	}

	T* Create(const char* _pcType, void* _pData = nullptr)
	{
		T* pObject = nullptr;

		if(_pcType)
		{
			try
			{
				pObject = m_umapFactories.at(_pcType)->Create(_pData);
			}
			catch(const std::out_of_range&)
			{
				//No key match
				assert(!"No factory found for object, have you Registered the type?");
			}
		}

		return(pObject);
	}

	T* Create(const T* _pGuess, void* _pData)
	{
		return(_pGuess ? Create(typeid(*_pGuess).name(), _pData) : nullptr);
	}

	T* Create(const T* _pGuess)
	{
		return(_pGuess ? Create(typeid(*_pGuess).name(), (void*)_pGuess) : nullptr);
	}

private:
	CTypeFactory() = default;
	~CTypeFactory()
	{
		for(auto const& keypair : m_umapFactories) if(keypair.second) delete keypair.second;
		m_umapFactories.clear();
	};

	//Member Variables
protected:
	static CTypeFactory<T>* sm_pThis;
	std::unordered_map<std::string, IFactoryInterface<T>*> m_umapFactories;

};

template<typename T>
CTypeFactory<T>* CTypeFactory<T>::sm_pThis = nullptr;

#endif //__TYPE_FACTORY_H__