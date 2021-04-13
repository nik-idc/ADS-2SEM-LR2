#include "pch.h"
#include "CppUnitTest.h"
#include "..\sf.h"
#include <ctime>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UTLR2
{
	TEST_CLASS(UTLR2)
	{
	public:

		TEST_METHOD(TestEncode)
		{
			srand(time(NULL));

			std::string a;

			size_t strSize = rand() % 100 + 1, codedSize = 0;

			for (size_t i = 0; i < strSize; i++)
				a.push_back(rand() % 255);

			sfCode s;

			s.encode(a);

			std::ifstream f;
			f.open("coded.txt", std::ifstream::ate | std::ifstream::binary);
			codedSize = f.tellg();
			f.close();

			Assert::IsTrue(codedSize < a.size());
		}

		TEST_METHOD(TestDecode)
		{
			srand(time(NULL));

			std::string a;

			size_t strSize = rand() % 100 + 1, codedSize = 0;

			for (size_t i = 0; i < strSize; i++)
				a.push_back(rand() % 255);

			sfCode s;

			s.encode(a);
			s.decode();

			char ch = 0;
			std::string decodedStr;
			std::ifstream f;
			f.open("decoded.txt", std::ifstream::in | std::ifstream::binary);
			while (f >> std::noskipws >> ch)
				decodedStr.push_back(ch);
			f.close();
			Assert::IsTrue(decodedStr == a);
		}
	};
}
