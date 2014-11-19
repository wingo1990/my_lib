#include <fstream>
#include <cassert>
#include <cstdio>
#include <regex>
#include <sys/stat.h>
#include <direct.h>

#include "FileTable.h"
#include "TableEntry.h"
#include "FileTableResultSet.h"

using namespace std;

std::string FileTable::_table_dir = ".\\";

void FileTable::Configure(std::string table_dir)
{
	_table_dir = table_dir;

	struct _stat fileStat;
	if (!((_stat(table_dir.c_str(), &fileStat) == 0) && (fileStat.st_mode & _S_IFDIR)))
	{
		// TODO: exception to handle.
		_mkdir(_table_dir.c_str());
	}
}

bool FileTable::open_table(TableName tn)
{
	assert(is_table_exist(tn));
	if (!is_table_exist(tn))
		return false;

	_table_name = tn;

	return read_all_column(_table_dir + _table_name) && \
                read_all_entry(_table_dir + _table_name);
}

void FileTable::close_table()
{
	table_flush();
	table_clear();
}

bool FileTable::create_table(TableName tn, std::string field_str)
{
	assert(!is_table_exist(tn));
	if (is_table_exist(tn))
		return false;

	_table_name = tn;

	int field_num = set_column(field_str);
	if (field_num == -1)
		return false;

	_field_num = field_num;

	return true;
}

bool FileTable::delete_table(TableName tn)
{
	return (!remove((_table_dir+tn).c_str()));
}

bool FileTable::add_column(const string str, FieldID id)
{
	assert(Field::is_field_string(str) && id <= (int)_field_num);
	if (!Field::is_field_string(str) || id > (int)_field_num)
		return false;

	Field field(str);
	_fields.insert(_fields.begin()+id, field);

	TableElement null_element(ElementTypeNULL);
	for (unsigned int i=0; i<_entry_num; i++)
		_entries[i].insert_element(id, null_element);
	
	++_field_num;

	return true;
}

bool FileTable::delete_column(string field_name)
{
	unsigned int off = 0;
	while (off<_field_num)
	{
		if (field_name == _fields[off].get_name())
			break;
		off++;
	}
	if (off == _field_num)
		return false;

	vector<Field>::iterator it = _fields.begin() + off;
	_fields.erase(it);
	_field_num--;

	for (unsigned int i=0; i<_entry_num; i++)
		_entries[i].delete_element(off);

	return true;
}

bool FileTable::insert_entry(std::string entry_string)
{
	assert(is_table_entry_string(entry_string));
	if (!is_table_entry_string(entry_string))
		return false;

	TableEntry te(&_fields, entry_string);

	_entries.push_back(te);
	_entry_num ++;

	return true;
}

int FileTable::update_entry(std::string setstr, std::string wherestr)
{
	// interpret condition string.
	string key, op, val;
	if (!expression_interpret(wherestr, key, op, val))
		return -1;
	CompareOperator wc = string2compareoperator(op);

	// interpret set string
	string field, fieldval;
	if (!expression_interpret(setstr, field, op, fieldval))
		return -1;
	int cid = get_column_id(field);

	// search and update.
	int updatenum = 0;
	for (unsigned int i=0; i<_entry_num; i++)
	{
		if (PreC(i, wc, key, val))
		{
			TableElement te(_fields[cid].get_type(), fieldval);
			_entries[i].set_specific_element(cid, te);
			updatenum ++;
		}
	}

	return updatenum;
}

TableResultSet* FileTable::select_entry(std::string wherestr)
{
	// interpret condition string.
	string key, op, val;
	if (!expression_interpret(wherestr, key, op, val))
		return NULL;
	CompareOperator wc = string2compareoperator(op);

	// search and add to results.
	FileTableResultSet *res = new FileTableResultSet;
	for (unsigned int i=0; i<_entry_num; i++)
	{
		if (PreC(i, wc, key, val))
			res->addEntry(_entries[i]);
	}
	for (unsigned int i=0; i<_field_num; i++)
		res->addFieldName(_fields[i].get_name());

	return res;
}

int FileTable::delete_entry(std::string wherestr)
{
	// interpret condition string.
	string key, op, val;
	if (!expression_interpret(wherestr, key, op, val))
		return false;
	CompareOperator wc = string2compareoperator(op);

	// search and remove it.
	int deletednum = 0;
	for (unsigned int i=0; i<_entry_num; )
	{
		if (PreC(i, wc, key, val))
		{
			// vector: delete the element, the later will move on it.
			_entries.erase(_entries.begin()+i);
			deletednum ++;
			_entry_num--;
		}
		else
			++i;
	}

	return deletednum;
}

bool FileTable::table_flush() 
{
	ofstream oss(_table_dir+_table_name, ios::trunc);
	oss.close();

	return write_all_column(_table_dir+_table_name) && write_all_entry(_table_dir+_table_name);
}

