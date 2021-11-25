#pragma once
#ifndef __VECTOR_MATH_H__
#define __VECTOR_MATH_H__

//Types
struct float3
{
	float x, y, z;

	float3()
		: x(0.0f)
		, y(0.0f)
		, z(0.0f)
	{
		//Constructor
	}

	float3(float _x, float _y, float _z)
		: x(_x)
		, y(_y)
		, z(_z)
	{
		//Constructor
	}

	void operator+=(const float3& _rhs)
	{
		x += _rhs.x;
		y += _rhs.y;
		z += _rhs.z;
	}
};

struct float4
{
	float x, y, z, w;

	float4()
		: x(0.0f)
		, y(0.0f)
		, z(0.0f)
		, w(1.0f)
	{
		//Constructor
	}

	float4(float _x, float _y, float _z, float _w)
		: x(_x)
		, y(_y)
		, z(_z)
		, w(_w)
	{
		//Constructor
	}
};

struct quaternion: float4
{
	quaternion()
	{
		//Constructor
	}

	quaternion(float _x, float _y, float _z, float _w)
	{
		//Constructor
	}

	float3 ToEuler() const
	{
		return(float3());
	}
};

struct float4x4
{
	//Variables
	float m[4][4];

	//Functions
	float4x4()
	{
		//Constructor
		Identity();
	}

	void Identity()
	{
		for(int row = 0; row < 4; ++row)
		{
			for(int col = 0; col < 4; ++col) m[row][col] = (row == col ? 1.0f : 0.0f);
		}
	}
};

/// <summary>
/// Allows for HLSL style swizzling of vectors
/// <para> - swizzle(pixel, vertexColor, argb);</para>
/// <para> - swizzle(flippedTex, texCoord, vu);</para>
/// <para> - swizzle(scalarSize, vec4pos, www);</para>
/// </summary>
/// <param name="a">Destination vector</param>
/// <param name="b">Source vector</param>
/// <param name="op">Component pattern (eg. xy, yz, xywz, rgba, uv)</param>
#define swizzle(a, b, op) memswizzle(a, b, #op)

/// <summary>
/// Allows for HLSL style swizzling of vectors
/// <para>Utilize swizzle(a, b, op)</para>
/// </summary>
/// <typeparam name="tLHS">Destination Type</typeparam>
/// <typeparam name="tRHS">Source Type</typeparam>
/// <param name="_lhs">Destination vector</param>
/// <param name="_rhs">Source vector</param>
/// <param name="_op">Operation pattern (eg. "xy", "zyx", "xxyw", "uv", "rgba")</param>
template<typename tLHS, typename tRHS, size_t _len>
void memswizzle(tLHS& _lhs, const tRHS& _rhs, const char(&_op)[_len])
{
	//Compile-time check to see if we're swizzling the correct number of components into the output
	//We could accept less components here but for now to be on the safe side, only accept 1:1
	static_assert((sizeof(tLHS) / sizeof(float)) == (_len - 1), "Swizzle operation does not match destination size");

	//Check for xyz with three components or less (len includes null-terminator)
	if(tolower(_op[0]) > 'w' && _len <= 4)
	{
		//Quick swizzle assuming xy or xyz combos (no w, non-uv, non-rgba)
		for(size_t i = 0; i < (_len - 1); ++i) ((float*)&_lhs)[i] = ((float*)&_rhs)[(int)(tolower(_op[i]) - 'x')];
	}
	else
	{
		//Swizzle using a map. Slower but allows for every variation expected
		const char* map = tolower(_op[0]) < 'w' ? tolower(_op[0]) < 'u' ? "rgba" : "uv" : "xyzw";
		for(size_t i = 0; i < (_len - 1); ++i) ((float*)&_lhs)[i] = ((float*)&_rhs)[(int)(strchr(map, tolower(_op[i])) - map)];
	}
}

#endif //__VECTOR_MATH_H__