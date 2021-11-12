#include "OctreeLeaf.h"

#include <string>

OctreeResult OctreeLeaf::get(char x, char y, char z, char depth, char size)
{
	return {id, depth};
}

bool OctreeLeaf::put(char x, char y, char z, char depth, char size,
		char id_)
{
	if(id == id_)
		return false;

	id = id_;

	return true;
}

bool OctreeLeaf::canCollapse()
{
	return false;
}

int OctreeLeaf::size()
{
	return sizeof(*this);
}

std::string OctreeLeaf::getInfo(std::string padding, int firstDepth)
{
	std::string info = "";
	std::string alternatePadding = "";
	for(int i = 0; i < firstDepth; i++)
		alternatePadding += "--";

	padding = padding.substr(0, padding.size()-2*firstDepth) +  alternatePadding;
	info += padding  +  "-> " + std::to_string(int(id)) + "\n";

	return info;
}


OctreeLeaf::OctreeLeaf(char type_)
{
	id = type_;
}

bool OctreeLeaf::isLeaf()
{
	return true;
}

char OctreeLeaf::getID()
{
	return id;
}

OctreeLeaf::~OctreeLeaf()
{

}

