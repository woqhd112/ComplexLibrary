#pragma once

#include "ComplexTime.h"

class ComplexRandom
{
public:

	ComplexRandom()
	{
	
	}

	~ComplexRandom()
	{
	
	}

	unsigned long long Random(unsigned long long start_range, unsigned long long end_range)
	{
		
	}

private:

	unsigned long long Seed(unsigned long long seed)
	{
		ComplexTime tm;
		ComplexTimeTable table = tm.GetCurrentTime();

		seed = 99999 * seed + 555555555 / 23241 * 44235;

		return seed % 223124;
	}

};