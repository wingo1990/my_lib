#ifndef _FILE_TABLE_H_
#define _FILE_TABLE_H_

//file table column number.
#define FTCN "FTCN:"
#define FTCN_LEN 5

//file table entry number.
#define FTEN "FTEN:"
#define FTEN_LEN 5

#include <vector>

#include "Table.h"
#include "Field.h"
#include "TableEntry.h"
#include "TableElement.h"

/*
		table file format
//version:x.x
//time:2014/09/34-21:34
...
FTCN:n\n
name-type name-type name-type			  ...... \n
...
FTEN:n\n
TableEntry[TableElement TableElement ...]		 \n
TableEntry[TableElement TableElement ...]		 \n
TableEntry[TableElement TableElement ...]		 \n
TableEntry[TableElement TableElement ...]        \n
TableEntry[TableElement TableElement ...]		 \n
...
...

*/

class FileTable : public Table
{
public:
	bool open_table(TableName);

	void close_table();

	bool create_table(TableName, std::string sclns);

	bool delete_table(TableName);

	bool insert_entry(std::string);

	// first parameter must be "name-type"
	bool add_column(std::string, FieldID id);
	// the parameter must be "name" not "name-type"
	bool delete_column(std::string);

	int update_entry(std::string, std::string);

	TableResultSet *select_entry(std::string);

	int delete_entry(std::string);

	FileTable(): _table_name(""), _field_num(0), _entry_num(0){}
	~FileTable(void){}

	//configure the table_dir.
	static void Configure(std::string);

private:
	TableName _table_name;
	unsigned int _field_num;
	unsigned int _entry_num;

	static std::string _table_dir;

	std::vector<TableEntry> _entries;

	std::vector<Field> _fields;

	// save to file
	bool table_flush() ;
	// clear object table
	void table_clear();

	// set _column_num and _fields from file.
	bool read_all_column(std::string&);
	// set _entry_num and _entries from file.
	bool read_all_entry(std::string&);
	// write _colums and _field_num to file.
	bool write_all_column(std::string&) ;
	// write _enties and _entry_num to file.
	bool write_all_entry(std::string&) ;

	int get_column_id(FieldName&);

	int get_field_num(std::string&);

	//TableElement createElement(ElementType et, std::string&);
	bool PreC(EntryID, CompareOperator, FieldName, std::string); 
	
	// set _field_types and _fields from `str`.
	int FileTable::set_column(std::string);

	bool is_table_entry_string(std::string);
	bool is_table_exist(std::string);
	bool expression_interpret(std::string&, std::string&, std::string&, std::string&);
	CompareOperator string2compareoperator(std::string strc);
};

#endif // _FILE_TABLE_H_

