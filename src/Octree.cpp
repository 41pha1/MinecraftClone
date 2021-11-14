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
	if(tree->isLeaf() && tree->get(x,y,z,0, size>> 1).id == id)
		return;

	if(size >> 1 != 1 && tree->isLeaf())
	{
		tree = new OctreeNode(tree->getIDs());
	}
	if(tree->put(x, y, z, 0, size >> 1, id))
	{
		if(!tree->isLeaf())
		{
			std::array<char,8> ids = tree->getChildsContent();
			delete tree;
			tree = new OctreeLeaf(ids);
		}
	}
}

void Octree::set(char*** elements)
{
	for(int x = 0; x < size; x++)
		for(int y = 0; y < size; y++)
			for(int z = 0; z < size; z++)
				set(x,y,z,elements[x][y][z]);
}


char Octree::get(char x, char y, char z)
{
	return tree->get(x, y, z, 0, size >> 1).id;
}

Octree::~Octree()
{
	delete tree;
}

void Octree::print()
{
	std::cout << tree->getInfo("", 0) << std::endl;
}
