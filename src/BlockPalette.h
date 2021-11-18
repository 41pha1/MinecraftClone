#ifndef BLOCKPALETTE_H_
#define BLOCKPALETTE_H_

#include <map>
#include <vector>

#include "Bitarray.h"

struct Blockstate
{
	int id;

	Blockstate(int id_)
	{
		id = id_;
	}
};

class BlockPalette {
public:
	Bitarray * indices = new Bitarray(0,0);
	std::vector<Blockstate> palette = {};
	std::map<int, bool> stateExists = {};
	std::map<int, int> paletteIndex = {};
	int size, size2;
	int bitsPerIndex;

	BlockPalette(int size);
	int get(int x, int y, int z);
	void set(int x, int y, int z, int value);
	virtual ~BlockPalette();
};

#endif /* BLOCKPALETTE_H_ */
