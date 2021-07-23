// ComplexLibrary.cpp : 정적 라이브러리를 위한 함수를 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "ComplexVector.h"
#include <iostream>

// TODO: 라이브러리 함수의 예제입니다.
void fnComplexLibrary()
{
	ComplexVector<int> vec, testv;
	vec.push_back(1);
	vec.push_back(2);
	vec.push_back(3);

	for(int i=0; i<vec.size();i++)
		printf("%d\n", vec.at(i));
}
