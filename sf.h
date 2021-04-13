#pragma once
#ifndef SF_H
#define SF_H

#include "stuff.h"
#include "symb.h"

class sfCode
{
public:

	// Constructor
	sfCode();

	// Encode method
	void encode(std::string input);

	// Decode method
	void decode();

private:
	// Swap method for revQuickSort
	void swap(int &el_1, int &el_2);

	// Partition method for revQuickSort
	int partition(list<symb> &l, int start, int end);

	// Reverse quick sort
	void revQuickSort(list<symb> &l, int start, int end);

	// Tree building
	void split(list<symb> symbols, list<symb> &codes, std::string c, size_t &mem);
	
	// Method for building list of symbols and their frequencies
	void buildFreqList(std::string input, list<symb> &codes);

	// Method to store encoded data
	void storeData(std::string input, list<symb> &codes, size_t mem);

	size_t codedSize;
	size_t decodedSize;
};
#endif