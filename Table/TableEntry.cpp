#include "TableEntry.h"

#include <string>
#include <cassert>
#include <iostream>
#include <sstream>
#include <regex>

using namespace std;

TableEntry::TableEntry(std::vector<Field>* pt, std::string str /* = "" */):_num(0), _ptypes(pt)
{	
	regex reg("\\s+");
	sregex_token_iterator it(str.begin(), str.end(), reg, -1);
	sregex_token_iterator end;

	int i = 0;
	while (it != end)
	{
		if (it->str().size())
		{
			TableElement te((*_ptypes)[i].get_type(), it->str());
			_entry.push_back(te);
			_num ++;
		}
		
		it++;
		i++;
	}
}

int TableEntry::size() const
{
	return _num;
}

void TableEntry::set_specific_element(FieldID id, TableElement &ele)
{
	assert(id < _num);

	_entry[id] = ele;
}

const TableElement& TableEntry::get_specific_element(FieldID id) const
{
	assert(id < _num);

	return _entry[id];
}

std::string TableEntry::str() 
{
	string ret;
	for (unsigned int i=0; i<_num; i++)
	{
		ret += _entry[i].get_value_as_string() + " ";
	}
	ret.erase(ret.size()-1);

	return ret;
}

bool TableEntry::insert_element(FieldID id, TableElement& tabe_element)
{
	assert(id <= _num);
	if (id > _num)
		return false;

	vector<TableElement>::iterator it = _entry.insert(_entry.begin()+id, tabe_element);
	size_t off = it - _entry.begin();

	if (off != id)
		return false;

	_num++;

	return true;
}

bool TableEntry::delete_element(FieldID id)
{
	assert(id < _num);
	if (id >= _num)
		return false;

	_entry.erase(_entry.begin() + id);

	_num--;

	return true;
}

