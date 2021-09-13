#pragma once

#include "ComplexTime.h"

namespace ComplexLibrary
{
	class ComplexRandom
	{
	public:

		ComplexRandom()
		{

		}

		virtual ~ComplexRandom()
		{

		}

		int Random(int start_range, int end_range)
		{
			if (start_range > end_range)
			{
				int tmp = end_range;
				end_range = start_range;
				start_range = tmp;
			}
			else if (start_range == end_range)
				return end_range;

			int range_count = (end_range - start_range) + 1;
			int seed = Seed(range_count);
			seed += start_range;
			return seed;
		}

		int Random()
		{
			return Seed();
		}

	private:

		int Seed(int last_seed = 0)
		{
			ComplexTime tm;

			static unsigned long long seed = (unsigned long long)tm.GetCurrentTime();

			// ù��° �õ�
			ComplexTimeTable table1 = tm.GetCurrentTime() - 2847172;
			unsigned long long firstSeed = (unsigned long long)table1;

			// �ι�° �õ�
			unsigned long long secondSeed = (firstSeed % seed) * seed;

			// ����° �õ�
			ComplexTimeTable table2 = tm.GetCurrentTime() - 5691928;
			unsigned long long d = (unsigned long long)table2;
			unsigned long long thirdSeed = (d - firstSeed) * d;

			// �׹�° �õ�
			unsigned long long fourthSeed = abs(int(firstSeed * secondSeed / thirdSeed));

			seed = firstSeed * seed + secondSeed / thirdSeed * fourthSeed;

			// ������ �õ�
			unsigned long long lastSeed = (unsigned long long)last_seed;
			if (last_seed == 0)
				lastSeed = seed % fourthSeed;

			return int(seed % lastSeed);
		}

	};
}