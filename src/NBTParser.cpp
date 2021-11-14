#include "NBTParser.h"

#include <iostream>
#include "NBTData.h"


NBTParser::NBTParser()
{

}

NBTData * NBTParser::parseData(std::vector<char>& data)
{
	return parseTag(data, 0).data;
}

parseResult NBTParser::parseTag(std::vector<char>& data, int index)
{
	char tagType = data[index];

	if(tagType == NBTData::TAG_END)
	{
		std::cerr << "unexspected end tag" << std::endl;
		return {index+1, new NBTData(NBTData::TAG_END, 0, "")};
	}

	int nameLength = (int)(unsigned char)(data[index + 1]) << 8 | (unsigned char)(data[index + 2]);

	std::string tagName = "";

	for(int i = 0; i < nameLength; i++)
		tagName += data[index + 3 + i];

	return parseTagPayload(data,  index + nameLength + 3, tagType, tagName);
}

parseResult NBTParser::parseTagPayload(std::vector<char> data, int index, char tagID, std::string name)
{
	switch(tagID)
	{
		case NBTData::TAG_BYTE: return parseTagByte(data, index, name);
		case NBTData::TAG_SHORT: return parseTagShort(data,index, name);
		case NBTData::TAG_INT: return parseTagInt(data, index, name);
		case NBTData::TAG_LONG: return parseTagLong(data, index, name);
		case NBTData::TAG_FLOAT: return parseTagFloat(data, index, name);
		case NBTData::TAG_DOUBLE: return parseTagDouble(data, index, name);
		case NBTData::TAG_BYTE_ARRAY: return parseTagByteArray(data, index, name);
		case NBTData::TAG_STRING: return parseTagString(data, index, name);
		case NBTData::TAG_LIST: return parseTagList(data, index, name);
		case NBTData::TAG_COMPOUND: return parseTagCompound(data, index, name);
		case NBTData::TAG_INT_ARRAY: return parseTagIntArray(data, index, name);
		case NBTData::TAG_LONG_ARRAY: return parseTagLongArray(data, index, name);
		default: std::cerr << "Unsupported NBT Tag of type " <<  int(tagID) << std::endl;
	}
	return {index+1, new NBTData(NBTData::TAG_END, 0, "")};
}

parseResult NBTParser::parseTagByte(std::vector<char>& data, int index, std::string name)
{
	char * value = new char(data[index]);
	return {index+1, new NBTData(NBTData::TAG_BYTE, value, name)};
}

parseResult NBTParser::parseTagShort(std::vector<char>& data, int index, std::string name)
{
	short * value = new short(parseShort(data, index));
	return  {index+2, new NBTData(NBTData::TAG_SHORT, value, name)};
}

parseResult NBTParser::parseTagInt(std::vector<char>& data, int index, std::string name)
{
	int * value = new int(parseInt(data, index));
	return  {index+4, new NBTData(NBTData::TAG_INT, value, name)};
}

parseResult NBTParser::parseTagLong(std::vector<char>& data, int index, std::string name)
{
	long long * value = new (long long)(parseLong(data, index));
	return  {index+8, new NBTData(NBTData::TAG_LONG, value, name)};
}

parseResult NBTParser::parseTagFloat(std::vector<char>& data, int index, std::string name)
{
	float * value = new float(*((float*) (&data[index])));
	return {index+4, new NBTData(NBTData::TAG_FLOAT, value, name)};
}

parseResult NBTParser::parseTagDouble(std::vector<char>& data, int index, std::string name)
{
	double * value = new double (*((double*) (&data[index])));
	return {index+8, new NBTData(NBTData::TAG_DOUBLE, value, name)};
}

parseResult NBTParser::parseTagString(std::vector<char>& data, int index, std::string name)
{
	int stringLength = parseShort(data, index);

	std::string * value = new std::string("");
	for(int i = 0; i < stringLength; i++)
		value->operator+=(data[index + 2 + i]);

	return {index + 2 + stringLength, new NBTData(NBTData::TAG_STRING, value, name)};
}

parseResult NBTParser::parseTagList(std::vector<char>& data, int index, std::string name)
{
	char tagID = data[index];
	int size = parseInt(data, index+1);
	index += 5;

	std::vector<NBTData *> * tagList = new std::vector<NBTData *>;

	for(int i = 0; i < size; i++)
	{
		auto result = parseTagPayload(data, index, tagID, "");
		index = result.offset;
		tagList->push_back(result.data);
	}

	return {index, new NBTData(NBTData::TAG_LIST, tagList, name)};
}

parseResult NBTParser::parseTagCompound(std::vector<char>& data, int index, std::string name)
{
	std::vector<NBTData *> * compoundData = new std::vector<NBTData *>;

	while(data[index] != NBTData::TAG_END)
	{
		auto result = parseTag(data, index);
		index = result.offset;
		compoundData->push_back(result.data);
	}

	return {index + 1, new NBTData(NBTData::TAG_COMPOUND, compoundData, name)};
}

parseResult NBTParser::parseTagByteArray(std::vector<char>& data, int index, std::string name)
{
	int count = parseInt(data, index);
	std::vector<char> * values = new std::vector<char>;

	for(int i = 0; i < count; i++)
	{
		values->push_back(data[index+i+4]);
	}

	return {index + count + 4, new NBTData(NBTData::TAG_BYTE_ARRAY, values, name)};
}


parseResult NBTParser::parseTagIntArray(std::vector<char>& data, int index, std::string name)
{
	int count = parseInt(data, index);
	std::vector<int> * values = new std::vector<int>;

	for(int i = 0; i < count; i++)
	{
		values->push_back(parseInt(data, index+i*4+4));
	}

	return {index + 4 * count + 4, new NBTData(NBTData::TAG_INT_ARRAY, values, name)};
}

parseResult NBTParser::parseTagLongArray(std::vector<char>& data, int index, std::string name)
{
	int count = parseInt(data, index);
	std::vector<long long> * values = new std::vector<long long>;

	for(int i = 0; i < count; i++)
	{
		values->push_back(parseLong(data, index+i*8+4));
	}

	return {index + 8 * count + 4, new NBTData(NBTData::TAG_LONG_ARRAY, values, name)};
}

short NBTParser::parseShort(std::vector<char>& data, int index)
{
	return (short)((unsigned char)(data[index]) << 8 | (unsigned char)(data[index + 1]));
}

int NBTParser::parseInt(std::vector<char>& data, int index)
{
	return (int)((unsigned char)(data[index]) << 24 | (unsigned char)(data[index + 1]) << 16| (unsigned char) (data[index+2]) << 8 | (unsigned char)(data[index + 3]));
}

long long NBTParser::parseLong(std::vector<char>& data, int index)
{
	long long value = 0;
	for(int i = 0; i < 8; i ++)
	{
		value |= ((long long)((unsigned char)(data[index + 7 - i])) << (i * 8));
	}
	return value;
}

NBTParser::~NBTParser()
{

}
