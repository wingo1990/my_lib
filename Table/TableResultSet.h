#ifndef _TABLERESULTSET_H_
#define _TABLERESULTSET_H_

#include <string>

typedef std::string FieldName;

class TableResultSet
{
public:
	virtual void beforeFirst() = 0;
	virtual void afterLast() = 0;
	virtual bool first() = 0;
	virtual bool last() = 0;
	virtual bool next(void)  = 0;
	virtual bool previous(void) = 0;
	virtual int getInt(const FieldName&) = 0;
	virtual double getDouble(const FieldName&) = 0;
	virtual std::string getString(const FieldName&) = 0;
};

#endif //_TABLERESULTSET_H_