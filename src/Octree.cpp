#include "Octree.h"

#include <iostream>
#include <string>

#include "OctreeElement.h"
#include "OctreeLeaf.h"
#include "OctreeNode.h"


Octree::Octree(char size_)
{
	size = size_;
	tree = new OctreeLeaf(0);
}

void Octree::set(char x, char y, char z, char id)
{
	if(tree->isLeaf() && tree->getID() == id)
		return;

	if(size >> 1 != 0 && tree->isLeaf())
	{
		tree = new OctreeNode(0);
	}
	if(tree->put(x, y, z, 0, size >> 1, id))
	{
		if(!tree->isLeaf())
		{
			char id = tree->getChildID();
			delete tree;
			tree = new OctreeLeaf(id);
		}
	}
}

char Octree::get(char x, char y, char z)
{
	return tree->get(x, y, z, 0, size >> 1).id;
}

Octree::~Octree()
{

}

void Octree::print()
{
	std::cout << tree->getInfo("", 0) << std::endl;
}
