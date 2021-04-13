#pragma once
#ifndef SYMB_H
#define SYMB_H

#include <iostream>
#include <fstream>
#include <string>

class symb // Class that contains: the letter, its frequency in the string and its code
{
public:

	symb();

	symb(int f);

	symb(char l);

	symb(char l, int f);

	symb(const symb &toCopy);

	symb &operator=(const symb &right);

	friend std::ostream& operator<<(std::ostream &os, const symb &out);

	void setLetter(char newLetter);

	char letter();

	void setFreq(int newFreq);

	int freq();

	void setCode(std::string c);

	std::string code();

private:
	char letter_v;
	int freq_v;
	std::string code_v;
};
#endif