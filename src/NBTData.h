#ifndef NBTDATA_H_
#define NBTDATA_H_

#include <string>

class NBTData {
public:
	static const char TAG_END = 0, TAG_BYTE = 1, TAG_SHORT = 2, TAG_INT = 3, TAG_LONG = 4, TAG_FLOAT = 5, TAG_DOUBLE = 6, TAG_BYTE_ARRAY = 7, TAG_STRING = 8, TAG_LIST = 9, TAG_COMPOUND = 10, TAG_INT_ARRAY = 11, TAG_LONG_ARRAY = 12;

	void* data;
	int type;
	std::string name;

	NBTData(int type_, void* data_, std::string name_);
	void print(std::string padding);
	NBTData* get(std::string tagName);
	NBTData* get(int listIndex);
	std::string getTagType(char tagID);
	virtual ~NBTData();
};

#endif
