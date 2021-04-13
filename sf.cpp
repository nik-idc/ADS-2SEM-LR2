#include "sf.h"
#include <bitset>

sfCode::sfCode()
{
	codedSize = 0;
	decodedSize = 0;
}

void sfCode::swap(int &el_1, int &el_2)
{
	int temp = el_2;
	el_2 = el_1;
	el_1 = temp;
}

int sfCode::partition(list<symb> &l, int start, int end)
{
	int pivot = l[(start + end) / 2].freq();
	int i = start - 1;
	int j = end + 1;
	while (1)
	{
		do
			i = i + 1;
		while (l[i].freq() > pivot);
		do
			j = j - 1;
		while (l[j].freq() < pivot);
		if (i >= j)
			return j;
		symb temp = l[j];
		l[j] = l[i];
		l[i] = temp;
	}
}

void sfCode::revQuickSort(list<symb> &l, int start, int end)
{
	if (start >= end) // Exit condition
		return;
	int index = partition(l, start, end); // First partition the array, i.e. put all elements <= pivot to the left of the pivot, and elements > pivot to the right of the pivot

	revQuickSort(l, start, index); //Sort sublist before the pivot
	revQuickSort(l, index + 1, end); //Sort sublist after the pivot
}

void sfCode::split(list<symb> symbols, list<symb> &codes, std::string c, size_t &mem)
{
	if (symbols.size() > 1) // Split a node as int as it is inter than 1
	{
		list<symb> group1, group2;
		int f1 = 0, f2 = 0, size = symbols.size(), size1 = 0, size2 = 0;

		int k = 0;
		while (size1 + size2 != size) // Splitting into two groups with relatively equal sums
		{
			if (f1 <= f2)
			{
				f1 += symbols[k].freq();
				group1.pushBack(symbols[k]);
				size1++;
			}
			else
			{
				f2 += symbols[k].freq();
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
	size_t bytesCount = 0; // Amount of bytes to allocate for encoded message
	if (mem % 8 == 0)
		bytesCount = mem / 8;
	else
		bytesCount = (mem / 8) + 1;

	unsigned char *finCode = new unsigned char[bytesCount]; // Creating a char array of the necessary amount of bytes

	size_t bitsCount = 0, count = 0; // 'bitsCount' - amount of bits in the current byte, 'count' - byte iterator
	unsigned char temp = 0b00000000, a = 0b10000000; // 'temp' - variable to write current byte into, 'a' - for bitwise operations

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
						temp |= (a >> bitsCount);
					}
					bitsCount++; // Moving one bit to the right
					if (bitsCount == 8) // If reached new byte
					{
						finCode[count] = temp; // Assign temp to current byte
						count++; // Iterate through the byte array
						bitsCount = 0; // Resetting amount of bits to shift to the right in the current byte
						temp = 0b00000000; // Resetting temp
					}
				}
			}
		}
	}
	if (finCode[count] != temp && count < bytesCount) // If last byte isn't initialised
		finCode[count] = temp;

	std::ofstream f; // For writing info into files
	f.open("coded.txt");
	for (size_t i = 0; i < bytesCount; i++) // Saving encoded information
	{
		f << finCode[i];
	}
	f.close();

	f.open("table.txt");
	for (size_t i = 0; i < codes.size(); i++) // Saving table of codes for decoding
	{
		f << codes[i];
		if(i < codes.size() - 1)
			f << ' ';
	}
	f.close();

	f.open("mem.txt");
	f << mem; // Writing amount of bits at the end
	f.close();

	delete[] finCode;
}

void sfCode::encode(std::string input)
{
	if (input.size() < 2)
		throw std::invalid_argument("ERROR: No point encoding a string less than 2 symbols long!");

	list<symb> codes, t; // 'codes' for int term containment, t - a variable for recursion
	size_t mem = 0; // Represents the exact amount of bits as a result of encoding

	//originalString = input;
	buildFreqList(input, codes); // Building frequency list from the initial input string

	if (codes.size() > 1) // If message has more than 1 distinct symbol
	{
		t = codes;
		split(t, codes, "", mem); // Building a binary tree of sublists
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

	std::ifstream f;
	std::ofstream writeDecoded;
	f.open("table.txt", std::ios_base::in | std::ios_base::binary); // Opening the table file to get the codes and the alphabet

	char curSymbol = 0; // Temporary variable to read symbols from file;
	std::string curCode = ""; // String variable to read current code
	bool first = false; // 'true' if current symbol is part of the original string, 'false' if it's part of the code
	size_t count = 0; // Iterator for letters

	while (f >> std::noskipws >> curSymbol) // Read file until end
	{
		if (!first) // If its a part of the alphabet then add it to the list
		{
			first = true;
			codes.pushBack(curSymbol);
		}
		else // Otherwise it's the code for that symbol
		{
			do
				curCode.push_back(curSymbol);
			while (f >> std::noskipws >> curSymbol && curSymbol != ' '); // Read the code until encounter a space or end of the file			
			
			codes[count].setCode(curCode); // Set the code for current letter in the alphabet
			count++; // Iterate
			curCode = "";

			first = false;
		}
	}

	f.close();

	f.open("mem.txt", std::ios_base::in | std::ios_base::binary); // Opening the file containing the amount of bits
	f >> mem; // Reading info
	f.close();

	f.open("coded.txt", std::ios_base::in | std::ios_base::binary); // Opening the file containing the code
	writeDecoded.open("decoded.txt"); // Opening file to write decoded info to

	std::string decoded = ""; // Writing the bits into 'bits' string, decoded information into 'decoded'
	unsigned char c = 0, temp; // 'c' for reading character, 'temp' for bitwise operations

	int k = 0; // Bits iterator
	std::string t = ""; // Temporary string to read current character's code
	while (f >> std::noskipws >> c)
	{
		temp = 0b10000000;
		for (size_t i = 0; i < 8 && k < mem; i++, k++) // Going through current character's bits
		{
			if ((temp >> i) & c)
				t += "1";
			else
				t += "0";
			for (size_t j = 0; j < codes.size(); j++) // Going through all the codes to see if current string of 1's and 0's is a code
			{
				if (t == codes[j].code())
				{
					decoded += codes[j].letter();
					writeDecoded << codes[j].letter();
					t = "";
					break;
				}
			}
		}
	}
	f.close();
	writeDecoded.close();
}