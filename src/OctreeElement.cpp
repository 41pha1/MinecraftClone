#include "OctreeElement.h"


OctreeResult OctreeElement::get(char x, char y, char z, char depth, char size)
{
	if(id != NODE)
		return {id, depth};

	int childID = 0;

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

	return children[childID]->get(x, y, z, depth+1, size>>1);
}

void OctreeElement::put(char x, char y, char z, char depth, char size,
		char id_)
{
	int childID = 0;

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

	if(id == NODE)
	{
		children[childID]->put(x, y, z, depth+1, size >> 1, id_);
	}else
	{
		if(size == 0)
			id = id_;
		else
		{
			id = NODE;
			for(int i = 0; i < 8; i++)
				children[i] = std::shared_ptr<OctreeElement>(new OctreeElement(0));
			children[childID]->put(x, y, z, depth+1, size >> 1, id_);
		}
	}

}

OctreeElement::OctreeElement(char type_)
{
	id = type_;

	if(id == NODE)
	{
		for(int i = 0; i < 8; i++)
			children[i] =std::shared_ptr<OctreeElement>(new OctreeElement(0));
	}
}

