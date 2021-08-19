#pragma once

#include "ComplexString.h"
#include "ComplexCalculate.h"
#include "ComplexRandom.h"

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

	int StringToInt(ComplexString value)
	{
		const char* buf = value.GetBuffer();
		int size = value.GetLength();

		int total = 0;
		int count = 0;
		for (int i = size - 1; i >= 0; i--)
		{
			int su = buf[i] - 48;
			total += (su * static_cast<int>(pow(10, count)));
			count++;
		}

		return total;
	}

	ComplexString Int64ToString(unsigned long long value)
	{
		unsigned long long tmp = value;
		int count = 0;
		char* newBuf = new char[4096];
		while (tmp)
		{
			int remainder = tmp % 10;
			newBuf[count] = remainder + 48;

			tmp /= 10;
			count++;
		}
		newBuf[count] = '\0';

		char* returnBuf = new char[4096];
		int j = 0;
		for (int i = count - 1; i >= 0; i--)
		{
			returnBuf[j] = newBuf[i];
			j++;
		}
		returnBuf[count] = '\0';

		ComplexString returnString = returnBuf;
		delete[] newBuf;
		delete[] returnBuf;

		return returnString;
	}

	ComplexString IntToString(int value)
	{
		int tmp = value;
		int count = 0;
		char* newBuf = new char[256];
		while (tmp)
		{
			int remainder = tmp % 10;
			newBuf[count] = remainder + 48;

			tmp /= 10;
			count++;
		}
		newBuf[count] = '\0';

		char* returnBuf = new char[256];
		int j = 0;
		for (int i = count - 1; i >= 0; i--)
		{
			returnBuf[j] = newBuf[i];
			j++;
		}
		returnBuf[count] = '\0';

		ComplexString returnString = returnBuf;
		delete[] newBuf;
		delete[] returnBuf;

		return returnString;
	}

	ComplexString GetText(int begin_idx, int end_idx, ComplexString& buf)
	{
		if (end_idx - begin_idx <= 0)
			return "";
		int text_size = end_idx - begin_idx + 1;
		char* text_buf = new char[text_size + 1];
		int buf_idx = 0;
		for (int i = begin_idx; i <= end_idx; i++)
		{
			text_buf[buf_idx] = buf[i];
			buf_idx++;
		}
		text_buf[text_size] = '\0';
		ComplexString text = text_buf;
		delete[] text_buf;

		return text;
	}

	double StringToDouble(ComplexString value)
	{
		int find_dot_idx = value.Find(".");
		if (find_dot_idx == -1)
			find_dot_idx = value.GetLength();

		ComplexString dot_before_buf = GetText(0, find_dot_idx - 1, value);
		ComplexString dot_after_buf = GetText(find_dot_idx + 1, value.GetLength() - 1, value);

		int after_size = dot_after_buf.GetLength();

		int after_factor = static_cast<int>(pow(10, after_size));

		int dot_before_int = StringToInt(dot_before_buf);
		int dot_after_int = StringToInt(dot_after_buf);

		double dot_after_double = static_cast<double>(dot_after_int) / after_factor;

		double total_double = 0.0;

		total_double += dot_before_int;
		total_double += dot_after_double;

		return total_double;
	}

	ComplexString DoubleToString(double value)
	{
		int share = static_cast<int>(value);
		int calc_share = share;
		int share_size = 0;

		while (calc_share != 0)
		{
			calc_share /= 10;
			share_size++;
		}

		double remainder = value - share;
		double calc_remainder = remainder;
		int remainder_size = 0;

		unsigned long long floor_remainder_dot_after = static_cast<unsigned long long>(calc_remainder);

		while (calc_remainder != floor_remainder_dot_after)
		{
			calc_remainder *= 10;
			remainder_size++;
			floor_remainder_dot_after = static_cast<unsigned long long>(calc_remainder);
		}

		ComplexString share_buf = IntToString(share);
		ComplexString remainder_buf = Int64ToString(floor_remainder_dot_after);

		ComplexString returnBuf;

		if (remainder_buf.IsEmpty())
			returnBuf.Format("%s", share_buf.GetBuffer());
		else
			returnBuf.Format("%s.%s", share_buf.GetBuffer(), remainder_buf.GetBuffer());

		return returnBuf;
	}

	int Random()
	{
		ComplexRandom ran;
		return ran.Random();
	}

	int Random(int start_range, int end_range)
	{
		ComplexRandom ran;
		return ran.Random(start_range, end_range);
	}
}