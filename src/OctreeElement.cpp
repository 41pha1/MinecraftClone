#include "OctreeElement.h"

#include <string>

OctreeResult OctreeElement::get(char x, char y, char z, char depth, char size)
{
	return {0,0};
}

bool OctreeElement::put(char x, char y, char z, char depth, char size,
		char id)
{
	return NULL;
}

bool OctreeElement::canCollapse()
{
	return NULL;
}

bool OctreeElement::isLeaf()
{
	return NULL;
}

int OctreeElement::size()
{
	return 0;
}

char OctreeElement::getID()
{
	return 0;
}

std::string OctreeElement::getInfo(std::string padding, int firstDepth)
{
	return "";
}

char OctreeElement::getChildID()
{
	return 0;
}

OctreeElement::~OctreeElement()
{

}