void FileTable::table_clear()
{
	_table_name = "";

	_fields.clear();
	_field_num = 0;

	_entries.clear();
	_entry_num = 0;
}

bool FileTable::read_all_column(string& file_name)
{
	ifstream ifs(file_name);
	if (!ifs.is_open())
		return false;

	// set column.
	string row;
	size_t pos = string::npos;
	while (getline(ifs, row))
	{
		pos = row.find(FTCN, 0);
		if (pos != string::npos)
			break;
	}

	// can not find FTCN.
	if (string::npos == pos)
		return false;

	// set _fields
	getline(ifs, row);
	if (-1 == set_column(row))
		return false;

	ifs.close();

	return true;
}

bool FileTable::write_all_column(string &file_name) 
{
	ofstream ofs(file_name, ios::app);
	if (!ofs.is_open())
		return false;

	// set FTCN
	ofs << FTCN << _field_num << "\n";

	// set column names
	for (unsigned int i=0; i<_field_num; ++i)
		ofs << _fields[i].str() << " ";
	ofs << "\n";

	ofs.close();

	return true;
}

bool FileTable::read_all_entry(string& file_name)
{
	ifstream ifs(file_name);
	if (!ifs.is_open())
		return false;

	// set entry number.
	string row;
	size_t pos = string::npos;
	while (getline(ifs, row))
	{
		pos = row.find(FTEN, 0);
		if (pos != string::npos)
			break;
	}

	// can not find FTEN.
	if (string::npos == pos)
		return false;

	row.erase(pos, FTEN_LEN);
	istringstream iss(row);
	iss >> _entry_num;

	// set entries
	for (unsigned int i=0; i<_entry_num; ++i)
	{
		getline(ifs, row);
		_entries.push_back(TableEntry(&_fields, row));
	}

	ifs.close();

	return true;
}

bool FileTable::write_all_entry(string &file_name) 
{
	ofstream ofs(file_name, ios::app);
	if (!ofs.is_open())
		return false;

	// set FTEN
	ofs << FTEN << _entry_num << "\n";

	// set entries
	for (unsigned int i=0; i<_entry_num; ++i)
		ofs << _entries[i].str()<< endl;

	ofs.close();

	return true;
}

// the `str` format must be "name-type name-type name-type..."
int FileTable::set_column(string str)
{
	assert(_fields.size() == 0);
	assert(_field_num == 0);

	regex reg("\\s+");
	sregex_token_iterator it(str.begin(), str.end(), reg, -1);
	sregex_token_iterator end;

	int field_num = 0;
	while (it != end)
	{
		string temp = it->str();
		if (temp.size())
		{
			if (Field::is_field_string(temp))
			{
				Field field(temp);
				_fields.push_back(field);
				field_num ++;
			}
			else
			{
				return -1;
			}

		}
		it++;
	}

	_field_num = field_num; 
	return field_num;
}

bool FileTable::PreC(EntryID eid, CompareOperator SC, FieldName name, string val)
{
	int cid = get_column_id(name);
	if (cid == _field_num)
		return false;

	TableElement te(_fields[cid].get_type(), val);
	TableElement temp = _entries[eid].get_specific_element(cid);

	switch (SC)
	{
	case OPERATOR_LESS:
		return temp < te;
	case OPERATOR_EQUAL:
		return temp == te;
	case OPERATOR_GREATER:
		return temp > te;
	default:
		assert(NULL);
		break;
	}

	return false;
}

bool FileTable::is_table_entry_string(std::string entry_string)
{
	regex reg("\\s");
	sregex_token_iterator it(entry_string.begin(), entry_string.end(), reg, -1);
	sregex_token_iterator end;

	int num = 0;
	while(it != end)
	{
		if (it->str().size())
			num ++;
		it++;
	}

	return num == _field_num;
}

inline bool FileTable::is_table_exist(std::string tn)
{
	ifstream ifs(_table_dir+tn);

	if (ifs)
		return true;
	else
		return false;
}

int FileTable::get_column_id(FieldName& name)
{
	unsigned int id = 0;
	while (id < _field_num)
	{
		if (_fields[id].get_name() == name)
			break;
		
		id++;
	}

	return id;
}

bool FileTable::expression_interpret(std::string& str, std::string& key, std::string& op, std::string& val)
{
	// ? is very import.
	regex reg("\\s*(.*?)\\s*(>|<|=)\\s*(.*?)\\s*");
	smatch sm;
	regex_match(str, sm, reg);

	if (sm.size() != 4)
		return false;

	key = sm[1];
	op = sm[2];
	val = sm[3];

	return true;
}

CompareOperator FileTable::string2compareoperator(string strc)
{
	if (strc == "=")
		return OPERATOR_EQUAL; 
	else if (strc == "<")
		return OPERATOR_LESS;
	else if (strc == ">")
		return OPERATOR_GREATER;
	else
		return OPERATOR_NULL;
}