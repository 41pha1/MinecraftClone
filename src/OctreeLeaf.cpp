#include "OctreeLeaf.h"

#include <algorithm>

OctreeResult OctreeLeaf::get(char x, char y, char z, char depth, char size)
{
	char childID = 0;
	if(x >= size)
	{
		x-=size;
		childID += 4;
	}
	if(y >= size)
	{
		y-=size;
		childID += 2;
	}
	if(z >= size)
	{
		z-=size;
		childID += 1;
	}

	return {ids[childID], depth};
}

bool OctreeLeaf::put(char x, char y, char z, char depth, char size,
		char id_)
{
	char childID = 0;
	if(x >= size)
	{
		x-=size;
		childID += 4;
	}
	if(y >= size)
	{
		y-=size;
		childID += 2;
	}
	if(z >= size)
	{
		z-=size;
		childID += 1;
	}

	if(ids[childID] == id_)
		return false;

	ids[childID] = id_;

	return true;
}

bool OctreeLeaf::canCollapse()
{
	char content = ids[0];
	for(int i = 1; i < 8; i++)
		if(ids[i] != content)
			return false;
	return true;
}

int OctreeLeaf::size()
{
	return sizeof(*this);
}

std::string OctreeLeaf::getInfo(std::string padding, int firstDepth)
{
	std::string info = "";

	for(int i = 0; i < 8; i++)
	{
		int fd = (i == 0 ? firstDepth + 1 : 0);
		std::string pd = (i == 7 ? padding + "  " : padding + " |");
		std::string alternatePadding = "";
		for(int j = 0; j < fd; j++)
			alternatePadding += "--";

		pd = pd.substr(0, pd.size()-2*fd) +  alternatePadding;
		info += pd  +  "-> " + std::to_string(ids[i]) + "\n";
	}

	return info +  padding +"\n";
}


OctreeLeaf::OctreeLeaf(char type_)
{
	ids = std::array<char, 8>();
	ids.fill(type_);
}

bool OctreeLeaf::isLeaf()
{
	return true;
}

OctreeLeaf::OctreeLeaf(std::array<char, 8> ids_)
{
	ids = ids_;
}

OctreeLeaf::~OctreeLeaf()
{

}

std::array<char, 8> OctreeLeaf::getIDs()
{
	return ids;
}
