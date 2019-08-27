#include "CharacterSet.h"

CharacterSet::CharacterSet() :
	BoolVector(256),
	Power(0)
{}

CharacterSet::CharacterSet(const char* str) :
	BoolVector(256)
{
	for (int i = strlen(str) - 1; i >= 0; i--)
		setOne(str[i]);
	Power = weight();
}

CharacterSet::CharacterSet(const CharacterSet& obj) :
	BoolVector(obj),
	Power(obj.Power)
{}

CharacterSet::CharacterSet(const BoolVector& vector) :
	BoolVector(vector, 256),
	Power(weight())
{}

void CharacterSet::set(int amount, ...)
{
	char param;
	va_list argptr;

	va_start(argptr, amount);

	for (; amount; amount--)
	{
		param = va_arg(argptr, char);
		setOne(param);
	}

	va_end(argptr);

	Power = weight();
}

void CharacterSet::set(const char* vector)
{
	for (int i = strlen(vector) - 1; i >= 0; i--)
		setOne(vector[i]);
	Power = weight();
}

std::string CharacterSet::get()
{
	std::string result;

	for (int i = 0; i < 256; i++)
		if (power() && BoolVector::operator[](i))
			result += char(i);

	return result;
}

/*CharacterSet CharacterSet::complement()
{
	return CharacterSet(~*this);
}*/

int CharacterSet::power()
{
	return Power;
}

void CharacterSet::clear()
{
	for (int i = 0; i < 32; i++)
		bv[i] = 0;
	Power = 0;
}

CharacterSet& CharacterSet::operator=(const CharacterSet& obj)
{
	if (this != &obj)
	{
		BoolVector::operator=(obj);
		Power = obj.Power;
	}
	return *this;
}

CharacterSet& CharacterSet::operator+=(const CharacterSet& obj)
{
	if (this != &obj)
	{
		BoolVector::operator|=(obj);
		Power = weight();
	}
	return *this;
}

CharacterSet CharacterSet::operator+(const CharacterSet& obj)
{
	if (this != &obj)
		return BoolVector::operator|(obj);

	return *this;
}

CharacterSet CharacterSet::operator+(char chr)
{
	if (bv[chr] == 0)
		Power++;
	setOne(chr);
	return *this;
}

CharacterSet& CharacterSet::operator-= (const CharacterSet& obj)
{
	for (int i = 0; i < 32; i++)
	{
		byte mask = 1;
		for (int j = 0; j < 8; j++, mask <<= 1)
			if (obj.bv[i] & mask && bv[i] & mask)
			{
				bv[i] &= ~mask;
				Power--;
			}
	}

	return *this;
}

CharacterSet CharacterSet::operator-(const CharacterSet& obj)
{
	return (CharacterSet(*this) -= obj);
}

CharacterSet CharacterSet::operator-(char chr)
{
	if (bv[chr] == 1)
		Power--;
	setZero(chr);
	return *this;
}

CharacterSet& CharacterSet::operator*=(const CharacterSet& obj)
{
	if (this != &obj)
	{
		//this->operator&=(obj);
		BoolVector::operator&=(obj);
		Power = weight();
	}
	return *this;
}

CharacterSet CharacterSet::operator*(const CharacterSet& obj)
{
	if (this != &obj)
		return BoolVector::operator&(obj);

	return *this;
}

CharacterSet CharacterSet::operator*(char chr)
{
	return BoolVector::operator&(chr);
}


CharacterSet CharacterSet::operator~()
{
	return BoolVector::operator~();
}

int CharacterSet::operator[](int index)
{
	return BoolVector::operator[](index);
}

void CharacterSet::print()
{
	for (int i = 0; i < 256; i++)
		if (BoolVector::operator[](i))
			std::cout << (char)i;
}

CharacterSet::~CharacterSet()
{}

std::istream& operator>>(std::istream& s, CharacterSet obj)
{
	int size;
	char chr;
	std::cout << "Enter number of characters : ";
	s >> size;
	for (int i = 0; i < size; i++)
	{
		s >> chr;
		obj.set(1, chr);
	}
	return s;
}

std::ostream& operator<<(std::ostream& s, CharacterSet obj)
{
	/*for (int i = 0; i < 256; i++)
		if (obj[i])
			s << (char)i;*/
	for (int i = 0; i < 256; i++)
		if (obj.BoolVector::operator[](i))
			s << (char)i;

	return s;
}