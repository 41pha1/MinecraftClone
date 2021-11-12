#ifndef OCTREEELEMENT_H_
#define OCTREEELEMENT_H_

#include <string>

struct OctreeResult
{
	char id;
	char depth;
};

class OctreeElement {
public:
	static const char NODE = -1;

	virtual OctreeResult get(char x, char y, char z, char depth, char size);
	virtual bool put(char x, char y, char z, char depth, char size, char id);
	virtual bool canCollapse();
	virtual bool isLeaf();
	virtual int size();
	virtual char getID();
	virtual char getChildID();
	virtual std::string getInfo(std::string padding, int firstDepth);
	virtual ~OctreeElement();
};

#endif /* OCTREEELEMENT_H_ */
