#ifndef OCTREEELEMENT_H_
#define OCTREEELEMENT_H_

#include <array>
#include <memory>

struct OctreeResult
{
	char id;
	char depth;
};

class OctreeElement;

class OctreeElement {
public:
	static const char NODE = -1;

	char id = -1;

	OctreeElement(char type_);
	OctreeResult get(char x, char y, char z, char depth, char size);
	void put(char x, char y, char z, char depth, char size, char id);
	std::array<std::shared_ptr<OctreeElement>, 8> children;
};

#endif /* OCTREEELEMENT_H_ */
