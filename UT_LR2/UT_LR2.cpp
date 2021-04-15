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

			std::string testStr;

			size_t strSize = rand() % 100 + 1, codedSize = 0;

			for (size_t i = 0; i < strSize; i++)
				testStr.push_back(rand() % 255);

			sfCode testSF;

			testSF.encode(testStr);

			std::ifstream fileReader;
			fileReader.open("coded.txt", std::ifstream::ate | std::ifstream::binary);
			codedSize = fileReader.tellg();
			fileReader.close();

			Assert::IsTrue(codedSize < testStr.size());
		}

		TEST_METHOD(TestDecode)
		{
			srand(time(NULL));

			std::string testStr;

			size_t strSize = rand() % 100 + 1, codedSize = 0;

			for (size_t i = 0; i < strSize; i++)
				testStr.push_back(rand() % 255);

			sfCode testSF;

			testSF.encode(testStr);
			testSF.decode();

			char ch = 0;
			std::string decodedStr;
			std::ifstream fileReader;
			fileReader.open("decoded.txt", std::ifstream::in | std::ifstream::binary);
			while (fileReader >> std::noskipws >> ch)
				decodedStr.push_back(ch);
			fileReader.close();
			Assert::IsTrue(decodedStr == testStr);
		}
	};
}
