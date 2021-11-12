#ifndef OCTREENODE_H_
#define OCTREENODE_H_

#include <array>
#include <memory>
#include <string>

#include "OctreeElement.h"

class OctreeNode : public OctreeElement
{
public:
	std::array<std::shared_ptr<OctreeElement>, 8> children;

	OctreeNode(char type_ = 0);
	OctreeResult get(char x, char y, char z, char depth, char size);
	bool put(char x, char y, char z, char depth, char size, char id);
	bool canCollapse();
	bool isLeaf();
	int size();
	char getID();
	char getChildID();
	std::string getInfo(std::string padding, int firstDepth);
	virtual ~OctreeNode();
};

#endif /* OCTREENODE_H_ */
