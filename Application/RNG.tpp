#pragma once
#include <random>

namespace RNG
{
	template<int A, int B>
	int generate()
	{
		static std::uniform_int_distribution<int> uid(A, B); //create distribution for a given interval
		return uid(rng); 
	}
}

