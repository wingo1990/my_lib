#ifndef _TABLE_H_
#define _TABLE_H_

/*
version:	1.0
author:		wingo.zhang

next version:
1.add exception handle. need write a exception class.
2.add primary key, is not same.
3.add error log.
*/


#include <string>

#include "TableResultSet.h"

typedef std::string TableName;
typedef std::string FieldName;
typedef unsigned int EntryID;
typedef unsigned int FieldID;

typedef enum tagXtableEnum
{
	DB_TABLE,
	FILE_TABLE,
	DEFAULT_TABLE = FILE_TABLE
}XTable;

class Table
{
public:
	virtual bool open_table(TableName) = 0;

	virtual void close_table() = 0;

	virtual bool create_table(TableName, std::string sclns) = 0;

	virtual bool delete_table(TableName) = 0;

	virtual bool insert_entry(std::string) = 0;

	virtual int delete_entry(std::string) = 0;

	virtual int update_entry(std::string, std::string) = 0;

	virtual TableResultSet *select_entry(std::string) = 0;

	virtual bool add_column(std::string, FieldID) = 0;

	virtual bool delete_column(std::string) = 0;

};

void TableConfiure(std::string conf, XTable xt=DEFAULT_TABLE);

Table *GetTableInstance(XTable xt=DEFAULT_TABLE);

#endif //_TABLE_H_
