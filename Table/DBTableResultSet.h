#ifndef _DB_TABLE_RESULTSET_H_
#define _DB_TABLE_RESULTSET_H_

#include "TableResultSet.h"
#include "mysql_public_iface.h"

class DBTableResultSet :
	public TableResultSet
{
public:
	DBTableResultSet(void *);
	~DBTableResultSet(void);

	void beforeFirst();
	void afterLast();
	bool first();
	bool last();
	bool next(void);
	bool previous(void);
	int getInt(const FieldName&);
	double getDouble(const FieldName&);
	std::string getString(const FieldName&);

private:
	sql::ResultSet* _sql_result_ptr;
};

#endif //_DB_TABLE_RESULTSET_H_

