#include "NBTData.h"

#include <iostream>
#include <vector>

NBTData::NBTData(int type_, void* data_, std::string name_)
{
	type = type_;
	data = data_;
	name = name_;
}

void NBTData::print(std::string padding)
{
	std::cout << padding << "\"" << name << "\" " << getTagType(type) << ": ";

	switch (type){
		case NBTData::TAG_BYTE: std::cout << int(*((char*)data)) << std::endl; break;
		case NBTData::TAG_SHORT:  std::cout << *((short*)data) << std::endl; break;
		case NBTData::TAG_INT:  std::cout << *((int*)data) << std::endl; break;
		case NBTData::TAG_LONG: std::cout << *((long long*)data) << std::endl; break;
		case NBTData::TAG_FLOAT:  std::cout << *((float*)data) << std::endl; break;
		case NBTData::TAG_DOUBLE:  std::cout << *((double*)data) << std::endl; break;
		case NBTData::TAG_BYTE_ARRAY:
		{
			auto bytes = *(std::vector<char> * ) data;
			std::cout << bytes.size() << " Bytes: [";
			for(unsigned int i = 0; i < bytes.size(); i++)
				std::cout << int(bytes[i]) << ", ";
			std::cout << "]" << std::endl;
			break;
		}
		case NBTData::TAG_STRING:  std::cout << *((std::string*)data) << std::endl; break;
		case NBTData::TAG_LIST:
		{
			auto tags = *(std::vector<NBTData *> * ) data;
			std::cout << tags.size() << " Tags: {" << std::endl;
			for(unsigned int i = 0; i < tags.size(); i++)
				tags[i]->print(padding + "  ");
			std::cout << padding << "}" << std::endl;
			break;
		}
		case NBTData::TAG_COMPOUND:
		{
			auto tags = *(std::vector<NBTData *> * ) data;
			std::cout << tags.size() << " Tags: {" << std::endl;
			for(unsigned int i = 0; i < tags.size(); i++)
				tags[i]->print(padding + "  ");
			std::cout << padding <<  "}" << std::endl;
			break;
		}
		case NBTData::TAG_INT_ARRAY:
		{
			auto ints = *(std::vector<int> * ) data;
			std::cout << ints.size() << " Ints: [";
			for(unsigned int i = 0; i < ints.size(); i++)
				std::cout << int(ints[i]) << ", ";
			std::cout << "]" << std::endl;
			break;
		}

		case NBTData::TAG_LONG_ARRAY:
		{
			auto longs = *(std::vector<long long> * ) data;
			std::cout << longs.size() << " Longs: [";
			for(unsigned int i = 0; i < longs.size(); i++)
				std::cout << longs[i] << ", ";
			std::cout << "]" << std::endl;
			break;
		}
	}
}

std::string NBTData::getTagType(char tagID)
{
	switch (tagID){
		case NBTData::TAG_BYTE: return "BYTE";
		case NBTData::TAG_SHORT:  return "SHORT";
		case NBTData::TAG_INT:  return "INT";
		case NBTData::TAG_LONG:  return "LONG";
		case NBTData::TAG_FLOAT:  return "FLOAT";
		case NBTData::TAG_DOUBLE:  return "DOUBLE";
		case NBTData::TAG_BYTE_ARRAY:  return "BYTE_ARRAY";
		case NBTData::TAG_STRING:  return "STRING";
		case NBTData::TAG_LIST:  return "LIST";
		case NBTData::TAG_COMPOUND:  return "COMPOUND";
		case NBTData::TAG_INT_ARRAY: return "INT_ARRAY";
		case NBTData::TAG_LONG_ARRAY: return "LONG_ARRAY";
	}
	return "INVALID TAG";
}

NBTData* NBTData::get(std::string tagName)
{
	if(type != NBTData::TAG_COMPOUND)
	{
		std::cerr << "Can only select tag by name from compound tag!" << std::endl;
		return 0;
	}

	for(NBTData * tag : *(std::vector<NBTData *> * ) data)
		if(tag->name == tagName)
			return tag;

	return 0;	//return 0 if no tag with the name was found.
}

NBTData* NBTData::get(int listIndex)
{
	if(type != NBTData::TAG_LIST)
	{
		std::cerr << "Can only select tag by index from list tag!" << std::endl;
		return 0;
	}
	if(listIndex < 0 || (unsigned int)listIndex >= ((std::vector<NBTData *> * ) data)->size())
	{
		std::cerr << "List index out of range" << std::endl;
		return 0;
	}

	return (*((std::vector<NBTData*> * ) data)) [listIndex];
}

NBTData::~NBTData()
{
	switch (type)
	{
		case NBTData::TAG_BYTE: delete (char*)data; break;
		case NBTData::TAG_SHORT:  delete (short*)data; break;
		case NBTData::TAG_INT:  delete (int*)data; break;
		case NBTData::TAG_LONG:  delete (long long*)data; break;
		case NBTData::TAG_FLOAT:  delete (float*)data; break;
		case NBTData::TAG_DOUBLE:  delete (double*)data; break;
		case NBTData::TAG_BYTE_ARRAY:  delete (std::vector<char> *)data; break;
		case NBTData::TAG_STRING:  delete (std::string *)data; break;
		case NBTData::TAG_COMPOUND:
		case NBTData::TAG_LIST:
		{
			for(NBTData * tag : *(std::vector<NBTData *> *)data)
				delete tag;
			delete (std::vector<NBTData *> *)data; break;
		}
		case NBTData::TAG_INT_ARRAY: delete (std::vector<int> *)data; break;
		case NBTData::TAG_LONG_ARRAY: delete (std::vector<long long> *)data; break;
	}
}

