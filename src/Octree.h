#ifndef OCTREE_H_
#define OCTREE_H_

#include "OctreeElement.h"

class Octree {
public:
	OctreeElement * tree;
	char size;

	char get(char x, char y, char z);
	void set(char x, char y, char z, char id);
	void set(char *** elements);

	void print();

	Octree(char size);
	virtual ~Octree();
};

#endif /* OCTREE_H_ */
