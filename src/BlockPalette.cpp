#include "BlockPalette.h"

#include <iostream>
#include "Chunk.h"

BlockPalette::BlockPalette(int size_)
{
	size = size_;
	size2 = size*size;
	bitsPerIndex = 0;
	set(0,0,0,0);
}

int BlockPalette::get(int x, int y, int z)
{
	return palette[indices->get(y * size2 + x * size + z)].id;
}

void BlockPalette::set(int x, int y, int z, int value)
{
	if(stateExists[value])
	{
		indices->set(y * size2 + x * size + z, paletteIndex[value]);
	}else
	{
		palette.push_back(Blockstate(value));
		stateExists[value] = true;
		paletteIndex[value] = palette.size()-1;

		int newBitsPerIndex = (palette.size() <= 1 ? 0 : 32 - __builtin_clz(palette.size()-1));

		if(newBitsPerIndex > bitsPerIndex)
		{
			Bitarray * newIndices = new Bitarray(newBitsPerIndex, size2*size);

			for(int i = 0; i < size2*size; i++)
				newIndices->set(i, indices->get(i));

			delete indices;
			indices = newIndices;
		}
		indices->set(y * size2 + x * size + z, paletteIndex[value]);
	}
}

BlockPalette::~BlockPalette()
{
	delete indices;
}

