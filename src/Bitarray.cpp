#include "Bitarray.h"

#include <cmath>
#include <iostream>

Bitarray::Bitarray(char bitsPerElement_, int N)
{
	bitsPerElement = bitsPerElement_;
	bitmask = (1 << bitsPerElement)-1;
	data = new char[std::max(1,int(std::ceil((N*8)/((float)bitsPerElement))))];
}

int Bitarray::get(int i)
{
	int bitIndex = i * bitsPerElement;
	int index = bitIndex/8;
	int bitoffset = bitIndex % 8;

	int part = (data[index] & (unsigned char)(bitmask << bitoffset)) >> bitoffset;

	if((bitmask >> (8 - bitoffset)) == 0)
		return part;

	part |=   (unsigned char)(data[index+1] &  (bitmask >> (8 - bitoffset))) << (8 - bitoffset);

	if((bitmask >> (16 - bitoffset)) == 0)
			return part;

	part |=   (unsigned char)(data[index+1] &  (bitmask >> (16 - bitoffset))) << (16 - bitoffset);

	return part;
}

void Bitarray::set(int i, int value)
{
	int bitIndex = i * bitsPerElement;
	int index = bitIndex/8;
	int bitoffset = bitIndex % 8;

	data[index] = (data[index] & ~(bitmask << bitoffset))  | ((value & bitmask) << bitoffset);

	if((bitmask >> (8 - bitoffset)) == 0)
		return;

	data[index+1] =  (data[index+1] & ~(bitmask >> (8 - bitoffset)))  | ((value & bitmask) >> (8 - bitoffset));

	if((bitmask >> (16 - bitoffset)) == 0)
			return;

	data[index+2] =  (data[index+2] & ~(bitmask >> (16 - bitoffset)))  | ((value & bitmask) >> (16 - bitoffset));
}

Bitarray::~Bitarray()
{
	delete data;
}
