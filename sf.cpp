#include "sf.h"
#include <bitset>

sfCode::sfCode()
{
	codedSize = 0;
	decodedSize = 0;
}

void sfCode::revQuickSort(list<symb> &l, int start, int end)
{
	long i = start, j = end;
	int temp, p;

	p = l[(start + end) / 2].freq(); // Middle element

	// Partitioning process
	do 
	{
		while (l[i].freq() > p)
			i++;
		while (l[j].freq() < p)
			j--;

		if (i <= j) 
		{
			symb temp = l[j];
			l[j] = l[i];
			l[i] = temp;
			i++; j--;
		}
	} while (i <= j);

	if (j > 0)
		revQuickSort(l, 0, j);
	if (end > i)
		revQuickSort(l, i, end);
}

void sfCode::split(list<symb> symbols, list<symb> &codes, std::string c, size_t &mem)
{
	if (symbols.size() > 1) // Split a node as int as it is inter than 1
	{
		list<symb> group1, group2;
		int freqSum1 = 0, freqSum2 = 0, size = symbols.size(), size1 = 0, size2 = 0;

		int k = 0;
		while (size1 + size2 != size) // Splitting into two groups with relatively equal sums
		{
			if (freqSum1 <= freqSum2)
			{
				freqSum1 += symbols[k].freq();
				group1.pushBack(symbols[k]);
				size1++;
			}
			else
			{
				freqSum2 += symbols[k].freq();
				group2.pushBack(symbols[k]);
				size2++;
			}
			k++;
		}

		// Coninuing building the tree
		split(group1, codes, c + '1', mem);
		split(group2, codes, c + '0', mem);
	}
	else // Reached a leaf
	{
		mem += c.size() * symbols[0].freq();
		symbols[0].setCode(c);	
		codes.popFront();
		codes.pushBack(symbols[0]);
	}
}

void sfCode::buildFreqList(std::string input, list<symb> &codes)
{
	size_t lettersCount = 0; // Variable for counting the amount of letters in the string
	for (size_t i = 0; i < input.size(); i++) // Going through the string
	{
		bool isThere = false; // 'false' if symbol is already there
		size_t curLetterCount = 0; // Variable for counting the times current letter has been repeated in the message
		for (size_t j = 0; j < codes.size(); j++) // Going through all known letters to see if current letter is already there
		{
			if (codes[j].letter() == input[i]) // If it's there setting isThere to true and going to another letter
			{
				isThere = true;
				break;
			}
		}

		if (!isThere) // If it's not there then add it to the list and count the amount of times it's repeated
		{
			codes.pushBack(input[i]);
			for (size_t j = i; j < input.size(); j++)
			{
				if (input[j] == input[i])
					curLetterCount++;
			}
			codes[lettersCount].setFreq(curLetterCount);
			lettersCount++;
		}
	}

	revQuickSort(codes, 0, lettersCount - 1); // Reverse sort the resulting list of symbols
}

void sfCode::storeData(std::string input, list<symb> &codes, size_t mem)
{
	size_t bytesAllocated = 0; // Amount of bytes to allocate for encoded message
	if (mem % 8 == 0)
		bytesAllocated = mem / 8;
	else
		bytesAllocated = (mem / 8) + 1;

	unsigned char *finCode = new unsigned char[bytesAllocated]; // Creating a char array of the necessary amount of bytes

	size_t bitsCount = 0, byteCount = 0; // 'bitsCount' - amount of bits in the current byte, 'count' - byte iterator
	unsigned char curByte = 0b00000000, movableBit = 0b10000000; // 'curByte' - variable to write current byte into, 'movableBit' - for bitwise operations

	for (size_t i = 0; i < input.size(); i++) // Going through the original string
	{
		for (size_t j = 0; j < codes.size(); j++) // Loop to find code of the current element in the string
		{
			if (input[i] == codes[j].letter()) // If current letter's code is found
			{
				for (size_t k = 0; k < codes[j].code().size(); k++) // Looping through the code string and storing it in bit form in the current byte
				{
					if (codes[j].code()[k] == '1') // If current bit is 1
					{
						curByte |= (movableBit >> bitsCount);
					}
					bitsCount++; // Moving one bit to the right
					if (bitsCount == 8) // If reached new byte
					{
						finCode[byteCount] = curByte;
						byteCount++; // Iterate through the byte array
						bitsCount = 0; // Resetting amount of bits to shift to the right in the current byte
						curByte = 0b00000000; // Resetting curByte
					}
				}
			}
		}
	}
	if (finCode[byteCount] != curByte && byteCount < bytesAllocated) // If last byte isn't initialised
		finCode[byteCount] = curByte;

	std::ofstream infoSaver; // For writing info into files
	infoSaver.open("coded.txt");
	for (size_t i = 0; i < bytesAllocated; i++) // Saving encoded information
	{
		infoSaver << finCode[i];
	}
	infoSaver.close();

	infoSaver.open("table.txt");
	for (size_t i = 0; i < codes.size(); i++) // Saving table of codes for decoding
	{
		infoSaver << codes[i];
		if(i < codes.size() - 1)
			infoSaver << ' ';
	}
	infoSaver.close();

	infoSaver.open("mem.txt");
	infoSaver << mem; // Writing amount of bits at the end
	infoSaver.close();

	delete[] finCode;
}

