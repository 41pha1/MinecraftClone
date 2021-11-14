#include "OctreeNode.h"
#include "OctreeLeaf.h"


char OctreeNode::getChildID(char x, char y, char z, char size)
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
	return childID;
}

OctreeResult OctreeNode::get(char x, char y, char z, char depth, char size)
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
	return children[childID]->get(x, y, z, depth+1, size>>1);
}

bool OctreeNode::put(char x, char y, char z, char depth, char size,
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

	auto child = children[childID];

	if(child->isLeaf() && child->get(x,y,z,depth+1,size >> 1).id == id_)
		return false;

	if(size >> 1 != 1 && child->isLeaf())
	{
		children[childID] = std::shared_ptr<OctreeElement>(new OctreeNode(child->getIDs()));
		children[childID]->put(x, y, z, depth+1, size >> 1, id_);
		return false;
	}

	if(child->put(x, y, z, depth+1, size >> 1, id_))
	{
		if(!child->isLeaf())
		{
			std::array<char,8> ids = child->getChildsContent();
			children[childID].reset();
			children[childID] = std::shared_ptr<OctreeElement>(new OctreeLeaf(ids));
		}
		return canCollapse();
	}
	return false;
}

bool OctreeNode::canCollapse()
{
	for(int i = 0; i < 8; i++)
		if(!(children[i]->isLeaf() && children[i]->canCollapse()))
			return false;

	return true;
}

int OctreeNode::size()
{
	int childsize = 0;

	for(int i = 0; i < 8; i++)
		childsize += children[i]->size();

	return sizeof(*this) + childsize;
}

std::string OctreeNode::getInfo(std::string padding, int firstDepth)
{
	std::string info = "";

	for(int i = 0; i < 8; i++)
	{

		info +=  children[i]->getInfo(((i == 7 ? padding + "  " : padding + " |")), (i == 0 ? firstDepth + 1 : 0));
	}

	return info +  padding +"\n";
}

OctreeNode::OctreeNode(char type)
{
	for(int i = 0; i < 8; i++)
		children[i] =  std::shared_ptr<OctreeElement>(new OctreeLeaf(type));
}

OctreeNode::OctreeNode(std::array<char,8> types)
{
	for(int i = 0; i < 8; i++)
		children[i] =  std::shared_ptr<OctreeElement>(new OctreeLeaf(types[i]));
}

bool OctreeNode::isLeaf()
{
	return false;
}

std::array<char,8> OctreeNode::getChildsContent()
{
	std::array<char, 8> ids;
	for(int i = 0; i < 8; i++)
		ids[i] = children[i]->getIDs()[0];
	return ids;
}

OctreeNode::~OctreeNode()
{

}

