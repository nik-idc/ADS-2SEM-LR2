#include "stuff.h"
#include "symb.h"
#include "sf.h"
#include <Windows.h>

void showResult()
{
	unsigned char ch = 0; // For reading current character
	float codedSize = 0, decodedSize = 0;
	std::string codedStr, decodedStr, tableStr;

	std::ifstream fileReader;

	// Getting the size of coded
	fileReader.open("coded.txt", std::ifstream::ate | std::ifstream::binary);
	codedSize = fileReader.tellg();
	fileReader.close();

	// Reading the coded file
	fileReader.open("coded.txt", std::ifstream::in | std::ifstream::binary);
	while (fileReader >> std::noskipws >> ch)	
		codedStr.push_back(ch);
	fileReader.close();

	// Getting the size of decoded
	fileReader.open("decoded.txt", std::ifstream::ate | std::ifstream::binary);
	decodedSize = fileReader.tellg();
	fileReader.close();

	// Reading the decoded file
	fileReader.open("decoded.txt", std::ifstream::in | std::ifstream::binary);
	while (fileReader >> std::noskipws >> ch)	
		decodedStr.push_back(ch);
	fileReader.close();

	// Reading the decoded file
	fileReader.open("table.txt", std::ifstream::in | std::ifstream::binary);
	while (fileReader >> std::noskipws >> ch)
		tableStr.push_back(ch);
	fileReader.close();

	// Printing out info
	std::cout << "Table of codes: " << tableStr << std::endl;

	std::cout << "Coded - " << codedStr << std::endl;
	std::cout << "Decoded - " << decodedStr << std::endl;

	std::cout << "Coded size in bits - " << codedSize * 8 << "\nDecoded size in bits - " << decodedSize * 8 << std::endl;
	std::cout << "Coded size in bytes - " << codedSize << "\nDecoded size in bytes - " << decodedSize << std::endl;

	std::cout << "Coded size is " << decodedSize / codedSize << " times smaller than the size of the original message" << std::endl;
}

int main()
{
	try
	{
		SetConsoleCP(1251);
		SetConsoleOutputCP(1251);

		std::cout << "Please enter a message you would like to encode:" << std::endl;

		std::string testString;
		std::getline(std::cin, testString);

		sfCode s;
		s.encode(testString);
		s.decode();

		showResult();
	}
	catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
	}
}
