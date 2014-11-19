#include <sstream>
#include <cassert>
#include <algorithm>

#include "FileTableResultSet.h"

using namespace std;

FileTableResultSet::FileTableResultSet(void): _num_row(0), _num_field(0) ,_row_pos(-1)
{
}

FileTableResultSet::~FileTableResultSet(void)
{
}

void FileTableResultSet::beforeFirst()
{
	_row_pos = -1;
}

void FileTableResultSet::afterLast()
{
	_row_pos = _num_row;
}

bool FileTableResultSet::first()
{
	_row_pos = (_num_row ? 0 : -1);
	
	return true;
}

bool FileTableResultSet::last()
{
	_row_pos = _num_row - 1;

	return true;
}

bool FileTableResultSet::next(void)
{
	if (_row_pos < _num_row)
		++_row_pos;

	return (_row_pos < _num_row);
}

bool FileTableResultSet::previous(void)
{
	if (_row_pos > -1)
		--_row_pos;

	return (_row_pos > -1);
}

// TODO: add Exception handle.
int FileTableResultSet::getInt(const FieldName& name)
{
	assert(_row_pos>-1 && _row_pos<_num_row);

	int pos = findField(name);

	TableElement te = _res[_row_pos].get_specific_element(pos);

	int ret = te.get_value_as_int();

	return ret;
}
double FileTableResultSet::getDouble(const FieldName& name)
{
	assert(_row_pos>-1 && _row_pos<_num_row);

	int pos = findField(name);

	TableElement te = _res[_row_pos].get_specific_element(pos);

	double ret = te.get_value_as_double();

	return ret;
}
std::string FileTableResultSet::getString(const FieldName& name)
{
	assert(_row_pos>-1 && _row_pos<_num_row);

	int pos = findField(name);

	TableElement te = _res[_row_pos].get_specific_element(pos);

	string ret = te.get_value_as_string();

	return ret;
}

void FileTableResultSet::addEntry(TableEntry &te)
{
	_res.push_back(te);
	_num_row ++;
}

void FileTableResultSet::addFieldName(std::string& fieldname)
{
	_field_name.push_back(fieldname);
	_num_field ++;
}

int FileTableResultSet::findField(const FieldName& name)
{
	int pos = -1;
	vector<FieldName>::iterator it = find(_field_name.begin(), _field_name.end(), name);
	if (it != _field_name.end())
		pos = (int)(it - _field_name.begin());

	return pos;
}
