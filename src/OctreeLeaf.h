#ifndef OCTREELEAF_H_
#define OCTREELEAF_H_

#include <array>
#include <string>

#include "OctreeElement.h"

class OctreeLeaf : public OctreeElement
{
public:
	std::array<char,8> ids;

	OctreeLeaf(char type_ = 0);
	OctreeLeaf(std::array<char,8> ids);
	OctreeResult get(char x, char y, char z, char depth, char size);
	bool put(char x, char y, char z, char depth, char size, char id);
	bool canCollapse();
	bool isLeaf();
	int size();
	std::array<char, 8> getIDs();
	std::string getInfo(std::string padding, int firstDepth);
	virtual ~OctreeLeaf();
};

#endif /* OCTREELEAF_H_ */
