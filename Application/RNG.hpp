#pragma once
#include <random>

namespace RNG
{
	template<int A, int B>
	int generate();
	
	namespace //anonymous namespace to simulate private membership
	{
		static inline std::random_device rd;
		static inline std::mt19937 rng(rd());
	}
}

#include "RNG.tpp"