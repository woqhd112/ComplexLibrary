#pragma once

#include "ComplexIterator.h"
#include "ComplexLinkedList.h"
#include "ComplexMap.h"
#include "ComplexNode.h"
#include "ComplexPool.h"
#include "ComplexQueue.h"
#include "ComplexSet.h"
#include "ComplexStack.h"
#include "ComplexUniqueQueue.h"
#include "ComplexVector.h"
#include "ComplexLog.h"
#include "ComplexFile.h"
#include "ComplexFormat.h"
#include "ComplexSingleton.h"
#include "ComplexCondition.h"
#include "ComplexLock.h"
#include "ComplexThread.h"
#include "ComplexXMLHandler.h"
#include "ComplexXMLNode.h"
#include "ComplexXMLParsor.h"
#include "ComplexConnectionPool.h"
#include "ComplexDatabase.h"
#include "ComplexPrint.h"
#include "ComplexRandom.h"
#include "ComplexStringTokenizer.h"
#include "ComplexTime.h"
#include "ComplexUtility.h"
#include "ComplexSystem.h"
#include "ComplexCollection.h"

using namespace ComplexLibrary;

namespace ComplexLibrary
{
	static ComplexSystem System;
	static ComplexCollection Collection;
	static ComplexUtility Util;
	static ComplexXMLHandler XML;

	class ComplexLibrary
	{
	public:

		ComplexLibrary()
		{
			System = ComplexSingleton<ComplexSystem>::GetInstance();
			Collection = ComplexSingleton<ComplexCollection>::GetInstance();
			Util = ComplexSingleton<ComplexUtility>::GetInstance();
			XML = ComplexSingleton<ComplexXMLHandler>::GetInstance();
		}

		virtual ~ComplexLibrary()
		{
			ComplexSingleton<ComplexSystem>::DestroyInstance();
			ComplexSingleton<ComplexCollection>::DestroyInstance();
			ComplexSingleton<ComplexUtility>::DestroyInstance();
			ComplexSingleton<ComplexXMLHandler>::DestroyInstance();
		}

	private:



	};
}


