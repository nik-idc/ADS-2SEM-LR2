#include "symb.h"

symb::symb()
{
	letter_v = 0;
	freq_v = 0;
	code_v = "";
}

symb::symb(int f)
{
	letter_v = 0;
	freq_v = f;
	code_v = "";

}

symb::symb(char l)
{
	letter_v = l;
	freq_v = 0;
	code_v = "";
}

symb::symb(char l, int f)
{
	letter_v = l;
	freq_v = f;
	code_v = "";
}

symb::symb(const symb &toCopy)
{
	letter_v = toCopy.letter_v;
	freq_v = toCopy.freq_v;
	code_v = toCopy.code_v;
}

symb &symb::operator=(const symb &right)
{
	letter_v = right.letter_v;
	freq_v = right.freq_v;
	code_v = right.code_v;
	return *this;
}

std::ostream &operator<<(std::ostream &os, const symb &out)
{
	os << out.letter_v << out.code_v;
	return os;
}

void symb::setLetter(char newLetter)
{
	letter_v = newLetter;
}

char symb::letter()
{
	return letter_v;
}

void symb::setFreq(int newFreq)
{
	freq_v = newFreq;
}

int symb::freq()
{
	return freq_v;
}

void symb::setCode(std::string c)
{
	code_v = c;
}

std::string symb::code()
{
	return code_v;
}