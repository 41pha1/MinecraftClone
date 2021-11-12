#ifndef OCTREELEAF_H_
#define OCTREELEAF_H_

#include <string>

#include "OctreeElement.h"

class OctreeLeaf : public OctreeElement
{
public:
	char id = 0;

	OctreeLeaf(char type_ = 0);
	OctreeResult get(char x, char y, char z, char depth, char size);
	bool put(char x, char y, char z, char depth, char size, char id);
	bool canCollapse();
	bool isLeaf();
	int size();
	char getID();
	std::string getInfo(std::string padding, int firstDepth);
	virtual ~OctreeLeaf();
};

#endif /* OCTREELEAF_H_ */
