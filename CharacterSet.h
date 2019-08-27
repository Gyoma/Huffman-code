#pragma once
#include <cstdarg>
#include "BoolVector.h"

class CharacterSet : private BoolVector
{
public:

	CharacterSet();

	CharacterSet(const char* str);

	CharacterSet(const CharacterSet& obj);

	CharacterSet(const BoolVector& vector);

	void set(int amount, ...);

	void set(const char* vector);

	std::string get();

	//CharacterSet complement();

	int power();

	void clear();

	CharacterSet& operator=(const CharacterSet& obj);

	CharacterSet & operator+=(const CharacterSet& obj);

	CharacterSet operator+(const CharacterSet& obj);

	CharacterSet operator+(char chr);

	CharacterSet& operator-= (const CharacterSet& obj);

	CharacterSet operator-(const CharacterSet& obj);

	CharacterSet operator-(char chr);

	CharacterSet& operator*=(const CharacterSet& obj);

	CharacterSet operator*(const CharacterSet& obj);

	CharacterSet operator*(char chr);

	CharacterSet operator~();

	int operator[](int index);

	void print();

	friend std::istream& operator>>(std::istream& s, CharacterSet obj);
	friend std::ostream& operator<<(std::ostream& s, CharacterSet obj);

	~CharacterSet();

private:
	int Power;
};