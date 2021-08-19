#pragma once

#include "ComplexString.h"
#include "ComplexStack.h"

char ConvertIntToChar(int nInt)
{
	if (nInt == 0) return '0';
	else if (nInt == 1) return '1';
	else if (nInt == 2) return '2';
	else if (nInt == 3) return '3';
	else if (nInt == 4) return '4';
	else if (nInt == 5) return '5';
	else if (nInt == 6) return '6';
	else if (nInt == 7) return '7';
	else if (nInt == 8) return '8';
	else if (nInt == 9) return '9';
	else if (nInt == 10) return 'a';
	else if (nInt == 11) return 'b';
	else if (nInt == 12) return 'c';
	else if (nInt == 13) return 'd';
	else if (nInt == 14) return 'e';
	else if (nInt == 15) return 'f';

	return '\0';
}

int ConvertCharToInt(ComplexString strStr)
{
	if (strStr == "0") return 0;
	else if (strStr == "1") return 1;
	else if (strStr == "2") return 2;
	else if (strStr == "3") return 3;
	else if (strStr == "4") return 4;
	else if (strStr == "5") return 5;
	else if (strStr == "6") return 6;
	else if (strStr == "7") return 7;
	else if (strStr == "8") return 8;
	else if (strStr == "9") return 9;
	else if (strStr == "a") return 10;
	else if (strStr == "b") return 11;
	else if (strStr == "c") return 12;
	else if (strStr == "d") return 13;
	else if (strStr == "e") return 14;
	else if (strStr == "f") return 15;

	return 0;
}

ComplexString InputTenToSet(int nMacro, ComplexString strOutput)
{
	ComplexString strText;
	ComplexString strFormat;
	int nBase = 0;
	int nRemainder = 0;
	int nShare = 0;
	int nSize = 0;
	char cTemp = 0;
	char cRemainder = '\0';
	char cShare = '\0';
	ComplexStack<char> totalBase;

	strText = strOutput;
	nBase = atoi(strText.GetBuffer());

	while (true)
	{
		nRemainder = nBase % nMacro;
		nShare = nBase / nMacro;

		if (nShare > (nMacro - 1))
		{
			nBase = nShare;
			cRemainder = ConvertIntToChar(nRemainder);
			totalBase.push(cRemainder);
		}
		else
		{
			cRemainder = ConvertIntToChar(nRemainder);
			cShare = ConvertIntToChar(nShare);
			totalBase.push(cRemainder);
			totalBase.push(cShare);
			break;
		}
	}

	nSize = (int)totalBase.size();
	for (int i = 0; i < nSize; i++)
	{
		cTemp = totalBase.top();
		strFormat.AppendFormat("%c", cTemp);
		totalBase.pop();
	}
	return strFormat;
}

ComplexString OutputSetToTen(int nMacro, ComplexString value)
{
	ComplexString strTest;
	ComplexString strFormat;
	int nLength = 0;
	int nSum = 0;
	int nConvertInt = 0;

	nLength = value.GetLength();

	for (int i = 0; i < nLength; i++)
	{
		strTest = (ComplexString)value.GetAt(i);
		strTest.SetAllLower();
		nConvertInt = ConvertCharToInt(strTest);
		nConvertInt = static_cast<int>(nConvertInt * pow(nMacro, nLength - (i + 1)));
		nSum += nConvertInt;
	}
	strFormat.Format("%d", nSum);
	return strFormat;
}
