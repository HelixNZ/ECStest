#pragma once
#ifndef __FUNCTION_TRAITS_H__
#define __FUNCTION_TRAITS_H__

//Taken from the following sources:
//  https://stackoverflow.com/questions/2562320/specializing-a-template-on-a-lambda-in-c0x
//	https://github.com/kennytm/utils/blob/master/traits.hpp

//Library Includes
#include <tuple>

//Prototypes
template <typename T>
struct function_traits
	: public function_traits<decltype(&T::operator())>
{
};
// For generic types, directly use the result of the signature of its 'operator()'

template <typename ClassType, typename ReturnType, typename... Args>
struct function_traits<ReturnType(ClassType::*)(Args...) const>
	// we specialize for pointers to member function
{
	enum { arity = sizeof...(Args) };
	// arity is the number of arguments.

	//Added as ::arg<n>::type is finnicky
	typedef std::tuple<Args...> all;

	typedef ReturnType result_type;

	template <size_t i>
	struct arg
	{
		typedef typename std::tuple_element<i, all>::type type;
		// the i-th argument is equivalent to the i-th tuple element of a tuple
		// composed of those arguments.
	};
};

#endif //__FUNCTION_TRAITS_H__