#pragma once
#ifndef __PROPERTIES_H__
#define __PROPERTIES_H__

//Library Includes
#include <vector>

//Local Includes
#include "functiontraits.h"

/// <summary>
/// Property class for allowing fast function iteration over stored data
/// </summary>
/// <typeparam name="T">Type stored by the property, expands to std::vector<T*> by default</typeparam>
/// <typeparam name="_friend">Owner class of this property needs to be placed here for full access</typeparam>
template<typename T, class _friend>
class IProperty
{
    //Member Functions
protected:
    IProperty() = default;
    ~IProperty()
    {
        //Destructor
        for(auto i = 0; i < data.size(); ++i)
        {
            if(data[i])
            {
                delete data[i];
                data[i] = nullptr;
            }
        }

        data.clear();
    };

public:

    /// <summary>
    /// ForEach lambda that accepts any type offered by the property
    /// <para>Parameter can be pointer or ref, always looping on valid data</para>
    /// </summary>
    /// <param name="_func">lambda[](type* _item){}</param>
    template<typename _funcType>
    void ForEach(_funcType _func) const
    {
        //Strip lambda args
        typedef function_traits<_funcType>::all traits; //Strip down the lambda
        typedef std::tuple_element<0, traits>::type _type; //Get the type of the first argument for _func
        typedef std::remove_pointer<_type>::type _type2; //Remove the pointer if it has one, std::decay does not work here and removing reference does nothing to aid us
        typedef std::conditional<std::is_pointer<_type>::value, int, bool>::type _select; //using int for * and bool for &

        //TODO: revist this function and consider ways to optimize it
        //      consider supporting more than one arg parsed
        for(auto i = 0; i < data.size(); ++i)
        {
            auto val = data[i];
            bool bSameAsT = typeid(_type) == typeid(val);
            bool bSameType = typeid(_type2) == typeid(*val);

            //If val is a valid pointer and typesafe, execute func selecting ref or pointer
            if(val && (bSameAsT || bSameType)) _func((_type)select(_select(), i));
        }
    }

    unsigned int Count() const
    {
        return((unsigned int)data.size());
    }

    T* operator[](unsigned int _uiIndex)
    {
        return(data[_uiIndex]);
    }

private:
    IProperty(const IProperty& _rhs) = default;

    //Helper functions for forcing reference
    T* select(int _s, int _i) const { return(data[_i]); }
    T& select(bool _s, int _i) const { return(*data[_i]); }

    //Member Variables
protected:
    std::vector<T*> data; //We own and control the memory of these
    friend _friend;
};

#endif //__PROPERTIES_H__