void sfCode::encode(std::string input)
{
	if (input.size() < 2)
		throw std::invalid_argument("ERROR: No point encoding a string less than 2 symbols long!");

	list<symb> codes, symbols; // 'codes' for int term containment, t - a variable for recursion
	size_t mem = 0; // Represents the exact amount of bits as a result of encoding

	buildFreqList(input, codes); // Building frequency list from the initial input string

	if (codes.size() > 1) // If message has more than 1 distinct symbol
	{
		symbols = codes;
		split(symbols, codes, "", mem); // Building a binary tree of sublists
	}
	else
	{
		codes[0].setCode("1");
		mem = codes[0].freq();
	}
	storeData(input, codes, mem); // Storing data into a .txt file
}

void sfCode::decode()
{
	list<symb> codes; // List that symbols and their codes will be written into
	size_t mem = 0; // Represents the exact amount of bits as a result of encoding

	std::ifstream infoReader;
	std::ofstream saveDecoded;
	infoReader.open("table.txt", std::ios_base::in | std::ios_base::binary); // Opening the table file to get the codes and the alphabet

	char curTableSymbol = 0; // Temporary variable to read symbols from file;
	std::string curCode = ""; // String variable to read current code
	bool first = false; // 'true' if current symbol is part of the original string, 'false' if it's part of the code
	size_t lettersCount = 0; // Iterator for letters

	while (infoReader >> std::noskipws >> curTableSymbol) // Read file until end
	{
		if (!first) // If its a part of the alphabet then add it to the list
		{
			first = true;
			codes.pushBack(curTableSymbol);
		}
		else // Otherwise it's the code for that symbol
		{
			do
				curCode.push_back(curTableSymbol);
			while (infoReader >> std::noskipws >> curTableSymbol && curTableSymbol != ' '); // Read the code until encounter a space or end of the file			
			
			codes[lettersCount].setCode(curCode); // Set the code for current letter in the alphabet
			lettersCount++; // Iterate
			curCode = "";

			first = false;
		}
	}

	infoReader.close();

	infoReader.open("mem.txt", std::ios_base::in | std::ios_base::binary); // Opening the file containing the amount of bits
	infoReader >> mem; // Reading info
	infoReader.close();

	infoReader.open("coded.txt", std::ios_base::in | std::ios_base::binary); // Opening the file containing the code
	saveDecoded.open("decoded.txt"); // Opening file to write decoded info to

	std::string decoded = ""; // Writing decoded information into 'decoded'
	unsigned char curSymbol = 0, movableBit; // 'curSymbol' - variable to write current symbol into, 'movableBit' - for bitwise operations

	size_t bitsCount = 0; // Bits iterator
	while (infoReader >> std::noskipws >> curSymbol)
	{
		movableBit = 0b10000000;
		for (size_t i = 0; i < 8 && bitsCount < mem; i++, bitsCount++) // Going through current character's bits
		{
			if ((movableBit >> i) & curSymbol)
				curCode += "1";
			else
				curCode += "0";
			for (size_t j = 0; j < codes.size(); j++) // Going through all the codes to see if current string of 1's and 0's is a code
			{
				if (curCode == codes[j].code())
				{
					decoded += codes[j].letter();
					saveDecoded << codes[j].letter();
					curCode = "";
					break;
				}
			}
		}
	}
	infoReader.close();
	saveDecoded.close();
}
