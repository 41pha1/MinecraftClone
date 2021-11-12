#include "OctreeNode.h"
#include "OctreeLeaf.h"


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

	if(child->isLeaf() && child->getID() == id_)
		return false;

	if(size >> 1 != 0 && child->isLeaf())
	{
		children[childID] = std::shared_ptr<OctreeElement>(new OctreeNode(0));
		children[childID]->put(x, y, z, depth+1, size >> 1, id_);
		return false;
	}

	if(child->put(x, y, z, depth+1, size >> 1, id_))
	{
		if(!child->isLeaf())
		{
			char id = child->getChildID();
			children[childID].reset();
			children[childID] = std::shared_ptr<OctreeElement>(new OctreeLeaf(id));
		}
		return canCollapse();
	}
	return false;
}

bool OctreeNode::canCollapse()
{
	char content = children[0]->getID();

	if(content == NODE)
		return false;

	for(int i = 1; i < 8; i++)
		if(children[i]->getID() != content)
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
	{
		children[i] =  std::shared_ptr<OctreeElement>(new OctreeLeaf(0));
	}
}

bool OctreeNode::isLeaf()
{
	return false;
}

char OctreeNode::getID()
{
	return NODE;
}

char OctreeNode::getChildID()
{
	return children[0]->getID();
}

OctreeNode::~OctreeNode()
{

}

