#ifndef _FILE_TABLE_RESULTSET_H_
#define _FILE_TABLE_RESULTSET_H_

#include <vector>

#include "Table.h"
#include "TableEntry.h"
#include "TableResultset.h"

class FileTableResultSet :
	public TableResultSet
{
public:
	FileTableResultSet(void);
	~FileTableResultSet(void);

	void beforeFirst();
	void afterLast();
	bool first();
	bool last();
	bool next(void);
	bool previous(void);
	int getInt(const FieldName&);
	double getDouble(const FieldName&);
	std::string getString(const FieldName&);

	void addEntry(TableEntry &te);
	void addFieldName(FieldName&);

private:
	int _row_pos;
	int _num_row;
	int _num_field;

	std::vector<TableEntry> _res;

	std::vector<FieldName> _field_name;

	int findField(const FieldName&);
};

#endif //_FILE_TABLE_RESULTSET_H_


