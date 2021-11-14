#ifndef NBTPARSER_H_
#define NBTPARSER_H_

#include <string>
#include <vector>
#include "NBTData.h"


struct parseResult
{
	int offset;
	NBTData * data;
};

class NBTParser {
public:
	NBTParser();
	static NBTData * parseData(std::vector<char> &data);
	static parseResult parseTag(std::vector<char> &data, int index);
	static parseResult parseTagPayload(std::vector<char> data, int index, char tagID, std::string name);
	static parseResult parseTagByte(std::vector<char> &data, int index, std::string name);
	static parseResult parseTagShort(std::vector<char> &data, int index, std::string name);
	static parseResult parseTagInt(std::vector<char> &data, int index, std::string name);
	static parseResult parseTagLong(std::vector<char> &data, int index, std::string name);
	static parseResult parseTagFloat(std::vector<char> &data, int index, std::string name);
	static parseResult parseTagDouble(std::vector<char> &data, int index, std::string name);
	static parseResult parseTagByteArray(std::vector<char> &data, int index, std::string name);
	static parseResult parseTagString(std::vector<char> &data, int index, std::string name);
	static parseResult parseTagList(std::vector<char> &data, int index, std::string name);
	static parseResult parseTagCompound(std::vector<char> &data, int index, std::string name);
	static parseResult parseTagIntArray(std::vector<char> &data, int index, std::string name);
	static parseResult parseTagLongArray(std::vector<char> &data, int index, std::string name);
	static short parseShort(std::vector<char> &data, int index);
	static int parseInt(std::vector<char> &data, int index);
	static long long parseLong(std::vector<char> &data, int index);
	virtual ~NBTParser();
};

#endif /* NBTPARSER_H_ */
