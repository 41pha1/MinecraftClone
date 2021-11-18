
#ifndef BITARRAY_H_
#define BITARRAY_H_


class Bitarray {
public:
	char bitsPerElement;
	char * data;
	int bitmask;

	Bitarray(char bitsPerElement, int N);
	int get(int i);
	void set(int i, int value);
	virtual ~Bitarray();
};

#endif /* BITARRAY_H_ */
