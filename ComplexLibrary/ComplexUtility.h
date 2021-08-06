#pragma once

#include "ComplexString.h"
#include "ComplexCalculate.h"
#include "ComplexTime.h"

namespace ComplexUtility
{
	// 2~16 진법까지 지원
	ComplexString ConvertNotation(int befor_notation, int after_notation, ComplexString inputValue)
	{
		if (befor_notation < 2 || befor_notation >16)
			return "";

		if (after_notation < 2 || after_notation > 16)
			return "";

		return InputTenToSet(after_notation, OutputSetToTen(befor_notation, inputValue));
	}

	// 명도가 높으면 밝은색 낮으면 어두운색
	int GetBrightness(int nRv, int nGv, int nBv)
	{
		int nMax = nRv;
		int nMin = nRv;

		if (nMax < nGv) nMax = nGv;
		if (nMin > nGv) nMin = nGv;
		if (nMax < nBv) nMax = nBv;
		if (nMin > nBv) nMin = nBv;

		// nRv, nGv, nBv가 전부 동일
		if (nMax == nRv && nMin == nRv)
		{
			nMin = nGv;	// 그냥 nGv값을 최소값으로 설정 nRv만 아니면된다..
		}

		double dFirstBrightness = round(static_cast<double>(nMax) / 2.125);
		double dLastBrightness = round(static_cast<double>(nMin) / 2.125);

		return static_cast<int>(dFirstBrightness + dLastBrightness);
	}

	// 현재시간 리턴
	ComplexTime::ComplexTimeTable GetCurrentTime()
	{
		return ComplexTime::GetCurrentTime();
	}
}